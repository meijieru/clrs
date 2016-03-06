#ifndef __LIB_H
#define __LIB_H

#include <cassert>
#include <limits>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>

class Dimension {
 public:
  Dimension() = default;
  Dimension(size_t n_row, size_t n_col) {
    _dims.push_back(n_row);
    _dims.push_back(n_col);
  }
  Dimension(std::vector<size_t> &dims) : _dims(dims) {}

  size_t GetDimN(int n);
  size_t Size(void) { return _dims.size(); }
  size_t NElement(void) {
    return accumulate(_dims.cbegin(), _dims.cend(), 1,
                      [](size_t x, size_t y) { return x * y; });
  }

 private:
  std::vector<size_t> _dims;
}; /* class: Dimension */

class RefRec {
 public:
  RefRec(size_t stride = 1, size_t start = 0)
      : _start(start), _stride(stride) {}

  bool IsContinuous(void) { return _stride == 1; }
  size_t NextIdx(int prev = -1) { return prev != -1 ? prev + _stride : _start; }
  size_t Stride(void) { return _stride; }

 private:
  size_t _start;
  size_t _stride;
}; /* class: RefRec */

template <typename T>
class MatrixImplement {
 public:
  MatrixImplement(size_t num_alloc) {
    _sz = num_alloc;
    Alloc(num_alloc);
  }

  ~MatrixImplement(void) { delete[] _in_storage; }

  // T &GetIdx(size_t idx) {
  // assert(idx < _sz);
  // return _in_storage[idx];
  // }
  T &GetIdx(size_t idx);
  size_t Size(void) { return _sz; }

 private:
  void Alloc(size_t sz) { _in_storage = new T[sz]; }

  int Lock(void) { return 0; }
  void Unlock(void) {}

 private:
  T *_in_storage;
  size_t _sz;
}; /* class: MatrixImplement */

extern template class MatrixImplement<float>;

// support 2d tensor temporarily
template <typename T>
class Matrix {
 public:
  typedef T value_type;
  typedef value_type &reference_type;

 public:
  Matrix() {}
  Matrix(const Dimension &dim) : _dim(dim) {}

  Matrix(std::vector<T> &diag) {}

  size_t GetDimN(int n) { return _dim.GetDimN(n); }
  size_t GetNDim(void) { return _dim.Size(); }

  // TODO : multi dim support
  void Display(void) {
    AssureAlloced();

    auto n_cols = NCol();
    auto n_rows = NRow();

    auto idx = NextIdx();
    for (size_t i = 0; i < n_rows; ++i) {
      for (size_t j = 0; j < n_cols; ++j) {
        std::cout << GetIdx(idx) << "\t";
        idx = NextIdx(idx);
      }
      std::cout << std::endl;
    }
  }

  Matrix<T> Dot(Matrix<T> &m) {
    auto n_col1 = NCol();
    auto n_row1 = NRow();
    auto n_col2 = m.NCol();
    auto n_row2 = m.NRow();

    assert(n_col1 == n_row2);
    Matrix<T> result(Dimension(n_row1, n_col2));

    for (size_t i = 0; i < n_row1; ++i) {
      for (size_t j = 0; j < n_col1; ++j) {
        for (size_t k = 0; k < n_col2; ++k) {
          result.GetIdx(i * result.NCol() + k) +=
              GetIdx(i * n_col1 + j) * m.GetIdx(j * n_col2 + k);
        }
      }
    }
    return result;
  }

  void Fill(T &x) {
    AssureAlloced();

    auto idx = NextIdx();
    for (size_t i = 0; i < _in_storage->Size(); ++i) {
      GetIdx(idx) = x;
      idx = NextIdx(idx);
    }
  }

  void Fill(const std::vector<T> &content) {
    assert(content.size() == _dim.NElement());

    AssureAlloced();

    auto idx = NextIdx();
    for (size_t i = 0; i < content.size(); ++i) {
      // std::cout << i << std::endl;
      GetIdx(idx) = content[i];
      // std::cout << GetIdx(idx) << std::endl;
      idx = NextIdx(idx);
    }
  }

  size_t Stride(void) { return _loc_rec.Stride(); }

  Matrix<T> Transpose(void) {
    AssureAlloced();

    auto n_col = NCol();
    auto n_row = NRow();

    Matrix<T> result(Dimension(n_col, n_row));

    auto idx = NextIdx();
    for (size_t i = 0; i < n_row; ++i) {
      for (size_t j = 0; j < n_col; ++j) {
        result.GetIdx(j * n_row + i) = GetIdx(idx);
        idx = NextIdx(idx);
      }
    }

    return result;
  }

  // manually
  T &GetIdx(size_t idx) {
    AssureAlloced();
    return _in_storage->GetIdx(idx);
  }
  size_t NextIdx(int idx = -1) { return _loc_rec.NextIdx(idx); }
  T &operator [](size_t idx) {
    return GetIdx(idx);
  }

 private:
  Dimension _dim;
  std::shared_ptr<MatrixImplement<T>> _in_storage;
  RefRec _loc_rec;

 private:
  void AssureAlloced(void) {
    if (!_in_storage) {
      assert(_dim.NElement());
      _in_storage.reset(new MatrixImplement<T>(_dim.NElement()));
    }
  }

 public:
  // for matrix only
  size_t NCol(void) {
    assert(GetNDim() == 2);
    return GetDimN(1);
  }

  size_t NRow(void) {
    assert(GetNDim() == 2);
    return GetDimN(0);
  }
}; /* class: Matrix */

template <typename T>
Matrix<T> Diagnal(std::vector<T> &diag) {
  auto _dim = Dimension(diag.size(), diag.size());
  Matrix<T> m(_dim);

  for (size_t i = 0; i < diag.size(); ++i) {
    m.GetIdx(i * diag.size() + i) = diag[i];
  }
  return std::move(m);
}

template <typename T = float>
Matrix<T> Eye(size_t n) {
  assert(n > 0);
  std::vector<T> v(n, 1);
  return Diagnal(v);
}

template <typename T>
std::vector<T> LUPSolve(Matrix<T> &l, Matrix<T> &u, std::vector<T> &pi,
                        std::vector<T> &b) {
  auto n = l.NRow();
  std::vector<T> x(n), y(n);

  for (size_t i = 0; i < n; ++i) {
    y[i] = b[pi[i]];
    for (size_t j = 0; j < i; ++j) {
      y[i] -= l.GetIdx(i * n + j) * y[j];
    }
  }

  for (size_t idx = 0; idx < n; ++idx) {
    auto i = n - idx - 1;
    x[i] = y[i];
    for (size_t j = i + 1; j < n; ++j) {
      x[i] -= u.GetIdx(i * n + j) * x[j];
    }
    x[i] /= u.GetIdx(i * n + i);
  }

  return x;
}

template <typename T>
void LUDecomposition(Matrix<T> a, Matrix<T> &l, Matrix<T> &u) {
  assert(a.NCol() == a.NRow());
  auto n = a.NCol();

  for (size_t i = 0; i < n; ++i) {
    l.GetIdx(i * n + i) = 1;
    u.GetIdx(i * n + i) = 0;
  }

  for (size_t i = 0; i < n; ++i) {
    u.GetIdx(i * n + i) = a.GetIdx(i * n + i);
    for (size_t j = i + 1; j < n; ++j) {
      l.GetIdx(j * n + i) = a.GetIdx(j * n + i) / u.GetIdx(i * n + i);
      u.GetIdx(i * n + j) = a.GetIdx(i * n + j);
    }

    for (size_t j = i + 1; j < n; ++j) {
      for (size_t k = i + 1; k < n; ++k) {
        a.GetIdx(j * n + k) -= l.GetIdx(j * n + i) * u.GetIdx(i * n + k);
      }
    }
  }
}

template <typename T>
void LUPDecomposition(Matrix<T> a, Matrix<T> &l, Matrix<T> &u,
                      std::vector<size_t> &pi) {
  assert(a.NCol() == a.NRow());
  auto n = a.NCol();
  pi.resize(n);

  using std::swap;

  for (size_t i = 0; i < n; ++i) {
    pi[i] = i;
  }
  for (size_t k = 0; k < n; ++k) {
    T p = 0;
    size_t _k;
    for (size_t i = k; i < n; ++i) {
      if (abs(a.GetIdx(i * n + k)) > p) {
        p = abs(a.GetIdx(i * n + k));
        _k = i;
      }
    }
    if (p == 0) {
      std::cout << "singular matrix" << std::endl;
    }
    swap(pi[k], pi[_k]);
    for (size_t i = 0; i < n; ++i) {
      swap(a.GetIdx(k * n + i), a.GetIdx(_k * n + i));
    }
    for (size_t i = k + 1; i < n; ++i) {
      a.GetIdx(i * n + k) /= a.GetIdx(k * n + k);
      for (size_t j = k + 1; j < n; ++j) {
        a.GetIdx(i * n + j) -= a.GetIdx(i * n + k) * a.GetIdx(k * n + j);
      }
    }
  }

  for (size_t i = 0; i < n; ++i) {
    l.GetIdx(i * n + i) = 1;
  }
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i; j < n; ++j) {
      u.GetIdx(i * n + j) = a.GetIdx(i * n + j);
    }
  }
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      l.GetIdx(j * n + i) = a.GetIdx(j * n + i);
    }
  }
}

#endif /* __LIB_H */
