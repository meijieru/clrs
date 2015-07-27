#include <cmath>
#include <climits>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

int i = 0;

class FibHeap {
public:
	typedef struct node {
		struct node *parent;
		list<struct node *> child;
		int key;
		int mark;
		int ischeck{0};
	} Node;

	FibHeap(): min_(0),nil_(0),size_(0){}
	~FibHeap() {
		Release();
	}

	void Release(void) {
		for (auto a:list_) {
			Release(a);
		}
	}
	void Release(Node *p) {
		if (!p) {
			return ;
		}

		for (auto a:p->child) {
			Release(a);
		}
		delete p;
	}

	void Print(void) {
		if (list_.size() == 0) {
			cout << "empty" << endl;
		}
		for (auto x:list_) {
			PrintNode(x,0);
		}
	}

	void PrintNode(Node *x,int depth) {
		cout << x->key << " " << (x->mark ? "marked " : "nmarked ")
			<< "depth:" << depth << endl;
		for (auto a:x->child) {
			PrintNode(a,depth + 1);
		}
	}

	Node *Insert(int x) {
		auto inserted = new Node;
		inserted->parent = nil_;
		inserted->key = x;
		inserted->mark = 0;
		list_.push_back(inserted);
		if (min_ == nil_) {
			min_ = inserted;
		} else {
			if (x < min_->key) {
				min_ = inserted;
			}
		}
		++size_;
		return inserted;
	}

	void Union(FibHeap &x) {
		// for (auto a:x.list_) {
			// list_.push_back(a);
		// }
		list_.splice(list_.begin(),list_,x.list_.begin(),x.list_.end());
		for (auto a:list_) {
			cout << a->key << " " ;
		}

		if (min_ == nil_ || 
				(x.min_ != nil_ && x.min_->key < min_->key)) {
			min_ = x.min_;
		}

		size_ += x.size_;
		x.size_ = 0;
		x.list_.empty();
		x.min_ = 0;
	}

	void Debugp(void) {
		if (min_ != nil_) {
			cout <<"min:" << min_->key << endl;
		}
		cout << list_.size() << endl;
		cout <<"size:"<< size_ << endl;
	}

	Node *ExtractMin(void) {
		Node *z = min_;

		list_.splice(list_.begin(),list_,min_->child.begin(),min_->child.end());
		if (min_ != nil_) {
			list_.remove(min_);
		} 

		if (list_.size() == 0) {
			min_ = nil_;
		} else {
			Consolidate();
			// cout << "here" << endl;
			min_ = list_.front();
			for (auto a:list_) {
				if (min_->key > a->key) {
					min_ = a;
				}
			}
		}
		// cout << min_->key << "therre" << endl;
		--size_;
		// Print();
		return z;
	}

	void DecreaseKey(Node *x,int k) {
		if (k > x->key) {
			cout << "too little" << endl;
		}

		x->key = k;
		auto y = x->parent;
		if (y != nil_ && x->key < y->key) {
			Cut(x,y);
			CascadingCut(y);
		}
		if (x->key < min_->key) {
			min_ = x;
		}
		// cout << min_->key;
	}
	
	void Delete(Node *x) {
		DecreaseKey(x,INT_MIN);
		delete ExtractMin();
	}

private:
	void Link(Node *c,Node *p) {
		list_.remove(c);
		p->child.push_back(c);
		c->parent = p;
		c->mark = 0;
	}

	void Consolidate(void) {
		int dn = log2(size_) + 1;
		vector<Node *> degree(dn,nil_);
		while (1) {
			Node *x = list_.front();
			// cout << x->key << endl;
			// cout <<"check" << x->ischeck << endl;
			if (x->ischeck) {
				break;
			}
			list_.pop_front();
			size_t d = x->child.size();
			while (degree[d] != nil_) {
				Node *y = degree[d];
				if (y->key < x->key) {
					swap(x,y);
				}
				// change the list,so the iterator died
				Link(y,x);
				list_.remove(x);
				degree[d] = nil_;
				++d;
			}
			degree[d] = x;
			x->ischeck = 1;
			list_.push_back(x);
		}
		for (auto a:list_) {
			a->ischeck = 0;
		}
	}

	void Cut(Node *c,Node *p) {
		p->child.remove(c);
		list_.push_back(c);
		c->parent = nil_;
		c->mark = 0;
	}

	void CascadingCut(Node *x) {
		Node *p = x->parent;
		if (p != nil_) {
			if (x->mark == 0) {
				x->mark = 1;
			} else {
				Cut(x,p);
				CascadingCut(p);
			}
		}
	}

	list<Node *> list_;
	Node *min_;
	Node *nil_;
	size_t size_;
};

class BinomialHeap {
public:
	class BinomialTree {
		public:
			typedef struct node {
				struct node *parent,*sibling,*child;
				int degree;
				int key;
				int check{0};
			} Node;

			BinomialTree() : root_(0) {}
			BinomialTree(int k) : root_(new Node) {
				root_->sibling = 0;
				root_->parent = 0;
				root_->child = 0;
				root_->key = k;
				root_->degree = 0;
			}
			~BinomialTree() { Release(root_); }

			void Print() {
				Print(root_,0);
			}
			void Print(Node *p,int depth) {
				if (!p) {
					return ;
				}
				for (int i = 0; i < depth; ++i) {
					cout << " ";
				}
				cout << p->key << endl;
				
				Print(p->child,depth + 1);
				if (p->parent && p->parent->child == p){
					while (p->sibling) {
						Print(p->sibling,depth);
						p = p->sibling;
					}
				}
			}

			Node *Min(void) {
				if (root_) {
					return root_;
				}
				return nullptr;
			}

			int Degree(void) {
				if (root_) {
					return root_->degree;
				}
				return INT_MIN;
			}

			void Release(Node *p) {
				if (p) {
					Release(p->child);
					Release(p->sibling);
					delete p;
				}
			}

			void Merge(BinomialTree &y) {
				if (root_->key > y.root_->key) {
					root_->sibling = y.root_->child;
					y.root_->child = root_;
					root_->parent = y.root_;
					root_ = y.root_;
				} else {
					y.root_->sibling = root_->child;
					root_->child = y.root_;
					y.root_->parent = root_;
				}
				++root_->degree;
				y.root_ = 0;
			}
			
			BinomialTree *Split(void) {
				BinomialTree *bt = new BinomialTree;
				Node *r = root_->child;
				r->parent = 0;
				bt->root_ = r;

				--root_->degree;
				root_->child = r->sibling;

				r->sibling = 0;
				return bt;
			}

		private:
			Node *root_;
	};

	struct ListNode {
		ListNode *next;
		BinomialTree *ptree;
	};

	ListNode *ListInsert(ListNode *ptr) {
		int degree = ptr->ptree->Degree();
		ListNode *p = list_;
		ListNode *prev = nullptr;

		while (p && p->ptree->Degree() < degree) {
			prev = p;
			p = p->next;
		} 
		
		ptr->next = p;
		if (prev) {
			prev->next = ptr;
		} else {
			list_ = ptr;
		}
		return ptr;
	}
	
	ListNode *ListDelete(ListNode *prev,ListNode *p) {
		if (p == list_) {
			list_ = p->next;
		} else {
			prev->next = p->next;
		}
		return p;
	}

	BinomialHeap() : list_(0) {}
	~BinomialHeap() {
		ListNode *p = list_;
		ListNode *prev = nullptr;
		while (p) {
			prev = p;
			p = p->next;
			delete prev->ptree;
			delete prev;
		}
	}

	void Print() {
		ListNode *p = list_;
		// while (p) {
			// cout << p->ptree->Min() << " " << endl;
			// p = p->next;
		// }
		while (p) {
			p->ptree->Print();
			p = p->next;
		}
	}

	BinomialTree::Node *Insert(int x) {
		ListNode *inserted = new ListNode;
		inserted->next = 0;
		inserted->ptree = new BinomialTree{x};
		auto tmp = inserted->ptree->Min();
		ListInsert(inserted);
		Consolidate(list_);
		return tmp;
	}

	// and delete the node
	int  ExtractMin(void) {
		ListNode *p = list_;
		ListNode *pmin = nullptr;
		int min = INT_MAX;
		while (p) {
			if (p->ptree->Min()->key < min) {
				pmin = p;
				min = p->ptree->Min()->key;
			}
			p = p->next;
		}

		p = list_;
		ListNode *prev = nullptr;
		while (p && p->next != pmin) {
			p = p->next;
		}
		prev = p;
		
		ListDelete(prev,pmin);
			// cout << "pmin" << endl;
		while (pmin->ptree->Degree() != 0) {
			auto inserted = new ListNode;
			inserted->ptree = pmin->ptree->Split();
			ListInsert(inserted);
			Consolidate(inserted);
			// Consolidate(list_);
		}
		delete pmin->ptree;
		delete pmin;
		return min;
	}

	void Union(BinomialHeap &x) {
		ListNode *p = x.list_;
	
		while (p) {
			auto tmp = p;
			p = p->next;
			ListInsert(tmp);
		}
		x.list_ = 0;
		Consolidate(list_);
	}

	void Consolidate(ListNode *x) {
		ListNode *p = x;
		while (p && p->next) {
			if (p->ptree->Degree() == 
					p->next->ptree->Degree()) {
				p->ptree->Merge(*p->next->ptree);
				ListDelete(p,p->next);
			} else {
				p = p->next;
			}
		}
	}

	void DecreaseKey(BinomialTree::Node *ptr,int k) {
		if (k > ptr->key) {
			cout << "too little" << endl;
			return ;
		}

		ptr->key = k;
		BinomialTree::Node *prev = nullptr;
		auto p = ptr->parent;
		while (p && p->key > k) {
			prev = p;
			p = p->parent;
		}
		cout << "h--------------------------" << endl;
		if (prev) {
			auto tmp = prev->key;
			prev->key = k;
			ptr->key = tmp;
		}
	} 

	int Delete(BinomialTree::Node *p) {
		DecreaseKey(p,INT_MIN);
		Print();
		return ExtractMin();
	}

private:
	ListNode *list_;
};


int main() {
	FibHeap fh;
	FibHeap fh2;
	vector <FibHeap::Node *> v;
	for (int i = 0; i < 10; ++i) {
		v.push_back(fh.Insert(i + 1));
	}
	for (int i = 0; i < 5; ++i) {
		fh2.Insert(i + 6);
	}
	fh.Union(fh2);
	fh.Print();
	auto p = fh.ExtractMin();
	cout << "1hhhhhhhhhhhhh" << p->key<< endl;
	fh.Print();
	fh.DecreaseKey(v[4],0);
	cout << "target" << v[3]->key << endl;
	fh.Delete(v[3]);
	fh.Print();
	cout << "---------------------------------" << endl;

	BinomialHeap bh,bh2;
	for (int i = 0; i < 8; ++i) {
		bh.Insert(i + 1);
	}
	for (int i = 0; i < 5; ++i) {
		bh2.Insert(i+3);
	}
	bh.Union(bh2);
	bh.Print();
	cout << "---------------------------" << endl;
	cout << "min " << bh.ExtractMin() << endl;
	bh.Print();
	bh.Insert(8);
	bh.DecreaseKey(bh.Insert(9),0);
	bh.Print();
	cout << "min " << bh.ExtractMin() << endl;
	// bh.Delete(bh.Insert(9));
	bh.Print();
	
	cout << "end" << endl;
}
