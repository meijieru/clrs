#include <climits>
#include <iostream>
#include <stack>
#include <algorithm>

int i;
using namespace std;

class rbTree {
 public:
  enum kColor { kBlack, kRed };
  typedef struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    enum kColor color;
    int data;
  } Node;

  rbTree() : nil_(new Node) {
    nil_->color = kBlack;
    nil_->left = nil_->right = nil_->parent = nil_;
    nil_->data = 0;
    root_ = nil_;
    // cout << "qu" << endl;
  }
  ~rbTree() {
    Release(root_);
    delete nil_;
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
  }

  void Insert(int target) {
    Node *last(nil_);
    Node *current(root_);
    while (current != nil_) {
      // cout << "hehe" << endl;
      last = current;
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
    Node *y = ptr;
    Node *x = nil_;
    kColor y_origin_color = y->color;
    if (ptr->left == nil_) {
      x = ptr->right;
      Transplant(ptr, ptr->right);
    } else if (ptr->right == nil_) {
      x = ptr->left;
      Transplant(ptr, ptr->left);
    } else {
      y = Min(ptr->right);
      y_origin_color = y->color;
      x = y->right;
      if (y->parent == ptr) {
        x->parent = y;
      } else {
        Transplant(y, y->right);
        y->right = ptr->right;
        y->right->parent = y;
      }
      Transplant(ptr, y);
      y->left = ptr->left;
      y->left->parent = y;
      y->color = ptr->color;
    }

    // cout << x->data << endl;
    // cout << "x" << endl;
    if (y_origin_color == kBlack) {
      DeleteFixUp(x);
    }
    delete ptr;
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
  }
  Node *root_;
  Node *nil_;
};

class AvlTree {
 public:
  typedef struct node {
    struct node *left, *parent, *right;
    int data;
    int height;
  } Node;

  AvlTree() : nil_(0) { root_ = nil_; }
  ~AvlTree() { Release(root_); }

  void Release(Node *ptr) {
    if (ptr->left != nil_) Release(ptr->left);
    if (ptr->right != nil_) Release(ptr->right);
    delete ptr;
  }

  void Insert(int target) {
    Node *last(nil_);
    Node *current(root_);
    while (current != nil_) {
      last = current;
      if (target < current->data) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    Node *inserted = new Node;
    inserted->parent = last;
    inserted->left = nil_;
    inserted->right = nil_;
    inserted->data = target;
    inserted->height = 0;
    if (!last) {
      root_ = inserted;
    } else if (inserted->data < last->data) {
      last->left = inserted;
    } else {
      last->right = inserted;
    }
    Balance(inserted);
  }

  void Delete(Node *ptr) {
    Node *x = nil_;
    if (!ptr->left) {
      // x = ptr->parent;
      x = ptr->right;
      Transplant(ptr, ptr->right);
    } else if (!ptr->right) {
      // x = ptr->parent;
      x = ptr->left;
      Transplant(ptr, ptr->left);
    } else {
      Node *tmp = Min(ptr->right);
      // x = tmp->parent;
      x = tmp;
      {
        if (tmp->parent != ptr) {
          Transplant(tmp, tmp->right);
          tmp->right = ptr->right;
          tmp->right->parent = tmp;
        }
        Transplant(ptr, tmp);
        tmp->left = ptr->left;
        tmp->left->parent = tmp;
      }
    }
    delete ptr;

    // if(x){
    // Balance(x);
    DeleteBalance(x);
    // }
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

  void Transplant(Node *dst, Node *src) {
    if (!dst->parent) {
      root_ = src;
    } else if (dst == dst->parent->left) {
      dst->parent->left = src;
    } else {
      dst->parent->right = src;
    }
    if (src) {
      src->parent = dst->parent;
    }
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

    // fix the height
    x->height = max(Height(x->left), Height(x->right)) + 1;
    y->height = max(Height(y->left), Height(y->right)) + 1;
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

    x->height = max(Height(x->left), Height(x->right)) + 1;
    y->height = max(Height(y->left), Height(y->right)) + 1;
  }

  void InOrderTreeWalk(void) { InOrderTreeWalk(root_); }
  void InOrderTreeWalk(Node *ptr) {
    if (ptr != nil_) {
      InOrderTreeWalk(ptr->left);
      cout << "height:" << ptr->height << " " << ptr->data << endl;
      InOrderTreeWalk(ptr->right);
    }
  }

 private:
  int Height(Node *x) { return x ? x->height : -1; }
  void Balance(Node *x) {
    int key = x->data;
    x = x->parent;
    while (x) {
      int deta = Height(x->left) - Height(x->right);
      if (deta == 2) {
        if (key < x->left->data) {
          RightRotate(x);
        } else {
          LeftRotate(x->left);
          RightRotate(x);
        }
      } else if (deta == -2) {
        if (key > x->right->data) {
          LeftRotate(x);
        } else {
          RightRotate(x->right);
          LeftRotate(x);
        }
      }
      x->height = max(Height(x->left), Height(x->right)) + 1;
      x = x->parent;
    }
  }

  // die
  void DeleteBalance(Node *x) {
    int key = x->data;
    x = x->parent;
    cout << "Balance1" << endl;
    while (x) {
      int deta = Height(x->left) - Height(x->right);
      cout << "Balance" << endl;
      if (deta == 2) {
        if (key < x->left->data) {
          RightRotate(x);
        } else {
          LeftRotate(x->left);
          RightRotate(x);
        }
      } else if (deta == -2) {
        if (key > x->right->data) {
          LeftRotate(x);
        } else {
          RightRotate(x->right);
          LeftRotate(x);
        }
      }
      x->height = max(Height(x->left), Height(x->right)) + 1;
      x = x->parent;
    }
  }

  Node *root_;
  Node *nil_;
};

class Treap {
 public:
  typedef struct node {
    struct node *left, *parent, *right;
    int data;
    int priority;
  } Node;

  Treap() : root_(0), kNil(0) {}
  ~Treap() { Release(root_); }

  void Insert(int target) {
    Node *last(kNil);
    Node *current(root_);
    while (current != kNil) {
      last = current;
      if (target < current->data) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    Node *inserted = new Node;
    inserted->parent = last;
    inserted->left = kNil;
    inserted->right = kNil;
    inserted->data = target;
    inserted->priority = rand() % 100;
    if (!last) {
      root_ = inserted;
    } else if (inserted->data < last->data) {
      last->left = inserted;
    } else {
      last->right = inserted;
    }
    Fix(inserted);
  }

  void Release(Node *ptr) {
    if (ptr->left != kNil) Release(ptr->left);
    if (ptr->right != kNil) Release(ptr->right);
    delete ptr;
  }

  void LeftRotate(Node *x) {
    Node *y = x->right;

    // turn y's left subtree into x's right subtree
    x->right = y->left;
    if (kNil != y->left) {
      y->left->parent = x;
    }

    // link x's parent to y
    y->parent = x->parent;
    if (kNil == x->parent) {  // change
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    // put x on y's left
    y->left = x;
    x->parent = y;
  }

  void RightRotate(Node *y) {
    Node *x = y->left;

    y->left = x->right;
    if (kNil != x->right) {
      x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == kNil) {
      root_ = x;
    } else if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
  }

  void InOrderTreeWalk(void) { InOrderTreeWalk(root_, 0); }
  void InOrderTreeWalk(Node *ptr, int height) {
    if (ptr != kNil) {
      InOrderTreeWalk(ptr->left, height + 1);
      cout << "height:" << height << " " << ptr->data
           << " pri:" << ptr->priority << endl;
      InOrderTreeWalk(ptr->right, height + 1);
    }
  }

 private:
  void Fix(Node *x) {
    while (x->parent && x->parent->priority > x->priority) {
      if (x == x->parent->left) {
        RightRotate(x->parent);
      } else {
        LeftRotate(x->parent);
      }
    }
  }

  Node *root_;
  Node *kNil;
};

int main() {
  Treap avl;
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
  avl.Insert(10);
  avl.Insert(32);
  avl.Insert(1);
  avl.Insert(22);
  avl.Insert(15);
  avl.InOrderTreeWalk();
  // avl.Delete(avl.Search(1));
  cout << "hehe" << endl;
  // avl.InOrderTreeWalk();
  avl.Insert(8);
  avl.Insert(11);
  avl.Insert(3);
  // avl.Delete(avl.Search(22));
  avl.Insert(14);
  avl.Insert(2);
  avl.Insert(45);
  avl.Insert(18);
  // tree.Insert(7);
  // tree.Insert(8);
  // tree.Insert(9);
  // tree.Delete(12);
  cout << "kk" << endl;
  avl.InOrderTreeWalk();
  cout << "tag1" << endl;
}
