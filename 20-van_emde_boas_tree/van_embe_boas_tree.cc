#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class vEBTree {
 public:
  struct node {
    size_t u;
    int min;
    int max;
    node *summary;
    vector<node *> cluster;

    void build(size_t bitsize) {
      max = min = -1;
      // cout << "bit " << bitsize << endl;
      if (bitsize <= 1) {
        u = 2;
        summary = nullptr;
        // cout << "here" << endl;
      } else {
        // modify
        u = 1 << bitsize;
        int new_size = bitsize / 2;
        summary = new node;
        // cout << (1 << new_size) << " new" << endl;
        summary->build(new_size);
        cluster.resize(1 << new_size);

        // cout << cluster.size() << " cluster sz" << endl;
        for (auto &a : cluster) {
          a = new node;
          a->build(new_size);
          // cout << "enter" << endl;
        }
        // cout << endl;
      }
      // cout << u << endl;
    }
  };

  inline int High(size_t u, int x) {
    int low_sqrt = sqrt(u);
    return x / low_sqrt;
  }
  inline int Low(size_t u, int x) {
    int low_sqrt = sqrt(u);
    return x % low_sqrt;
  }
  inline int Index(size_t u, int x, int y) {
    int low_sqrt = sqrt(u);
    return x * low_sqrt + y;
  }

  vEBTree(size_t n) {
    int length = 0;
    while (n) {
      n >>= 1;
      ++length;
    }
    root_ = new node;
    root_->build(length - 1);
  }
  ~vEBTree(void) { Release(root_); }

  void Print() { Print(root_, 0); }
  void Print(node *p, int depth) {
    if (p->min == -1) return;
    for (int i = 0; i < depth; ++i) {
      cout << " ";
    }
    cout << "depth:" << depth << " min:" << p->min << " max" << p->max
         << " u:" << p->u;
    cout << endl;
    for (auto &a : p->cluster) {
      Print(a, depth + 1);
    }
  }
  void PrintSumm() { PrintSumm(root_->summary, 0); }
  void PrintSumm(node *p, int depth) {
    if (!p) return;
    for (int i = 0; i < depth; ++i) {
      cout << " ";
    }
    cout << p->min << " " << p->max << endl;
    for (auto a : p->cluster) {
      PrintSumm(a, depth + 1);
    }
  }

  void Release(node *p) {
    if (p) {
      delete p->summary;
    }
    for (auto a : p->cluster) {
      Release(a);
    }
  }

  int Min(node *p) { return p->min; }
  int Max(node *p) { return p->max; }

  int Member(node *p, int x) {
    if (x == p->min || x == p->max) {
      return 1;
    } else if (p->u == 2) {
      return 0;
    } else {
      return Member(p->cluster[High(p->u, x)], Low(p->u, x));
    }
  }

  int Successor(node *p, int x) {
    if (p->u == 2) {
      if (x == 0 && p->max == 1) {
        return 1;
      } else {
        return -1;
      }
    } else if (p->min != -1 && x < p->min) {
      return p->min;
    } else {
      int max_low = Max(p->cluster[High(p->u, x)]);
      if (max_low != -1 && Low(p->u, x) < max_low) {
        int offset = Successor(p->cluster[High(p->u, x)], Low(p->u, x));
        return Index(p->u, High(p->u, x), offset);
      } else {
        int succ_clus = Successor(p->summary, High(p->u, x));
        if (succ_clus == -1) {
          return -1;
        } else {
          int offset = Min(p->cluster[succ_clus]);
          return Index(p->u, succ_clus, offset);
        }
      }
    }
  }

  int Predecessor(node *p, int x) {
    if (p->u == 2) {
      if (x == 1 && p->min == 0) {
        return 0;
      } else {
        return -1;
      }
    } else if (p->max != -1 && x < p->max) {
      return p->max;
    } else {
      int min_low = Min(p->cluster[High(p->u, x)]);
      if (min_low != -1 && Low(p->u, x) > min_low) {
        int offset = Predecessor(p->cluster[High(p->u, x)], x);
        return Index(p->u, High(p->u, x), offset);
      } else {
        int prev_clus = Predecessor(p->summary, High(p->u, x));
        if (prev_clus == -1) {
          if (p->min != -1 && x > p->min) {
            return p->min;
          }
          return -1;
        } else {
          int offset = Max(p->cluster[prev_clus]);
          return Index(p->u, High(p->u, x), offset);
        }
      }
    }
  }

  void Insert(int x) { Insert(root_, x); }
  void Insert(node *p, int x) {
    if (p->min == -1) {
      EmptyInsert(p, x);
    } else {
      if (x < p->min) {
        int tmp = p->min;
        p->min = x;
        x = tmp;
      }

      if (p->u > 2) {
        if (Min(p->cluster[High(p->u, x)]) == -1) {
          Insert(p->summary, High(p->u, x));
          EmptyInsert(p->cluster[High(p->u, x)], Low(p->u, x));
        } else {
          Insert(p->cluster[High(p->u, x)], Low(p->u, x));
        }
      }

      if (x > p->max) {
        p->max = x;
      }
    }
  }

 private:
  void EmptyInsert(node *p, int x) {
    p->min = x;
    p->max = x;
  }

  node *root_;
};

int main() {
  vEBTree veb(16);
  vector<int> tb{2, 3, 4, 5, 7, 14, 15};
  // veb.Insert(1);
  // veb.Insert(2);
  for (size_t i = 0; i < tb.size(); ++i) {
    veb.Insert(tb[i]);
    // cout << "----------------------" << tb[i] <<endl;
    // veb.Print();
    // cout << "sum----------------------" << tb[i] <<endl;
    // veb.PrintSumm();
  }
  veb.Print();
}
