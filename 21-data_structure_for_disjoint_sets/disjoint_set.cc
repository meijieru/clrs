#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

class UFS {
 public:
  UFS(size_t n) : rank_(n + 1), parent_(n + 1) {}

  void MakeSet(size_t i) {
    parent_[i] = i;
    rank_[i] = 0;
  }

  size_t Find(size_t i) {
    if (parent_[i] == i) {
      return i;
    } else {
      parent_[i] = Find(parent_[i]);
      return parent_[i];
    }
  }

  void Link(size_t x, size_t y) {
    if (rank_[x] < rank_[y]) {
      parent_[x] = y;
    } else {
      parent_[y] = x;
      if (rank_[x] == rank_[y]) {
        ++rank_[x];
      }
    }
  }

  void Union(size_t x, size_t y) { Link(Find(x), Find(y)); }

 private:
  vector<size_t> rank_;
  vector<size_t> parent_;
};

class ListUfs {
 public:
  struct head;
  struct node {
    head *h;
    node *next;
    int key;
  };

  struct head {
    node *head, *tail;
  };

  ~ListUfs() {
    for (auto &a : root_) {
      auto p = a->head;
      auto tmp = p;
      while (p) {
        tmp = p;
        p = p->next;
        delete tmp;
      }
      delete a;
    }
  }

  node *MakeSet(int x) {
    auto inserted = new node;
    auto p = new head;
    p->head = inserted;
    p->tail = inserted;
    inserted->key = x;
    inserted->h = p;
    inserted->next = nullptr;
    root_.push_back(p);
    return inserted;
  }

  head *Find(node *x) { return x->h; }

  void Link(head *x, head *y) {
    x->tail->next = y->head;
    auto p = y->head;
    while (p) {
      p->h = x;
      p = p->next;
    }
    x->tail = y->tail;
    root_.remove(y);
    delete y;
  }

  void Union(node *x, node *y) { Link(Find(x), Find(y)); }

  void Print() {
    for (auto a : root_) {
      auto p = a->head;
      while (p) {
        cout << p->key << " ";
        p = p->next;
      }
      cout << endl;
    }
  }

 private:
  list<head *> root_;
};

int main() {
  ListUfs lfs;
  vector<ListUfs::node *> tb;

  for (int i = 0; i < 10; ++i) {
    tb.push_back(lfs.MakeSet(i));
  }
  for (int i = 0; i < 10; i += 4) {
    lfs.Union(tb[i], tb[i + 1]);
  }
  lfs.Print();
}
