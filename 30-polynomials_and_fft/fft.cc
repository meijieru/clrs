#include <complex>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename T>
void AuxDisplay(const T &x, string prefix = "", string split = " ") {
  cout << prefix;
  for (auto &item : x) {
    cout << item << split;
  }
  cout << endl;
  cout << "---------------------------" << endl;
}

using element = complex<float>;

vector<element> RecursiveFFTImpl(vector<element> &a, bool is_inverse) {
  auto n = a.size();
  if (n == 1) {
    return a;
  }

  vector<element> a0(n / 2), a1(n / 2);
  for (size_t i = 0; i < n / 2; ++i) {
    a0[i] = a[2 * i];
    a1[i] = a[2 * i + 1];
  }

  auto y0 = RecursiveFFTImpl(a0, is_inverse);
  auto y1 = RecursiveFFTImpl(a1, is_inverse);
  vector<element> y(n);

  element w_n(0.0, M_PI * 2 / n);
  if (is_inverse) {
    w_n = -w_n;
  }
  w_n = exp(w_n);
  element w = 1;

  for (size_t i = 0; i < n / 2; ++i) {
    y[i] = y0[i] + w * y1[i];
    y[i + n / 2] = y0[i] - w * y1[i];
    w *= w_n;
  }
  return y;
}

vector<element> RecursiveFFT(vector<element> &a) {
  return RecursiveFFTImpl(a, false);
}

vector<element> InverseDFT(vector<element> &a) {
  auto n = a.size();
  auto y = RecursiveFFTImpl(a, true);
  transform(y.begin(), y.end(), y.begin(), [&](element x) {
    if (abs(x.imag()) < 10e-5) {
      x = element(x.real(), 0);
    }
    return x / element(n);
  });
  return y;
}

vector<element> MulPolyNaive(const vector<element> &lhs,
                             const vector<element> &rhs) {
  vector<element> result(lhs.size() + rhs.size() - 1);
  for (size_t i = 0; i < lhs.size(); ++i) {
    for (size_t j = 0; j < rhs.size(); ++j) {
      result[i + j] += lhs[i] * rhs[j];
    }
  }
  return result;
}

vector<element> MulCorrespondPoints(const vector<element> &lhs,
                                    const vector<element> &rhs) {
  vector<element> result(lhs.size());
  for (size_t i = 0; i < lhs.size(); ++i) {
    result[i] = lhs[i] * rhs[i];
  }
  return result;
}

vector<element> MulPolyPoints(const vector<element> &lhs,
                              const vector<element> &rhs) {
  size_t size = lhs.size() * 2;
  vector<element> expand_lhs(size), expand_rhs(size);
  copy(lhs.begin(), lhs.end(), expand_lhs.begin());
  copy(rhs.begin(), rhs.end(), expand_rhs.begin());
  auto fft_lhs = RecursiveFFT(expand_lhs);
  auto fft_rhs = RecursiveFFT(expand_rhs);
  auto fft_result = MulCorrespondPoints(fft_lhs, fft_rhs);
  auto result = InverseDFT(fft_result);
  result.pop_back();
  return result;
}

size_t Rev(size_t idx, size_t s) {
  size_t result = 0;
  for (size_t i = 0; i < s; ++i) {
    result |= ((idx >> i) & 1) << (s - i - 1);
  }
  return result;
}

vector<element> BitReverseCopy(const vector<element> &a) {
  auto n = a.size();
  size_t s = 0;

  auto _n = n - 1;
  while (_n > 0) {
    _n >>= 1;
    ++s;
  }

  vector<size_t> index(a.size());
  for (size_t i = 0; i < n; ++i) {
    index[i] = Rev(i, s);
  }

  vector<element> result(a.size());
  for (size_t i = 0; i < n; ++i) {
    result[i] = a[index[i]];
  }
  return result;
}

vector<element> IterativeFFT(const vector<element> &a) {
  auto _a = BitReverseCopy(a);
  auto n = a.size();

  for (size_t s = 0; s <= log2(n); ++s) {
    size_t m = 1 << s;
    element w_m(0, M_PI * 2 / m);
    w_m = exp(w_m);
    for (size_t k = 0; k < n; k += m) {
      element w = 1;
      for (size_t j = 0; j < m / 2; ++j) {
        auto t = w * _a[k + j + m / 2];
        auto u = _a[k + j];
        _a[k + j] = u + t;
        _a[k + j + m / 2] = u - t;
        w *= w_m;
      }
    }
  }

  return _a;
}

void TestFFT(void) {
  vector<element> p1{{1.0, 0}, {2.0, 0}, {3.0, 0}, {4.0, 0}};
  vector<element> p2{{1.0, 0}, {2.0, 0}, {3.0, 0}, {4.0, 0}};
  auto p1_point = RecursiveFFT(p1);
  AuxDisplay(p1_point);
  AuxDisplay(IterativeFFT(p1));
  AuxDisplay(InverseDFT(p1_point));
  AuxDisplay(MulCorrespondPoints(p1, p2));
  AuxDisplay(MulPolyNaive(p1, p2));
  AuxDisplay(MulPolyPoints(p1, p2));
}

int main(void) {
  TestFFT();

  return 0;
}
