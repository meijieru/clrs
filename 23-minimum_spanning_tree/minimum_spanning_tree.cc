#include <climits>

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>
#include <functional>

using namespace std;

class UFS {
 public:
  UFS(size_t size) : set_(size) {
    for (size_t i = 0; i < size; ++i) {
      set_[i].parent = i;
      set_[i].rank = 0;
    }
  }

  void Union(int x, int y) { Link(FindSet(x), FindSet(y)); }

  int FindSet(int x) {
    if (set_[x].parent != x) {
      set_[x].parent = FindSet(set_[x].parent);
    }

    return set_[x].parent;
  }

 private:
  struct Node {
    int parent{-1};
    int rank{0};
  };

  void Link(int x, int y) {
    if (set_[x].rank < set_[y].rank) {
      set_[y].parent = x;
    } else {
      set_[x].parent = y;

      if (set_[x].rank == set_[y].rank) {
        set_[y].rank += 1;
      }
    }
  }

  vector<Node> set_;
};

struct Point {
  Point(void) : prev(0), key(INT_MAX) {}
  int x;
  int prev;
  int key;
};

struct Edge {
  Edge(int x, int y, int weight) : x(x), y(y), weight(weight) {}
  int x, y, weight;
};

vector<pair<int, int>> Kruskal(vector<Edge> &edges, size_t size) {
  auto fun = [&](Edge x, Edge y) { return x.weight < y.weight; };
  sort(edges.begin(), edges.end(), fun);

  // for (auto &a : edges) {
  // cout << a.x << " " << a.y << " " << a.weight << endl;
  // }
  // cout << "-----------------------" << endl;

  UFS disjoint(size + 1);
  vector<pair<int, int>> path;
  int count = 0;

  for (size_t i = 0; i < edges.size(); ++i) {
    if (disjoint.FindSet(edges[i].x) != disjoint.FindSet(edges[i].y)) {
      path.push_back({edges[i].x, edges[i].y});
      disjoint.Union(edges[i].x, edges[i].y);
      count += edges[i].weight;
    }
  }

  return path;
}

struct AdjacentList {
  AdjacentList(size_t size, vector<Edge> &edges) : lists(size) {
    for (auto &a : edges) {
      lists[a.x].push_back(Node(a.y, a.weight));
      lists[a.y].push_back(Node(a.x, a.weight));
    }
  };

  struct Node {
    Node(int y, int weight) : y(y), weight(weight) {}
    int y;
    int weight;
  };

  vector<list<Node>> lists;
};

void RefreshMinHeap(
    priority_queue<Point *, vector<Point *>, function<bool(Point *, Point *)>>
        &min_heap) {
  vector<Point *> blob(min_heap.size());

  auto n = min_heap.size();
  for (size_t i = 0; i < n; ++i) {
    blob[i] = min_heap.top();
    min_heap.pop();
  }

  for (auto &a : blob) {
    min_heap.push(a);
  }
}

vector<pair<int, int>> Prim(vector<Edge> &edges, int src, size_t size) {
  auto fun = [](Point *x, Point *y) { return x->key > y->key; };

  vector<Point> points(size + 1);
  vector<int> visit(size + 1);
  vector<pair<int, int>> path;

  AdjacentList adj(size + 1, edges);

  int count = 0;

  for (size_t i = 1; i < size + 1; ++i) {
    points[i].x = i;
  }
  points[src].key = 0;
  // cout << "src:" << src << endl;

  priority_queue<Point *, vector<Point *>, function<bool(Point *, Point *)>>
      min_heap(fun);

  for (size_t i = 1; i < size + 1; ++i) {
    min_heap.push(&points[i]);
  }

  visit[1] = true;
  while (min_heap.size()) {
    auto u = min_heap.top();
    path.push_back({points[u->x].prev, u->x});
    count += points[u->x].key;
    visit[u->x] = true;
    // cout << u->x << " " << u->key << endl;
    min_heap.pop();

    for (auto &a : adj.lists[u->x]) {
      if (visit[a.y]) {
        continue;
      }

      if (a.weight < points[a.y].key) {
        points[a.y].prev = u->x;
        points[a.y].key = a.weight;
        RefreshMinHeap(min_heap);
      }
    }
  }
  cout << count << endl;

  return path;
}

int main(void) {
  vector<Edge> edges{
      {1, 2, 4}, {2, 3, 8}, {3, 4, 7},  {4, 5, 9}, {5, 6, 10},
      {6, 7, 2}, {7, 8, 1}, {8, 9, 7},  {8, 1, 8}, {2, 8, 11},
      {3, 9, 2}, {3, 6, 4}, {4, 6, 14}, {7, 9, 6},
  };

  for (auto &p : Kruskal(edges, 9)) {
    cout << p.first << ' ' << p.second << endl;
  }
  cout << endl;

  for (auto &p : Prim(edges, 1, 9)) {
    cout << p.first << ' ' << p.second << endl;
  }

  return 0;
}
