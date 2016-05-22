#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

int Mod(int a, int b) { return (a % b >= 0) ? (a % b) : (a % b + b); }

int Euclid(int a, int b) {
  if (b > a) {
    return Euclid(b, a);
  }
  return b == 0 ? a : Euclid(b, Mod(a, b));
}

int EuclidIter(int a, int b) {
  if (b > a) {
    std::swap(a, b);
  }
  while (true) {
    if (b == 0) {
      return a;
    } else {
      a = Mod(a, b);
      std::swap(a, b);
    }
  }
}

std::tuple<int, int, int> ExtendEuclid(int a, int b) {
  if (b == 0) {
    return std::make_tuple(a, 1, 0);
  }

  auto tmp = ExtendEuclid(b, a % b);
  return std::make_tuple(
      std::get<0>(tmp), std::get<2>(tmp),
      (std::get<1>(tmp) - std::get<2>(tmp) * static_cast<int>(a / b)));
}

bool ModularLinearEquationSolver(int a, int b, int n) {
  int d, x, y;
  std::tie(d, x, y) = ExtendEuclid(a, n);

  if (b % d == 0) {
    int x0 = Mod(x * (b / d), n);
    for (int i = 0; i < d; ++i) {
      std::cout << Mod(x0 + i * (n / d), n) << " ";
    }
    std::cout << std::endl;
    return true;
  }
  return false;
}

int ModularExp(int a, int b, int n) {
  int c = 0, d = 1;
  int k = 0;

  for (int i = 1;; ++i) {
    if (!(b >> i)) {
      k = i - 1;
      break;
    }
  }

  for (int i = k; i >= 0; --i) {
    c = 2 * c;
    d = Mod(d * d, n);
    if ((b >> i) & 1) {
      ++c;
      d = Mod(d * a, n);
    }
    // std::cout << ((b >> i) & 1) << " " << c << " " << d << std::endl;
  }
  return d;
}

bool PseudoPrime(int n) {
  if (Mod(ModularExp(2, n - 1, n), n) != 1) {
    return false;
  }
  return true;
}

// n should > 2
bool Witness(int a, int n) {
  int u = n - 1, t = 0;
  while ((u & 1) == 0) {
    ++t;
    u /= 2;
  }

  auto x_curr = 0, x_prev = ModularExp(a, u, n);
  for (int i = 1; i < t + 1; ++i) {
    x_curr = Mod(x_prev * x_prev, n);
    if ((x_curr == 1) && (x_prev != 1) && (x_prev != n - 1)) {
      return true;
    }
    x_prev = x_curr;
  }

  if (x_curr != 1) {
    return true;
  }
  return false;
}

bool MillerRobin(int n, int s) {
  for (int i = 0; i < s; ++i) {
    auto a = rand() % (n - 1) + 1;
    if (Witness(a, n)) {
      return false;
    }
  }
  return true;
}

void TestEuclid(void) {
  std::cout << "TestEuclid" << std::endl;

  std::cout << Euclid(99, 78) << std::endl;
  std::cout << Euclid(78, 99) << std::endl;

  std::cout << EuclidIter(99, 78) << std::endl;
  std::cout << EuclidIter(78, 99) << std::endl;

  auto x = ExtendEuclid(99, 78);
  std::cout << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x)
            << std::endl;
  std::cout << std::endl;
}

void TestModularLinearEquationSolver(void) {
  std::cout << "TestModularLinearEquationSolver" << std::endl;
  ModularLinearEquationSolver(14, 30, 100);
  std::cout << std::endl;
}

void TestModularExp(void) {
  std::cout << "TestModularExp" << std::endl;
  std::cout << ModularExp(7, 560, 561) << std::endl;
  std::cout << std::endl;
}

void TestPseudoPrime(void) {
  std::cout << "TestPseudoPrime" << std::endl;
  std::cout << "false positive:" << std::endl;
  std::cout << PseudoPrime(341) << " " << PseudoPrime(561)
      << " " << PseudoPrime(645) << std::endl;
  std::cout << "true positive:" << std::endl;
  std::cout << PseudoPrime(17) << " " << PseudoPrime(523)
      << " " << PseudoPrime(281) << std::endl;
  std::cout << "true negative:" << std::endl;
  std::cout << PseudoPrime(527) << " " << PseudoPrime(119)
      << " " << PseudoPrime(951) << std::endl;
  std::cout << std::endl;
}

void TestMillerRobin(void) {
  std::cout << "TestMillerRobin" << std::endl;
  Witness(7, 561);
  std::cout << MillerRobin(561, 10) << std::endl;
  std::cout << MillerRobin(17, 10) << " " << MillerRobin(523, 10)
      << " " << MillerRobin(281, 10) << std::endl;
  std::cout << std::endl;
}

int main(void) {
  TestEuclid();
  TestModularLinearEquationSolver();
  TestModularExp();
  TestPseudoPrime();
  TestMillerRobin();

  return 0;
}
