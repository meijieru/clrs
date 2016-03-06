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
  }

  size_t BIdx(size_t idx) {
    for (size_t i = 0; i < B_.size(); ++i) {
      if (B_[i] == idx) return i;
    }
    cerr << "not in B_" << endl;
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
      if (b_[i] > 0) {
        k = b_[i] < min_b ? (min_b = b_[i], i) : k;
      }
    }
    if (k == -1) {
      return false;
    }

    auto n = a_.NCol();

    SlackForm new_slackform;
    vector<float> newC(c_.size() + 1, 0);
    newC[0] = -1;
    Matrix<float> newA(Dimension(a_.NRow(), a_.NCol() + 1));
    for (size_t i = 0; i < a_.NRow(); ++i) {
      for (size_t j = 0; j < a_.NCol(); ++j) {
        newA[i * n + j + 1] = a_[i * n + j];
      }
      newA[i * n] = -1;
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
        // TODO : Pivot the 0 to be the non_basic var
      }

      // TODO : restore the value
      for (size_t i = 0; i < a_.NRow(); ++i) {
        for (size_t j = 0; j < a_.NCol(); ++j) {
          a_[i * n + j] = newA[i * (n + 1) + j + 1];
        }
      }
      b_ = new_slackform.b_;
    }
  }

 public:
  bool Simplex(vector<float> &solution) {
    // InitSimplex();

    const size_t kTag = 10000;
    auto n = a_.NCol();

    while (true) {
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
    // for (size_t i = 0; i < B_.size(); ++i) {
      // size_t idx = B_[i];
      // if (idx < N_.size()) {
        // solution[idx] = b_[i];
      // }
    // }
    for (size_t i = 0; i < B_.size(); ++i) {
      solution[i] = GetValue(i);
    }

    DebugPrint();
    return true;
  }


  void DebugPrint(void) {
    cout << __func__ << endl;
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

void Test(void) {
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

int main(void) {
  Test();

  return 0;
}
