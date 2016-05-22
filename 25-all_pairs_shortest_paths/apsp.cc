#include <climits>
#include <cstdlib>

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

using ll = long long int;

struct Edge {
  size_t u, v;
  int w;
};

ll *ConstructGraph(vector<Edge> &edges, size_t sz) {
  auto p = new ll[sz * sz];

  fill_n(p, sz * sz, INT_MAX);
  for (size_t i = 0; i < sz; ++i) {
    p[i * sz + i] = 0;
  }
  for (auto &edge : edges) {
    p[edge.u * sz + edge.v] = edge.w;
  }

  return p;
}

void *InitParent(vector<Edge> &edges, ll *parent, size_t sz) {
  fill_n(parent, sz * sz, INT_MAX - 1);
  for (auto &edge : edges) {
    parent[edge.u * sz + edge.v] = edge.u;
  }
}

void PrintPath(ll *parent, size_t sz, size_t u, size_t v) {
  if (u == v) {
    cout << u << endl;
  } else if (parent[u * sz + v] == -1) {
    cout << "no path from " << u << " to " << v << endl;
  } else {
    PrintPath(parent, sz, u, parent[u * sz + v]);
    cout << " " << v << endl;
  }
}

void PrintMatrix(ll *p, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < sz; ++j) {
      if (p[i * sz + j] != INT_MAX && p[i * sz + j] != INT_MAX - 1) {
        cout << p[i * sz + j] << " ";
      } else {
        cout << "m ";
      }
    }
    cout << endl;
  }
}

ll *ExtendShortestPath(const ll *l, size_t sz,const ll *w, ll *parent, bool &modified) {
  auto p = new ll[sz * sz];

  modified = false;
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < sz; ++j) {
      p[i * sz + j] = l[i * sz + j];
      for (size_t k = 0; k < sz; ++k) {
        auto prev = p[i * sz + j];
        auto next = l[i * sz + k] + w[k * sz + j];
        bool new_min = false;
        p[i * sz + j] =
            (prev <= next) ? prev : (modified |= true, new_min = true, next);
        if (new_min && parent) {
          parent[i * sz + j] = k;
        }
      }
    }
  }

  return p;
}

ll *SlowAllPairsShortestPath(const ll *w, ll *parent, size_t sz) {
  vector<ll *> ls;
  bool modified = true;

  auto l = new ll[sz * sz];
  copy_n(w, sz * sz, l);
  ls.push_back(l);
  while (modified) {
    auto last_l = ls.back();
    ls.push_back(ExtendShortestPath(last_l, sz, w, parent, modified));
  }

  for (size_t i = 0; i < ls.size() - 1; ++i) {
    delete[] ls[i];
    ls[i] = nullptr;
  }

  return ls.back();
}

ll *FloydWarshall(ll *w, ll *parent, size_t sz) {
  ll *d = new ll[sz * sz];
  copy_n(w, sz * sz, d);

  for (size_t k = 0; k < sz; ++k) {
    for (size_t i = 0; i < sz; ++i) {
      for (size_t j = 0; j < sz; ++j) {
        d[i * sz + j] = min(d[i * sz + j], d[i * sz + k] + d[k * sz + j]);
      }
    }
  }

  return d;
}

int main(void) {
  vector<Edge> matrix_mul_edges = {
      {0, 1, 3}, {0, 2, 8},  {0, 4, -4}, {1, 3, 1}, {1, 4, 7},
      {2, 1, 4}, {3, 2, -5}, {3, 0, 2},  {4, 3, 6},
  };
  auto matrix_mul_sz = 5;
  auto count = matrix_mul_sz * matrix_mul_sz;

  auto matrix_mul_graph = ConstructGraph(matrix_mul_edges, matrix_mul_sz);
  auto parent = new ll[count];
  InitParent(matrix_mul_edges, parent, matrix_mul_sz);

  auto result =
      SlowAllPairsShortestPath(matrix_mul_graph, parent, matrix_mul_sz);
  PrintMatrix(result, matrix_mul_sz);
  cout << "--------------" << endl;

  delete[] result;

  InitParent(matrix_mul_edges, parent, matrix_mul_sz);
  result = FloydWarshall(matrix_mul_graph, parent, matrix_mul_sz);
  PrintMatrix(result, matrix_mul_sz);

  delete[] matrix_mul_graph;
  delete[] parent;
  delete[] result;

  return 0;
}
