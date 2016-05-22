#include "lib.h"

#include <iostream>
#include <iterator>

using namespace std;

int main(void) {
  vector<float> _l = {1, 0, 0, 0.2, 1, 0, 0.6, 0.5, 1};
  vector<float> _u = {5, 6, 3, 0, 0.8, -0.6, 0, 0, 2.5};
  vector<float> p = {2, 0, 1};
  vector<float> b = {3, 7, 8};
  Matrix<float> l(Dimension(3, 3));
  Matrix<float> u(Dimension(3, 3));
  l.Fill(_l);
  u.Fill(_u);

  l.Display();
  cout << "-----------" << endl;
  u.Display();
  cout << "-----------" << endl;

  auto x = LUPSolve(l, u, p, b);

  ostream_iterator<float> out_iter(cout, " ");

  copy(x.begin(), x.end(), out_iter);
  cout << endl;
  cout << "-----------" << endl;

  vector<float> _a = {
      2, 3, 1, 5, 6, 13, 5, 19, 2, 19, 10, 23, 4, 10, 11, 31,
  };
  Matrix<float> a(Dimension(4, 4));
  Matrix<float> l1(Dimension(4, 4));
  Matrix<float> u1(Dimension(4, 4));
  a.Fill(_a);

  LUDecomposition(a, l1, u1);
  l1.Display();
  cout << "-----------" << endl;
  u1.Display();
  cout << "-----------" << endl;

  vector<float> _a2 = {
    2, 0, 2, 0.6, 3, 3, 4, -2, 5, 5, 4, 2, -1, -2, 3.4, -1,
  };
  Matrix<float> a2(Dimension(4, 4));
  Matrix<float> l2(Dimension(4, 4));
  Matrix<float> u2(Dimension(4, 4));
  a2.Fill(_a2);
  vector<size_t> pi2;

  LUPDecomposition(a2, l2, u2, pi2);
  l2.Display();
  cout << "-----------" << endl;
  u2.Display();
  cout << "-----------" << endl;
  copy(pi2.begin(), pi2.end(), out_iter);
  cout << endl;

  return 0;
}
