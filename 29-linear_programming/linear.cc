#include "../28-matrix_operations/lib.h"

#include <limits>
#include <algorithm>
#include <limits>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

template <typename T>
void AuxPrint(T &x, string prefix = "") {
  ostream_iterator<typename T::value_type> out(cout, " ");
  cout << prefix;
  copy(x.begin(), x.end(), out);
  cout << endl;
}

class SlackForm {
 public:
  SlackForm() {}

  void SetB(vector<float> &b) { b_ = b; }
  void SetC(vector<float> &c) { c_ = c; }
  void SetA(Matrix<float> &a) {
    a_ = a;
    N_.resize(a.NCol());
    B_.resize(a.NRow());
    for (size_t i = 0; i < a.NCol(); ++i) {
      N_[i] = i;
    }
    for (size_t i = 0; i < a.NRow(); ++i) {
      B_[i] = i + N_.size();
    }
    v_ = 0;
  }

 private:
  bool IsBasic(size_t idx) {
    return find(B_.begin(), B_.end(), idx) != B_.end();
  }

  size_t NIdx(size_t idx) {
    for (size_t i = 0; i < N_.size(); ++i) {
      if (N_[i] == idx) return i;
    }
    cerr << "not in N_" << endl;
    return 0x888;
  }

  size_t BIdx(size_t idx) {
    for (size_t i = 0; i < B_.size(); ++i) {
      if (B_[i] == idx) return i;
    }
    cerr << "not in B_" << endl;
    return 0x888;
  }

  float GetValue(size_t idx) {
    if (IsBasic(idx)) {
      for (size_t i = 0; i < B_.size(); ++i) {
        if (B_[i] == idx) {
          return b_[i];
        }
      }
    } else {
      return 0;
    }
    cerr << "cannot get value" << endl;
    return 0x888;
  }

  void Pivot(size_t l, size_t e) {
    auto n = a_.NCol();
    Matrix<float> newA(Dimension(a_.NRow(), a_.NCol()));
    vector<float> newb(b_.size());
    vector<float> newc(c_.size());

    newb[l] = b_[l] / a_[l * n + e];
    for (size_t j = 0; j < N_.size(); ++j) {
      if (j != e) {
        newA[l * n + j] = a_[l * n + j] / a_[l * n + e];
      }
    }
    newA[l * n + e] = 1.0f / a_[l * n + e];
    for (size_t i = 0; i < B_.size(); ++i) {
      if (i != l) {
        newb[i] = b_[i] - a_[i * n + e] * newb[l];
        for (size_t j = 0; j < N_.size(); ++j) {
          if (j != e) {
            newA[i * n + j] = a_[i * n + j] - a_[i * n + e] * newA[l * n + j];
          }
          newA[i * n + e] = -a_[i * n + e] * newA[l * n + e];
        }
      }
    }

    auto newv = v_ + c_[e] * newb[l];
    for (size_t j = 0; j < N_.size(); ++j) {
      if (j != e) {
        newc[j] = c_[j] - c_[e] * newA[l * n + j];
      }
    }
    newc[e] = -c_[e] * newA[l * n + e];

    swap(N_[e], B_[l]);
    a_ = newA;
    v_ = newv;
    b_ = newb;
    c_ = newc;
  }

  bool InitSimplex(void) {
    int k = -1;
    int min_b = numeric_limits<int>::max();
    for (size_t i = 0; i < b_.size(); ++i) {
      k = b_[i] < min_b ? (min_b = b_[i], i) : k;
    }
    if (b_[k] >= 0) {
      return true;
    }

    auto n = a_.NCol();

    SlackForm new_slackform;
    vector<float> newC(c_.size() + 1, 0);
    newC[0] = -1;
    Matrix<float> newA(Dimension(a_.NRow(), a_.NCol() + 1));
    for (size_t i = 0; i < a_.NRow(); ++i) {
      for (size_t j = 0; j < a_.NCol(); ++j) {
        newA[i * (n + 1) + j + 1] = a_[i * n + j];
      }
      newA[i * (n + 1)] = -1;
    }

    new_slackform.SetB(b_);
    new_slackform.SetA(newA);
    new_slackform.SetC(newC);

    new_slackform.Pivot(k, 0);

    vector<float> solution;
    if (new_slackform.Simplex(solution)) {
      if (GetValue(0) != 0) {
        return false;
      }
      if (IsBasic(0)) {
        new_slackform.Pivot(new_slackform.BIdx(0), 0);
      }

      auto col = new_slackform.NIdx(0);
      newA = new_slackform.a_;
      for (size_t i = 0; i < newA.NRow(); ++i) {
        size_t _j = 0;
        for (size_t j = 0; j < newA.NCol(); ++j) {
          if (j != col) {
            a_[i * n + _j] = newA[i * (n + 1) + j];
            ++_j;
          }
        }
      }

      b_ = new_slackform.b_;
      transform(new_slackform.B_.begin(), new_slackform.B_.end(), B_.begin(),
                [](size_t x) { return x - 1; });

      copy_if(new_slackform.N_.begin(), new_slackform.N_.end(), N_.begin(),
              [&](size_t x) { return x != col; });
      transform(N_.begin(), N_.end(), N_.begin(),
                [](size_t x) { return x - 1; });

      vector<float> _c(c_.size());
      for (size_t i = 0; i < N_.size(); ++i) {
        if (new_slackform.IsBasic(i + 1)) {
          size_t row = new_slackform.BIdx(i + 1); // index + 1 in new_slackform
          for (size_t j = 0; j < _c.size(); ++j) {
            // _c[new_slackform.N_[j]] -= c_[i] * a_[row * n + j];
            _c[j] -= c_[i] * a_[row * n + j];
            v_ += c_[i] * b_[i];
          }
        } else {
          _c[new_slackform.NIdx(i + 1)] += c_[i];
        }
      }
      copy(_c.begin(), _c.end(), c_.begin());

      DebugPrint();
      return true;
    } else {
      cout << "cannot init" << endl;
      return false;
    }
  }

 public:
  bool Simplex(vector<float> &solution) {
    if (!InitSimplex()) {
      return false;
    }
    // cout << "init complete" << endl;

    const size_t kTag = 10000;
    auto n = a_.NCol();

    while (true) {
      // cout << "hhh" << endl;
      size_t e = kTag, min_n = kTag;
      // Bland rules
      for (size_t i = 0; i < N_.size(); ++i) {
        if (c_[i] > 0) {
          e = N_[i] < min_n ? (min_n = N_[i], i) : (e);
        }
      }
      if (e == kTag) {
        break;
      }
      // cout << "e is: " << e << endl;
      // cout << "min_n is: " << min_n << endl;
      size_t l = kTag, min_delta = kTag;
      for (size_t i = 0; i < B_.size(); ++i) {
        if (a_[i * n + e] > 0) {
          auto delta = b_[i] / a_[i * n + e];
          if (delta < min_delta) {
            l = i;
            min_delta = delta;
          }
        }
      }
      if (l == kTag) {
        return false;
      }

      Pivot(l, e);
    }

    solution.resize(c_.size(), 0);
    for (size_t i = 0; i < B_.size(); ++i) {
      size_t idx = B_[i];
      if (idx < N_.size()) {
        solution[idx] = b_[i];
      }
    }

    return true;
  }

  void DebugPrint(void) {
    cout << __func__ << "----------" << endl;
    a_.Display();
    ostream_iterator<float> out(cout, " ");
    cout << "b: ";
    copy(b_.begin(), b_.end(), out);
    cout << endl;
    cout << "c: ";
    copy(c_.begin(), c_.end(), out);
    cout << endl;
    cout << "N: ";
    copy(N_.begin(), N_.end(), out);
    cout << endl;
    cout << "B: ";
    copy(B_.begin(), B_.end(), out);
    cout << endl;
    cout << "v: " << v_ << endl;
    cout << "--------------------" << endl;
  }

 private:
  vector<size_t> B_;
  vector<size_t> N_;
  vector<float> b_;
  vector<float> c_;
  vector<float> result_;
  Matrix<float> a_;
  float v_;
};  // class: SlackForm

void Test0(void) {
  SlackForm slack_form;
  vector<float> b_ = {
      30, 24, 36,
  };
  vector<float> c_ = {
      3, 1, 2,
  };
  vector<float> a_ = {
      1, 1, 3, 2, 2, 5, 4, 1, 2,
  };
  Matrix<float> a(Dimension(3, 3));
  a.Fill(a_);
  slack_form.SetB(b_);
  slack_form.SetC(c_);
  slack_form.SetA(a);

  vector<float> solution;
  slack_form.Simplex(solution);
  for (auto x : solution) {
    cout << x << " ";
  }
  cout << endl;
}

void Test1(void) {
  SlackForm slack_form;
  vector<float> b_ = {2, -4};
  vector<float> c_ = {2, -1};
  vector<float> a_ = {2, -1, 1, -5};
  Matrix<float> a(Dimension(2, 2));
  a.Fill(a_);
  slack_form.SetB(b_);
  slack_form.SetC(c_);
  slack_form.SetA(a);

  vector<float> solution;
  slack_form.Simplex(solution);
  slack_form.DebugPrint();
  for (auto x : solution) {
    cout << x << " ";
  }
  cout << endl;
}

int main(void) {
  Test1();

  return 0;
}
