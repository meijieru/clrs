#include <climits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

class BTree {
public:
	typedef struct node {
		vector<struct node *> kids;
		vector<int> key;
		int isleaf = 0;
	}Node;

	BTree(int t):t_(t) {
		root_ = AllocateNode();
		root_->isleaf = 1;
		DiskWrite(root_);
	}
	~BTree(void) {
		Release(root_);
	}

	void DebugHook(vector<int> x) {
		for (auto a:x) {
			cout << a << " ";
		}
		cout << endl;
	}
	void Release(Node *p) {
		if (!p) {
			for (auto a:p->kids) {
				Release(a);
			}
		}
	}

	void Print(void) {
		Print(root_,0);
	}

	void Print(Node *p,int depth) {
		// cout << "l:" << p->key.size() << " ";
		cout << "[";
		for (auto a:p->key) {
			cout << a << " ";
		}
		cout << "]";
		// cout << "depth:" << depth << endl;

		if (!IsLeaf(p)) {
			for (auto a:p->kids) {
				// cout << "kd" << a->isleaf << endl;
				Print(a,depth+1);
			}
		}
	}

	int IsLeaf(Node *p) {
		return p->isleaf;
	}

	pair<Node *,int> Search(int x) {
		Node *p = root_;
		while (1) {
			int i = 0;
			int n = p->key.size();
			while (i < n && x > p->key[i]) {
				++i;
			}

			if (i < n && x == p->key[i]) {
				return {p,i};
			}

			if (IsLeaf(p)) {
				return {0,0};
			}

			DiskRead(p->kids[i]);
			p = p->kids[i];
		}
	}

	pair<Node *,int> Min(void) {
		return Min(root_);
	}
	pair<Node *,int> Min(Node *ptr) {
		if (!ptr->key.size()) {
			return {0,0};
		}

		Node *p = ptr;
		while(!p->isleaf) {
			DiskRead(p->kids[0]);
			p = p->kids[0];
		}
		
		return {p,0};
	}

	pair<Node *,int> Max(void) {
		return Max(root_);
	}
	pair<Node *,int> Max(Node *ptr) {
		if (!ptr->key.size()) {
			return {0,0};
		}

		Node *p = ptr;
		while(!p->isleaf) {
			DiskRead(p->kids[p->kids.size()-1]);
			p = p->kids[p->kids.size()-1];
		}
		
		return {p,p->key.size()-1};
	}

	pair<Node *,int> Predesessor(int x) {
		Node *p = root_;
		stack<pair<Node *,int>> s;
		while (1) {
			int i = 0;
			int n = p->key.size();
			while (i < n && x > p->key[i]) {
				++i;
			}
			if (i < n && x == p->key[i]) {
				s.push({p,i});
				break;
			}

			if (IsLeaf(p)) {
				s.push({0,0});
				break;
			}

			DiskRead(p->kids[i]);
			s.push({p,i});
			p = p->kids[i];
		}

		pair<Node *,int> t = s.top();s.pop();
		if (!t.first) {
			return {0,0};
		}

		if (!t.first->isleaf) {
			return Max(t.first->kids[t.second]);
		}

		if (t.second) {
			return {t.first,t.second-1};
		}

		if (!s.size()) {
			return {0,0};
		} else {
			pair<Node *,int> tmp = s.top();s.pop();
			while (tmp.first->key[tmp.second] > x) {
				if (tmp.second) {
					return {tmp.first,tmp.second-1};
				}
				if (s.size()) {
					tmp = s.top();s.pop();
				} else {
					break;
				}
			}
			if (tmp.first->key[tmp.second] > x) {
				return {0,0};
			}
			return {tmp.first,tmp.second-1};
		}
	}

	void Insert(int k) {
		if (root_->key.size() == (size_t)(2*t_-1)) {
			// cout << "root full" << endl;
			Node *r = AllocateNode();
			r->isleaf = 0;
			r->kids.resize(1);			
			r->kids[0] = root_;
			root_ = r;
			SplitChild(r,0);
			// Print();
			// cout << "hhhhhhhhhhhhhh" << endl;
			InsertNonfull(r,k);
		} else {
			// cout << "root not full" << endl;
			InsertNonfull(root_,k);
		}
	}

	void Delete(int k) {
		if (root_->key.size() == 1) {
			if (root_->isleaf) {
				DeleteNonone(root_,k);
			} else {
				DiskRead(root_->kids[0]);
				DiskRead(root_->kids[1]);
				// the only case that the height will decrease
				if (root_->kids[0]->key.size() == (size_t)(t_ - 1) &&
						root_->kids[1]->key.size() == (size_t)(t_ - 1)) {
					Node *tmp = MergeChild(root_,0);
					DeleteNode(root_);
					root_ = tmp;
				}
			}
		}

		DeleteNonone(root_,k);
		// cout << "get" << endl;
	}

	// garantee that every recursive the node p has at least t_ key
	void DeleteNonone(Node *p,int k) {
		// if (!p) {
			// return;
		// }
		// case 1
		
		if (IsLeaf(p)) {
			cout << "case 1" <<endl;
			size_t i = 0;
			while (i < p->key.size() && p->key[i] != k) {
				++i;
			}
			// cout << "count i " << i << endl;
			if (i == p->key.size()) {
				cout << "not exist" << endl;
			} else {
				for (size_t j = i; j < p->key.size() - 1; ++j) {
					p->key[j] = p->key[j+1];
				}
				// cout << "count here " << i << endl;
				p->key.resize(p->key.size() - 1);
				// cot << "get" << endl;
				// return;
			}
			return ;
		} else {
				cout << "get" << endl;
			size_t i = 0;
			while(i < p->key.size() && k > p->key[i]) {
				++i;
			}
			cout << i << endl;
			
			// case 2
			if (k == p->key[i]) {
				DiskRead(p->kids[i]);
				if (p->kids[i]->key.size() >= t_) {
					// case 2.1
					cout << "case 2.1" << endl;
					auto tmp = Max(p->kids[i]);
					p->key[i] = tmp.first->key[tmp.second];
					// cout << p->key[i] << "kids" <<endl;
					k = p->key[i];
					p = p->kids[i];
					// DebugHook(p->key);
					// DeleteNonone(p->kids[i],p->key[i]);
				} else if (p->kids[i+1]->key.size() >= t_) {
					// case 2.2
					cout << "case 2.2" << endl;
					auto tmp = Min(p->kids[i+1]);
					p->key[i] = tmp.first->key[tmp.second];
					k = p->key[i];
					p = p->kids[i+1];
					// DeleteNonone(p->kids[i+1],p->key[i]);
				} else {
					// case 2.3
					cout << "case 2.3" << endl;
					p = MergeChild(p,i);
					// DeleteNonone(MergeChild(p,i),k);
				}
			} else {
				// case 3
				Node *ptr = p->kids[i];
				// cout << i << " i" << endl;
				if (ptr->key.size() == t_ - 1) {
					// case 3.1
					if (i > 0 && p->kids[i-1]->key.size() > t_ - 1) {
						cout << "case 3.1" << endl;
						ShiftToRight(p,i);
						p = ptr;
						// DeleteNonone(ptr,k);
					} else if (i == 0 && p->kids[i+1]->key.size() > t_ - 1) {
						cout << "case 3.1" << endl;
						ShiftToLeft(p,i);
						p = ptr;
						// DeleteNonone(ptr,k);
					} else {
						// case 3.2
						cout << "case 3.2" << endl;
						Node *tmp = 0;
						if (i > 0) {
							// DeleteNonone(MergeChild(p,i-1),k);
							tmp = MergeChild(p,i-1);
							p = tmp;
							// Print();
							// cout << "a" << endl;
							// DeleteNonone(tmp,k);
						} else {
							DeleteNonone(MergeChild(p,i),k);
							tmp = MergeChild(p,i);
							p = tmp;
							// Print();
							// cout << "b" << endl;
							// DeleteNonone(tmp,k);
						}
					}
				}
				// DeleteNonone(p->kids[i],k);
			}
			DeleteNonone(p,k);
		}
	}


private:
	void DiskRead(Node *p) { p = 0; }
	void DiskWrite(Node *p) { p = 0; }
	Node *AllocateNode(void) { return new Node; }
	void DeleteNode(Node *p) { if (!p) delete p; }
	
	void ShiftToLeft(Node *p,int index) {
		Node *left = p->kids[index];
		Node *right = p->kids[index+1];

		left->key.resize(left->key.size() + 1);
		left->key[left->key.size()-1] = p->key[index];
		
		p->key[index] = right->key[0];
		for (int i = 0; (size_t)i < right->key.size() - 2; ++i) {
			right->key[i] = right->key[i+1];
		}
		right->key.resize(right->key.size() - 1);

		DiskWrite(right);
		DiskWrite(left);
		DiskWrite(p);
	}

	void ShiftToRight(Node *p,int index) {
		Node *left = p->kids[index];
		Node *right = p->kids[index+1];

		right->key.resize(right->key.size() + 1);
		
		for (int i = right->key.size() - 1; i > 0; --i) {
			cout << right->key[i] << endl;
			right->key[i] = right->key[i-1];
		}
		right->key[0] = p->key[index];
		p->key[index] = left->key[left->key.size()-1];
		left->key.resize(left->key.size() - 1);

		DiskWrite(right);
		DiskWrite(left);
		DiskWrite(p);
	}

	// when both child has t_ -1 element
	Node *MergeChild(Node *p,int index) {
		Node *left = p->kids[index];
		Node *right = p->kids[index+1];

		int left_key_size = left->key.size();
		left->key.resize(left_key_size + right->key.size() + 1);
		for (size_t i = 0; i < t_ - 1; ++i) {
			left->key[i+t_] = right->key[i];
		}
		left->key[t_-1] = p->key[index];

		if (!left->isleaf) {
			left->kids.resize(left->kids.size() + t_);
			for (size_t i = 0; i < t_; ++i) {
				left->kids[i+t_] = right->kids[i];
			}
		}

		for (int i = index; (size_t)i < p->key.size(); ++i) {
			p->key[i] = p->key[i+1];
			p->kids[i+1] = p->kids[i+2];
		}
		p->key.resize((p->key.size() - 1));
		p->kids.resize((p->kids.size() - 1));

		DeleteNode(right);

		DiskWrite(right);
		DiskWrite(left);
		DiskWrite(p);

		return left;
	}

	void SplitChild(Node *p,int i) {
		// cout << "SplitChild" << endl;
		Node *z = AllocateNode();
		Node *y = p->kids[i];
		z->isleaf = y->isleaf;
		z->key.resize(t_-1);
		z->kids.resize(t_);
		for (size_t j = 0; j < t_ - 1; ++j) {
			z->key[j] = y->key[j+t_];
		}

		if (!IsLeaf(y)) {
			for (size_t j = 0; j < t_; ++j) {
				z->kids[j] = y->kids[j+t_];
			}
		}

		int tmp = y->key[t_-1];
		y->kids.resize(t_);
		y->key.resize(t_-1);

		p->kids.resize(p->kids.size() + 1);
		p->key.resize(p->key.size() + 1);
		for (int j = p->kids.size()-1; j > i + 1; ++j) {
			p->kids[j] = p->kids[j-1];
		}
		p->kids[i+1] = z;
		for (int j = p->key.size()-1; j > i; ++j) {
			p->key[j] = p->key[j-1];
		}
		p->key[i] = tmp;
		// cout << "SplitChild success" << endl;
		DiskWrite(y);
		DiskWrite(z);
		DiskWrite(p);
		// Print();
	}

	void InsertNonfull(Node *x,int k) {
		if (IsLeaf(x)) {
			int i = x->key.size();
			x->key.resize(x->key.size() + 1,INT_MAX);
			// cout << "IsLeaf" << endl;
			// cout << "size:" << x->key.size() << endl;
			while(i >= 0 && k < x->key[i]) {
				x->key[i+1] = x->key[i];
				--i;
			}
			x->key[i+1] = k;
			// cout << "i+1" << i+1 << endl;
			DiskWrite(x);
		} else {
			int i = x->key.size() - 1;
			// cout << "isnot Leaf" << endl;
			while(i >= 0 && k < x->key[i]) {
				--i;
			}
			++i;
			DiskRead(x->kids[i]);
			if (x->kids[i]->key.size() == (size_t)(2*t_-1)) {
				SplitChild(x,i);
				if (k > x->key[i]) {
					++i;
				}
			}
			InsertNonfull(x->kids[i],k);
			// cout << "Print" << endl;
			// Print();
		}
	}

	size_t t_;
	Node *root_;
};

int main() {
	BTree bt(2);
	vector<int> tb{18, 31, 12, 10, 15, 48, 45, 47, 50, 52, 23, 30, 20};
	vector<int> del{15, 18, 23, 30, 31, 52, 50, 48, 47, 45, 20, 12, 10};
	// for (int i = 0; i < 20; ++i) {
		// bt.Insert(i+1);
	// }
	for (auto a:tb) {
		bt.Insert(a);
	}
	bt.Print();
	cout << endl;
	// for (auto a:del) {
		// cout << "delete " << a << endl;
		// bt.Delete(a);
		// bt.Print();
		// cout << endl;
	// }
	// 
	for (size_t i = 0; i < del.size(); ++i) {
		
		cout << "delete " << del[i] << endl;
		bt.Delete(del[i]);
		bt.Print();
		cout << endl;
	}
	
	bt.Delete(50);
	cout << "get" <<endl;
	bt.DebugHook(del);
		// bt.Print();
	// cout << bt.Min().first->key[bt.Min().second] << endl;
	// cout << bt.Max().first->key[bt.Max().second] << endl;
	// cout << "------------------" << endl;
	// for (int i = 1; i < 16; ++i) {
		// cout << bt.Predesessor(i+1).first->key[bt.Predesessor(i+1).second] << endl;
	// }
		// cout << bt.Predesessor(4).first->key[bt.Predesessor(4).second] << endl;
}
