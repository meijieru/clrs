#include <climits>
#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

class OsTree {
 public:
  enum kColor { kBlack, kRed };
  typedef struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    enum kColor color;
    int data;
    int size;
  } Node;

  OsTree() : nil_(new Node) {
    nil_->color = kBlack;
    nil_->left = nil_->right = nil_->parent = nil_;
    nil_->data = 0;
    nil_->size = 0;
    root_ = nil_;
    // cout << "qu" << endl;
  }
  ~OsTree() {
    Release(root_);
    delete nil_;
  }

  Node *Select(Node *x, int rank) {
    int r = x->left->size + 1;
    if (r == rank) {
      return x;
    } else if (r < rank) {
      return Select(x->right, rank - r);
    } else {
      return Select(x->left, rank);
    }
  }

  int Rank(Node *x) {
    int r = x->left->size + 1;
    Node *y = x;
    while (y != root_) {
      if (y == y->parent->right) {
        r = r + y->parent->left->size + 1;
      }
      y = y->parent;
    }
    return r;
  }

  int Rank(int key) { return Rank(Search(key)); }

  int KeyRank(int key) {
    Node *p = root_;
    int r = 0;
    while (key != p->data) {
      // not in the tree
      if (p == nil_) {
        return -1;
      }
      if (key < p->data) {
        p = p->left;
      } else {
        r += p->left->size + 1;
        p = p->right;
      }
    }
    // add itself
    return r + 1;
  }

  void LeftRotate(Node *x) {
    Node *y = x->right;

    // turn y's left subtree into x's right subtree
    x->right = y->left;
    if (nil_ != y->left) {
      y->left->parent = x;
    }

    // link x's parent to y
    y->parent = x->parent;
    if (nil_ == x->parent) {  // change
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    // put x on y's left
    y->left = x;
    x->parent = y;

    // fix the size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
  }

  void RightRotate(Node *y) {
    Node *x = y->left;

    y->left = x->right;
    if (nil_ != x->right) {
      x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == nil_) {
      root_ = x;
    } else if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }

    x->right = y;
    y->parent = x;

    x->size = y->size;
    y->size = y->left->size + y->right->size + 1;
  }

  void Insert(int target) {
    Node *last(nil_);
    Node *current(root_);
    while (current != nil_) {
      // cout << "hehe" << endl;
      last = current;
      ++current->size;
      // cout << current->data << " " << current->size << endl;
      if (target < current->data) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    // cout << "ha" << endl;
    Node *inserted = new Node;
    inserted->parent = last;
    inserted->left = nil_;
    inserted->right = nil_;
    inserted->data = target;
    inserted->color = kRed;
    inserted->size = 1;
    if (nil_ == last) {
      root_ = inserted;
    } else if (inserted->data < last->data) {
      last->left = inserted;
    } else {
      last->right = inserted;
    }

    InsertFixUp(inserted);
  }

  void Delete(int target) { Delete(Search(target)); }
  void Delete(Node *ptr) {
    // fix the size of the ancestor
    Node *tmp = ptr;
    while (tmp->parent != nil_) {
      tmp->parent->size--;
      tmp = tmp->parent;
    }
    // cout << "here change" << endl;
    // InOrderTreeWalk();
    Node *y = ptr;
    Node *x = nil_;
    kColor y_origin_color = y->color;
    if (ptr->left == nil_) {
      x = ptr->right;
      Transplant(ptr, ptr->right);
      // change
      --ptr->right->size;
    } else if (ptr->right == nil_) {
      x = ptr->left;
      Transplant(ptr, ptr->left);
      // change
      --ptr->left->size;
    } else {
      y = Min(ptr->right);
      // change
      Node *tmp2 = y->right;
      y_origin_color = y->color;
      x = y->right;
      if (y->parent == ptr) {
        x->parent = y;
      } else {
        Transplant(y, y->right);
        // change
        --y->right->size;
        y->right = ptr->right;
        y->right->parent = y;
        // fix the size
        // y->size = y->left->size + y->right->size + 1;
      }
      Transplant(ptr, y);
      y->left = ptr->left;
      y->left->parent = y;
      y->color = ptr->color;
      // change
      --y->size;

      // fix the size between tmp2 and y
      while (tmp2->parent != y) {
        --tmp2->parent->size;
        cout << tmp2->data << tmp2->parent->data << endl;
        tmp2 = tmp2->parent;
      }
    }

    // InOrderTreeWalk();
    // cout << x->data << endl;
    // cout << "x" << endl;
    if (y_origin_color == kBlack) {
      DeleteFixUp(x);
    }
    delete ptr;
    // cout << "nilsize" << nil_->size << endl;
  }

  Node *Search(int target) {
    Node *tmp = root_;
    while (tmp != nil_ && tmp->data != target) {
      if (target < tmp->data) {
        tmp = tmp->left;
      } else {
        tmp = tmp->right;
      }
    }
    return tmp;
  }

  Node *Max(void) { return Max(root_); }

  Node *Max(Node *ptr) {
    while (ptr->right != nil_) {
      ptr = ptr->right;
    }
    return ptr;
  }

  Node *Min(void) { return Min(root_); }

  Node *Min(Node *ptr) {
    while (ptr->left != nil_) {
      ptr = ptr->left;
    }
    return ptr;
  }

  Node *Successor(Node *ptr) {
    if (ptr->right != nil_) return Min(ptr->right);
    Node *tmp = ptr->parent;
    while (tmp != nil_ && tmp->right == ptr) {
      ptr = tmp;
      tmp = tmp->parent;
    }
    return tmp;
  }

  Node *Predecessor(Node *ptr) {
    if (ptr->left != nil_) return Max(ptr->left);
    Node *tmp = ptr->parent;
    while (tmp != nil_ && tmp->left == ptr) {
      ptr = tmp;
      tmp = tmp->parent;
    }
    return tmp;
  }

  void Release(Node *ptr) {
    if (ptr->left != nil_) Release(ptr->left);
    if (ptr->right != nil_) Release(ptr->right);
    delete ptr;
  }

  void InOrderTreeWalk(void) { InOrderTreeWalk(root_, 0); }
  void InOrderTreeWalk(Node *ptr, int height) {
    if (ptr != nil_) {
      InOrderTreeWalk(ptr->left, height + 1);
      cout << "height:" << height << " " << ptr->data;
      cout << " size:" << ptr->size << " ";
      if (ptr->color == kRed) {
        cout << " red" << endl;
      } else {
        cout << " black" << endl;
      }
      InOrderTreeWalk(ptr->right, height + 1);
    }
  }

 private:
  void InsertFixUp(Node *ptr) {
    while (ptr->parent->color == kRed) {
      // case 1
      if (ptr->parent == ptr->parent->parent->left) {
        Node *tmp = ptr->parent->parent->right;
        // case 1.1
        if (tmp->color == kRed) {
          ptr->parent->color = kBlack;
          tmp->color = kBlack;
          ptr->parent->parent->color = kRed;
          ptr = ptr->parent->parent;
        } else {
          // case 1.2 transform to the case 1.3
          if (ptr == ptr->parent->right) {
            ptr = ptr->parent;
            LeftRotate(ptr);
          }
          // case 1.3
          ptr->parent->color = kBlack;
          ptr->parent->parent->color = kRed;
          RightRotate(ptr->parent->parent);
        }
      } else {
        // case 2 symmetric to the case 1
        Node *tmp = ptr->parent->parent->left;
        if (tmp->color == kRed) {
          ptr->parent->color = kBlack;
          tmp->color = kBlack;
          ptr->parent->parent->color = kRed;
          ptr = ptr->parent->parent;
        } else {
          if (ptr == ptr->parent->left) {
            ptr = ptr->parent;
            RightRotate(ptr);
          }
          ptr->parent->color = kBlack;
          ptr->parent->parent->color = kRed;
          LeftRotate(ptr->parent->parent);
        }
      }
    }
    root_->color = kBlack;
  }

  // fix the size in rotate
  void DeleteFixUp(Node *x) {
    Node *w;
    // regard x as an extra black,to keep the property 5
    while (x != root_ && x->color == kBlack) {
      // cout << i++ << "root:" << root_->data << endl;
      // case 1
      if (x == x->parent->left) {
        // cout << "w1" << endl;
        w = x->parent->right;
        // case 1.1 transform to other
        if (w->color == kRed) {
          // cout << "eh1" << endl;
          w->color = kBlack;
          x->parent->color = kRed;
          LeftRotate(x->parent);
          w = x->parent->right;
        }
        // case 1.2
        if (w->left->color == kBlack && w->right->color == kBlack) {
          w->color = kRed;
          x = x->parent;
          // cout << "eh2" << endl;
        } else {
          // case 1.3
          // cout << "eh3" << endl;
          if (w->right->color == kBlack) {
            w->left->color = kBlack;
            w->color = kRed;
            RightRotate(w);
            w = x->parent->right;
          }
          // case 1.4
          // cout << "eh4" << endl;
          w->color = x->parent->color;
          x->parent->color = kBlack;
          w->right->color = kBlack;
          LeftRotate(x->parent);
          x = root_;
        }
      } else {
        // symmetric
        w = x->parent->left;
        if (w->color == kRed) {
          w->color = kBlack;
          x->parent->color = kRed;
          RightRotate(x->parent);
          w = x->parent->left;
        }
        // case 1.2
        if (w->left->color == kBlack && w->right->color == kBlack) {
          w->color = kRed;
          x = x->parent;
        } else {
          // case 1.3
          if (w->left->color == kBlack) {
            w->right->color = kBlack;
            w->color = kRed;
            LeftRotate(w);
            w = x->parent->left;
          }
          // case 1.4
          w->color = x->parent->color;
          x->parent->color = kBlack;
          w->left->color = kBlack;
          RightRotate(x->parent);
          x = root_;
        }
      }
    }
    x->color = kBlack;
    // cout << "woqu" << x->data << endl;
  }

  void Transplant(Node *dst, Node *src) {
    if (dst->parent == nil_) {
      root_ = src;
    } else if (dst->parent->left == dst) {
      dst->parent->left = src;
    } else {
      dst->parent->right = src;
    }
    src->parent = dst->parent;
    // cout << "change" << endl;
    // fix the size outside
    src->size = dst->size;
  }

  Node *root_;
  Node *nil_;
};

int main() {
  OsTree ostree;
  // tree.Insert(41);
  // tree.Insert(38);
  // tree.Insert(31);
  // tree.Insert(12);
  // tree.Insert(119);
  // tree.Insert(8);
  // tree.Insert(1);
  // tree.Insert(2);
  // tree.Insert(3);
  // tree.Insert(4);
  // tree.Insert(5);
  // tree.Insert(6);
  ostree.Insert(10);
  ostree.Insert(32);
  ostree.Insert(1);
  // ostree.InOrderTreeWalk();
  ostree.Insert(22);
  ostree.Insert(15);

  ostree.InOrderTreeWalk();
  ostree.Delete(ostree.Search(32));
  cout << "delete" << endl;
  ostree.InOrderTreeWalk();
  cout << "hehe" << endl;
  // ostree.InOrderTreeWalk();
  // ostree.Insert(8);
  // ostree.Insert(11);
  // ostree.Insert(3);
  // ostree.Delete(ostree.Search(22));
  // ostree.Insert(14);
  // ostree.Insert(2);
  // ostree.Insert(45);
  // ostree.Insert(18);
  // cout << "Rank:" << ostree.Rank(ostree.Search(1)) << endl;
  // cout << "Rank:" << ostree.Rank(ostree.Search(32)) << endl;
  // cout << "KeyRank:" << ostree.KeyRank(32) << endl;
  // // tree.Insert(7);
  // // tree.Insert(8);
  // // tree.Insert(9);
  // // tree.Delete(12);
  // cout << "kk" << endl;
  // ostree.InOrderTreeWalk();
  /* cout << "tag1" << endl; */
}
