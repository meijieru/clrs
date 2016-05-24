#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <cstring>

using namespace std;

bool IsSuffix(string a, string b) {
  return a == b.substr(b.length() - a.length(), a.length());
}

void FiniteAutomationMatcher(string &t, map<pair<int, char>, int> &delta,
                             int m) {
  auto n = t.length();
  // status are [0, m]
  int q = 0;
  for (size_t i = 0; i < n; ++i) {
    q = delta[make_pair(q, t[i])];
    if (q == m) {
      cout << "Pattern occurs with shift: " << i - m + 1 << endl;
    }
  }
}

map<pair<int, char>, int> ComputeTransitionFunction(string &p,
                                                          vector<char> sigma) {
  auto m = p.length();
  map<pair<int, char>, int> delta;
  for (size_t q = 0; q < m + 1; ++q) {
    for (auto a: sigma) {
      for (int k = min(m, q + 1);; --k) {
        if (IsSuffix(p.substr(0, k), p.substr(0, q) + a)) {
          delta[make_pair(q, a)] = k;
          break;
        }
      }
    }
  }
  return delta;
}

vector<int> ComputePrefixFunction(string &p) {
  auto m = p.length();
  vector<int> pi(m);
  pi[0] = -1;
  int k = -1;
  // every iter has (k == pi[q - 1])
  for (size_t q = 1; q < m; ++q) {
    while ((k >= 0) && (p[k + 1] != p[q])) {
      k = pi[k - 1];
    }
    if (p[k + 1] == p[q]) {
      ++k;
    }
    pi[q] = k;
  }
  return pi;
}

void KmpMatcher(string &t, string &p) {
  int n = t.length();
  int m = p.length();
  auto pi = ComputePrefixFunction(p);
  int q = -1;
  // status are [-1, m - 1]
  for (int i = 0; i < n; ++i) {
    while ((q >= 0) && (p[q + 1] != t[i])) {
      q = pi[q];
    }
    if (p[q + 1] == t[i]) {
      ++q;
    }
    if (q == m - 1) {
      cout << "Pattern occurs with shift: " << i - m + 1 << endl;
    }
  }
}

int main(void) {
  string t("abababacaba");
  string p("ababaca");

  vector<char> char_set;
  for (char c = 'a'; c < 'c' + 1; ++c) {
    char_set.push_back(c);
  }
  auto delta = ComputeTransitionFunction(p, char_set);
  // for (auto item : delta) {
    // cout << item.first.first << " " << item.first.second << " " << item.second << endl;
  // }
  FiniteAutomationMatcher(t, delta, p.length());

  // for (auto k : ComputePrefixFunction(p)) {
    // cout << k << " ";
  // }
  // cout << endl;
  KmpMatcher(t, p);

  return 0;
}
