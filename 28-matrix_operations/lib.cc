#include "lib.h"

#include <cassert>
#include <limits>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

size_t Dimension::GetDimN(int n) {
  if (n < 0) {
    n = _dims.size() + n;
  }
  assert(n < static_cast<int>(_dims.size()));
  return _dims[n];
}

template <typename T>
T &MatrixImplement<T>::GetIdx(size_t idx) {
  assert(idx < _sz);
  return _in_storage[idx];
}

template class MatrixImplement<float>;
