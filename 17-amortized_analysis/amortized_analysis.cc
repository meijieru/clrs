#include <climits>
#include <cstring>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class Table {
 public:
  Table() : num_(0), size_(0), ptr_(0) {}
  ~Table() {
    if (size_) {
      if (size_ == 1) {
        delete ptr_;
      } else {
        delete[] ptr_;
      }
    }
    ptr_ = 0;
  }

  size_t Size(void) { return size_; }
  size_t Num(void) { return num_; }

  void Insert(int x) {
    if (!size_) {
      ptr_ = new int;
      size_ = 1;
    }

    if (num_ == size_) {
      int *inserted = new int[2 * size_];
      for (size_t i = 0; i < size_; ++i) {
        inserted[i] = ptr_[i];
      }
      delete[] ptr_;
      ptr_ = inserted;
      size_ *= 2;
    }

    ptr_[num_] = x;
    ++num_;
  }

  void Delete() {
    if (size_ == 1) {
      delete ptr_;
      ptr_ = 0;
      size_ = 0;
    }

    --num_;
    if (4 * num_ / size_ < 1) {
      int *inserted = new int[size_ / 2];
      for (size_t i = 0; i < num_; ++i) {
        inserted[i] = ptr_[i];
      }
      delete[] ptr_;
      ptr_ = inserted;
      size_ /= 2;
    }
  }

 private:
  size_t num_;
  size_t size_;
  int *ptr_;
};

class DynamicBinarySearch {
 public:
  DynamicBinarySearch(size_t n) : size_(log2(n + 1)) {
    ptr_avai_ = new int[size_];
    array_ptr_ = new int *[size_];
    ptr_used_ = new int[size_];
    for (int i = 0; i < size_; ++i) {
      ptr_avai_[i] = 0;
      array_ptr_[i] = 0;
      ptr_used_[i] = 0;
    }
  }

  ~DynamicBinarySearch(void) {
    for (int i = 0; i < size_; ++i) {
      if (ptr_avai_[i]) {
        delete[] array_ptr_[i];
      }
    }
    delete[] ptr_avai_;
    delete[] array_ptr_;
    delete[] ptr_used_;
  }

  void ExtractMin(int k, int x, int *dst) {
    if (!k) {
      dst[0] = x;
      return;
    }

    int *p = new int[k];
    memset(p, 0, k * sizeof(int));
    for (int j = 0; j < pow(2, k); ++j) {
      int min = x;
      int pt = INT_MAX;
      for (int i = 0; i < k; ++i) {
        if (p[i] < pow(2, i)) {
          if (min > array_ptr_[i][p[i]]) {
            min = array_ptr_[i][p[i]];
            pt = i;
          }
        }
      }
      if (pt != INT_MAX) {
        ++p[pt];
      }
      dst[j] = min;
      cout << min << " min" << endl;
      for (int i = 0; i < k; ++i) {
        cout << p[i] << " ";
      }
      cout << "p" << endl;
    }
    delete[] p;
  }

  void Insert(int x) {
    int i = 0;
    while (ptr_used_[i] == 1) {
      ptr_used_[i] = 0;
      ++i;
    }
    if (!ptr_avai_[i]) {
      int s = pow(2, i);
      array_ptr_[i] = new int[s];
      ptr_avai_[i] = 1;
    }
    ExtractMin(i, x, array_ptr_[i]);
    ptr_used_[i] = 1;
  }

  void Search(int x, int **dst, int *index) {
    for (int i = 0; i < size_ - 1; ++i) {
      if (ptr_avai_[i] == 1) {
        for (int j = 0; j < pow(2, i); ++j) {
          if (array_ptr_[i][j] == x) {
            *dst = array_ptr_[i];
            *index = j;
            return;
          }
        }
      }
    }
    *dst = 0;
    *index = INT_MAX;
  }

  // void Delete(int x) {
  // int j = 0;
  // while(ptr_avai_[j] == 0) {
  // ++j;
  // }
  // int index;
  // int *p;
  // Search(x,&p,&index);

  // if (!p) {
  // return ;
  // }

  // int size_j = pow(2,j);
  // int size_p = pow(2,index);
  // int tmp = array_ptr_[j][size_j-1];

  // for (int i = index; i < size_p - 1; ++i) {
  // p[i] = p[i+1];
  // }
  // p[size_p-1] = INT_MAX;
  // InsertToLine(p,size_p,tmp);

  // }

  void Print(void) {
    for (int i = 0; i < size_; ++i) {
      if (ptr_used_[i]) {
        cout << "column " << i << " ";
        cout << "size:" << pow(2, i) << endl;
        for (int j = 0; j < pow(2, i); ++j) {
          cout << array_ptr_[i][j] << " ";
        }
        cout << endl;
      }
    }
  }

  void Debug(void) {
    for (int i = 0; i < size_; ++i) {
      cout << "column" << i << " ";
      cout << ptr_avai_[i] << " " << ptr_used_[i] << endl;
    }
  }

  int Size(void) { return size_; }

 private:
  // void InsertToLine(int *p,int size,int x) {
  // int left = 0;
  // int right = size - 1;
  // int mid = size / 2;
  // int index = INT_MAX;
  // while(left <= right) {
  // if (p[mid] == x) {
  // index = mid;
  // } else if (p[mid] < x) {
  // right = mid - 1;
  // mid = (mid + left) / 2;
  // } else {
  // left = mid + 1;
  // mid = (mid + right) / 2;
  // }
  // }
  // index = left;

  // int tmp = p[index];
  // p[index] = x;
  // for (int i = index + 1; i < size; ++i) {
  // p[i] = tmp;
  // tmp = p[i+1];
  // }
  // }

  int size_;
  int *ptr_avai_;
  int *ptr_used_;
  int **array_ptr_;
};

int main() {
  // Table tb;
  // for (int i = 0; i < 5; ++i) {
  // tb.Insert(i);
  // cout << tb.Size() << " " << tb.Num() << endl;
  // }
  // for (int i = 0; i < 5; ++i) {
  // tb.Delete();
  // cout << tb.Size() << " " << tb.Num() << endl;
  // }

  DynamicBinarySearch dy(7);
  for (int i = 0; i < 7; ++i) {
    dy.Insert(i + 1);
    // dy.Print();
    // dy.Debug();
    // cout << endl;
  }
  dy.Print();
}
