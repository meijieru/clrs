#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>

using namespace std;

/**************** activity select problem **************************/
vector<int> GreedyActivitySelectorRecursive(vector<int> s,vector<int> f,int k,int n) {
	int m = k + 1;
	while ((m <= n) && (s[m] < f[k])) {
		++m;
	}

	if (m <= n) {
		vector<int> tmp = GreedyActivitySelectorRecursive(s,f,m,n);
		tmp.push_back(m);
		return tmp;
	}
	return {};
}

void GreedyActivitySelector(vector<int> s,vector<int> f) {

	int n = f.size();
	vector<int> result{0};

	for (int m = 2,k = 1; m <= n; ++m) {
		if (s[m] >= f[k]) {
			result.push_back(m);
			k = m;
		}
	}

	for (auto a:result) {
		cout << a << " ";
	}
	cout << endl;
}

void GreedyActivitySelectorLast(vector<int> s,vector<int> f) {
	int n = f.size();
	vector<int> index(n);
	vector<int> result{n-1};

	for (int i = 0; i < n; ++i) {
		index[i] = i;
	}
	auto fun = [s,index](int x,int y) { return s[index[x]] < s[index[y]]; };
	/** */
	sort(index.begin(),index.end(),fun);

	int k = n - 1;
	for (int i = n-2; i > 0; --i) {
		if (f[index[i]] <= s[index[k]]) {
			result.push_back(index[i]);
			k = i;
		}
	}

	for (auto a:result) {
		cout << a << " ";
	}
	cout << endl;
}


/************************* Interval graph color problem *****************************/
void IntervalGraphColor(vector<int> s,vector<int> f) {
	enum condition { kStart,kFinish };
	vector<pair<int,condition>> a;

	for (auto t:s) {
		a.push_back({t,kStart});
	}
	for (auto t:f) {
		a.push_back({t,kFinish});
	}

	sort(a.begin(),a.end());
	int max = 0;
	int tmp = 0;
	for (auto t:a) {
		if (t.second == kStart) {
			++tmp;
			if (tmp > max) {
				max = tmp;
			}
		} else {
			--tmp;
		}
	}
	cout << max << endl;
}

/****************************** Fractional knapsack problem ************************/
int Partition(vector<pair<int,int>> &t,int left,int right) {
	// swap(t[rand()%(right-left+1)],t[right]);

	int i = left - 1;
	for (int j = left; j < right; ++j) {
		if(t[j].first < t[right].first) {
			++i;
			swap(t[i],t[j]);
		}
	}

	swap(t[i+1],t[right]);
	return i+1;
}

int Sum(vector<pair<int,int>> t,int left,int right) {
	int sum = 0;
	for (int i = left; i < right + 1; ++i) {
		sum += t[i].second;
	}
	return sum;
}


float FractionalKnapsack(int total, vector<pair<int, int>> &things) {
    int sum_w = 0;
    float supply = 0.0;

    size_t l = 0;
    size_t r = things.size()-1;
    size_t m = 0;

    while (1) {
        if (l != r) {
            m = Partition(things, l, r);
        } else {
            m = l;
        }

		sum_w = Sum(things,0,m-1);
        if (sum_w <= total) {
            if (sum_w + things[m].second > total) {
                supply = float(total - sum_w) / things[m].second * things[m].first;
                break;
            } else if (m == things.size() - 1) {
                supply = things[m].first;
                break;
        } else {
                l = m + 1;
            }
        } else {
            r = m + 1;
        }
    }

    float result = 0.0;
    for (size_t i = 0; i < m; ++i) {
        result += things[i].first;
    }
    result += supply;
    return result;
}

/***************************** huffman code **************************/
typedef struct node {
	node():left(0),right(0),frequence(0),c(0){}
	struct node *left,*right;
	int frequence;
	char c;
}Node;

void Release(Node *p) {
	if (p == nullptr) {
		return;
	} else {
		Release(p->left);
		Release(p->right);
		delete p;
	}
}

void PrintTree(Node *p,int height) {
	if(p != nullptr) {
		PrintTree(p->left,height+1);
		if(p->c != 0) {
			cout << p->frequence <<" " << height << endl;
		}
		PrintTree(p->right,height+1);
	}
}

void Huffman(vector<pair<char,int>> &c) {
	int n = c.size();
	auto fun = [](Node *x,Node *y) { return x->frequence > y->frequence; };

	priority_queue<Node *,vector<Node *>,function<bool(Node *x,Node *y)>> min_heap(fun);
	for (int i = 0; i < n; ++i) {
		Node *inserted = new Node;
		inserted->c = c[i].first;
		inserted->frequence = c[i].second;
		min_heap.push(inserted);
	}

	for (int i = 0; i < n - 1; ++i) {
		Node *inserted = new Node;
		inserted->left = min_heap.top();
		min_heap.pop();
		inserted->right = min_heap.top();
		min_heap.pop();
		inserted->frequence = inserted->left->frequence + inserted->right->frequence;
		min_heap.push(inserted);
	}

	PrintTree(min_heap.top(),0);
	// Release(min_heap.top());
	cout << "free" << endl;

	// return min_heap.top();
}

int main() {
	vector<int> s1 { 0, 1, 3, 0, 5, 3, 5, 6, 8, 8, 2, 12  };
	vector<int> f1 { 0, 4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16 };
	vector<int> s {  1, 3, 0, 5, 3, 5, 6, 8, 8, 2, 12  };
	vector<int> f {  4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16 };
	vector<int> result = GreedyActivitySelectorRecursive(s1,f1,0,11);
	for (auto a = result.rbegin(); a < result.crend(); a++) {
		cout << *a << " ";
	}
	cout << endl;
	GreedyActivitySelector(s,f);
	GreedyActivitySelectorLast(s,f);

	IntervalGraphColor(s,f);

	vector<pair<int,int>> things{{100,20} ,{60,10} ,{120,30}};
	// cout << Partition(things,0,2) << endl;
	cout << FractionalKnapsack(50,things) << endl;

	vector<pair<char,int>> code{ {'f',5}, {'e',9}, {'c',12}, {'b',13}, {'d',16}, {'a',45} };
	Huffman(code);
	// PrintTree(root,0);
	// Release(root);
}
