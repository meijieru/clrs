#include <climits>
#include <cassert>

#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <queue>

using namespace std;

struct Edge {
  Edge(int u = 0, int v = 0, int w = 0) : u(u), v(v), w(w) {}

  int u, v, w;
};

struct Node {
  Node(int idx = 0, int d = INT_MAX, int p = -1) : idx(idx), d(d), p(p) {}

  int idx, d, p;
  list<pair<int, int>> adjs;
};

bool Relax(Node *u, Node *v, int w) {
  // otherwise may overflow
  if (v->d - w > u->d) {
    v->d = u->d + w;
    v->p = u->idx;
    return true;
  }
  return false;
}

void InitNodes(vector<Node> &nodes, size_t s) {
  for (auto &node : nodes) {
    node.p = -1;
    node.d = INT_MAX;
  }
  nodes[s].d = 0;
}

vector<Node> ConstructGraph(const vector<Edge> &edges, size_t sz) {
  vector<Node> nodes(sz);
  for (size_t i = 0; i < sz; ++i) {
    nodes[i].idx = i;
  }
  for (auto &e : edges) {
    nodes[e.u].adjs.push_back({e.v, e.w});
  }

  return nodes;
}

void PrintGraph(const vector<Node> &nodes) {
  for (auto &u : nodes) {
    for (auto &edge : u.adjs) {
      cout << u.idx << "->" << edge.first << " " << edge.second << endl;
    }
  }
}

bool BellmanFord(vector<Node> &nodes, vector<Edge> &edges, size_t s) {
  InitNodes(nodes, s);

  for (size_t i = 0; i < edges.size(); ++i) {
    bool modifid = false;
    for (auto &e : edges) {
      modifid |= Relax(&nodes[e.u], &nodes[e.v], e.w);
    }
    if (!modifid) {
      break;
    }
  }

  for (auto &e : edges) {
    if (nodes[e.v].d > nodes[e.u].d + e.w) {
      return false;
    }
  }
  return true;
}

void TopoVisit(vector<Node> &nodes, Node *pu, vector<Node *> &sorted_reverse) {
  pu->d = -1;

  for (auto &edge : pu->adjs) {
    if (nodes[edge.first].d != -1) {
      TopoVisit(nodes, &nodes[edge.first], sorted_reverse);
    }
  }
  sorted_reverse.push_back(pu);
}

vector<Node *> TopoSort(vector<Node> &nodes) {
  vector<Node *> sorted_reverse;

  for (auto &node : nodes) {
    node.d = INT_MAX;
  }
  for (auto &node : nodes) {
    if (node.d == INT_MAX) {
      TopoVisit(nodes, &node, sorted_reverse);
    }
  }

  vector<Node *> sorted(sorted_reverse.size());
  copy(sorted_reverse.rbegin(), sorted_reverse.rend(), sorted.begin());
  return sorted;
}

void DagShortestPaths(vector<Node> &nodes, size_t s) {
  auto topo_sorted = TopoSort(nodes);

  InitNodes(nodes, s);

  for (auto &pu : topo_sorted) {
    for (auto &edge : pu->adjs) {
      Relax(pu, &nodes[edge.first], edge.second);
    }
  }
}

template <class T, class PositionSetter, class Compare = less<T>>
class Heap {
 public:
  Heap(const PositionSetter &set_pos = PositionSetter(),
       const Compare &cmp = Compare())
      : _set_pos(set_pos), _cmp(cmp) {
    _data.push_back(T());
    // CorrectPosition(0);
  }

  bool Empty() const { return 1 == _data.size(); }

  const T &Top() const {
    assert(!Empty());
    return _data[1];
  }

  T Pop() {
    assert(!Empty());

    swap(_data[1], _data.back());
    CorrectPosition(1);

    T temp = _data.back();
    _data.pop_back();

    MaxHeapify(1);

    return move(temp);
  }

  void Push(const T &key) {
    _data.push_back(key);
    Float(_data.size() - 1);
  }

  void Push(T &&key) {
    _data.push_back(key);
    Float(_data.size() - 1);
  }

  void IncreaseKey(size_t pos, const T &key) {
    if (_cmp(_data[pos], key)) {
      _data[pos] = key;
      Float(pos);
    }
  }

  void IncreaseKey(size_t pos, function<void(T &key)> change) {
    change(_data[pos]);
    Float(pos);
  }

  void Float(size_t pos) {
    while (parent(pos) > 0 && _cmp(_data[parent(pos)], _data[pos])) {
      swap(_data[parent(pos)], _data[pos]);
      CorrectPosition(pos);
      pos = parent(pos);
    }
    CorrectPosition(pos);
  }

  vector<T> &DebugGetData() { return _data; }

 private:
  vector<T> _data;
  const PositionSetter _set_pos;
  const Compare _cmp;

  void MaxHeapify(const size_t pos) {
    size_t left = left_child(pos);
    size_t right = right_child(pos);

    size_t maxpos{pos};
    if (0 < left && left < _data.size() && _cmp(_data[maxpos], _data[left])) {
      maxpos = left;
    }
    if (0 < right && right < _data.size() &&
        _cmp(_data[maxpos], _data[right])) {
      maxpos = right;
    }
    if (maxpos != pos) {
      swap(_data[pos], _data[maxpos]);
      MaxHeapify(maxpos);
    }
  }

  void CorrectPosition(const size_t pos) { _set_pos(_data[pos], pos); }

  static size_t left_child(size_t x) { return x << 1; }
  static size_t right_child(size_t x) { return (x << 1) + 1; }
  static size_t parent(size_t x) { return x >> 1; }
};

void Dijkstra(vector<Node> &nodes, int s) {
  vector<int> heap_pos(nodes.size());
  auto set_pos = [&](Node *x, size_t pos) { heap_pos[x->idx] = pos; };

  auto cmp = [](Node *x, Node *y) { return x->d > y->d; };

  Heap<Node *, function<void(Node *, size_t)>,
       function<bool(Node * x, Node * y)>> heap(set_pos, cmp);

  InitNodes(nodes, s);

  for (auto &node : nodes) {
    heap.Push(&node);
  }

  while (!heap.Empty()) {
    Node *pu = heap.Pop();
    for (auto &edge : pu->adjs) {
      Relax(pu, &nodes[edge.first], edge.second);
      heap.Float(heap_pos[nodes[edge.first].idx]);
    }
  }
}

int *Johnson(vector<Node> &nodes, vector<Edge> edges) {
  Node s(nodes.size());

  for (auto &node : nodes) {
    s.adjs.push_back({node.idx, 0});
    edges.push_back({s.idx, node.idx, 0});
  }
  nodes.push_back(s);

  if (!BellmanFord(nodes, edges, s.idx)) {
    cout << "exist negative loop" << endl;
    return nullptr;
  }
  nodes.pop_back();
  for (int i = 0; i < s.idx; ++i) {
    edges.pop_back();
  }

  vector<Node> johnson_nodes(nodes.size());
  copy(nodes.cbegin(), nodes.cend(), johnson_nodes.begin());
  for (auto &node : johnson_nodes) {
    for (auto &edge : node.adjs) {
      edge.second = edge.second + nodes[node.idx].d - nodes[edge.first].d;
    }
  }

  int *d = new int[nodes.size() * nodes.size()];
  for (auto &u : nodes) {
    Dijkstra(johnson_nodes, u.idx);
    for (auto &v : johnson_nodes) {
      d[u.idx * nodes.size() + v.idx] = v.d + nodes[v.idx].d - nodes[u.idx].d;
    }
  }

  return d;
}

int main(void) {
  vector<Edge> edges = {
      {0, 1, 6},  {0, 3, 7},  {1, 2, 5}, {1, 3, 8}, {1, 4, -4},
      {2, 1, -2}, {3, 2, -3}, {3, 4, 9}, {4, 2, 7}, {4, 0, 2},
  };

  auto nodes = ConstructGraph(edges, 5);
  BellmanFord(nodes, edges, 0);
  for (auto node : nodes) {
    cout << node.idx << "'s p is: " << node.p << ", d is: " << node.d << endl;
  }
  cout << "---------------------" << endl;

  vector<Edge> dag_edges = {
      {0, 1, 5}, {0, 2, 3}, {1, 2, 2},  {1, 3, 6}, {2, 3, 7},
      {2, 4, 4}, {2, 5, 2}, {3, 4, -1}, {3, 5, 1}, {4, 5, -2},
  };
  auto dag_nodes = ConstructGraph(dag_edges, 6);
  auto topo_sorted = TopoSort(dag_nodes);
  DagShortestPaths(dag_nodes, 1);
  for (auto &node : dag_nodes) {
    cout << node.idx << "'s p is: " << node.p << ", d is: " << node.d << endl;
  }
  cout << "---------------------" << endl;

  vector<Edge> dij_edges = {
      {0, 1, 10}, {0, 3, 5}, {1, 2, 1}, {1, 3, 2}, {2, 4, 4},
      {3, 1, 3},  {3, 2, 9}, {3, 4, 2}, {4, 2, 6}, {4, 0, 3},
  };
  auto dij_nodes = ConstructGraph(dij_edges, 5);
  Dijkstra(dij_nodes, 0);
  for (auto &node : dij_nodes) {
    cout << node.idx << "'s p is: " << node.p << ", d is: " << node.d << endl;
  }
  cout << "---------------------" << endl;

  vector<Edge> johnson_edges = {
      {0, 1, 3}, {0, 2, 8}, {0, 4, -4}, {1, 3, 1}, {1, 4, 7},
      {2, 1, 4}, {3, 0, 2}, {3, 2, -5}, {4, 3, 6},
  };
  auto johnson_nodes = ConstructGraph(johnson_edges, 5);
  auto d = Johnson(johnson_nodes, johnson_edges);
  for (size_t i = 0; i < johnson_nodes.size(); ++i) {
    for (size_t j = 0; j < johnson_nodes.size(); ++j) {
      cout << d[i * johnson_nodes.size() + j] << " ";
    }
    cout << endl;
  }

  return 0;
}
