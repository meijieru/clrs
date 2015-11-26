#include <climits>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <list>

using namespace std;

class LinkGraph {
public:
		enum color {
			kWhite,kGray,kBlack
		};
	struct Edge;
	struct Vertex {
		int x;
		int parent,depth;
		int s,f;
		color cl = kWhite;

		Vertex *Copy(void) {
			Vertex *inserted = new Vertex;
			inserted->x = x;
			inserted->parent = parent;
			inserted->cl = cl;
			inserted->depth = depth;
			inserted->s = s;
			inserted->f = f;
			return inserted;
		}
	};
	struct Edge {
		int start,end;
		int weight;

		Edge *Reverse(void) {
			Edge *inserted = new Edge;
			// cout << "Reverse " << start<< " " << end<< endl;
			inserted->start = end;
			inserted->end = start;
			inserted->weight = weight;
			return inserted;
		}
	};

	LinkGraph(size_t n) {
		int i = 0;
		vertexs_.resize(n);
		for (auto &a:vertexs_) {
			a = new Vertex;
			a->x = i++;
		}
		edges_.resize(n);
	}
	~LinkGraph(void) {
		for (auto &a:edges_) {
			for (auto &b:a) {
				delete b;
			}
		}
		for (auto &a:vertexs_) {
			delete a;
		}
	}

	void BFS(int start) {
		for (auto &a:vertexs_) {
			a->cl = kWhite;
			a->depth = -1;
			a->parent = -1;
		}

		vertexs_[start]->cl = kGray;
		vertexs_[start]->depth = 0;
		vertexs_[start]->parent = -1;

		queue<Vertex *> q;
		q.push(vertexs_[start]);
		while (q.size()) {
			auto tmp = q.front();q.pop();
			for (auto &b:edges_[tmp->x]) {
				if (vertexs_[b->end]->cl == kWhite) {
					vertexs_[b->end]->cl = kGray;
					vertexs_[b->end]->depth = tmp->depth + 1;
					vertexs_[b->end]->parent = tmp->x;
					q.push(vertexs_[b->end]);
				}
			}
			tmp->cl = kBlack;
		}
	}

	void BFSPrint(int start,int end) {
		if (start == end) {
			cout << start;
		} else if (vertexs_[end]->parent != -1) {
			BFSPrint(start,vertexs_[end]->parent);
			cout << " " << end;
		} else {
			cout << "there is no pace from "
				<< start << " to " << end << endl;
		}
	}

	void DFS(void) {
		for (auto &a:vertexs_) {
			// a->s = a->f = -1;
			a->cl = kWhite;
			a->parent = -1;
		}
		int time = 0;
		for (auto &a:vertexs_) {
			if (a->cl == kWhite) {
				DFSVisit(a->x,&time);
			}
		}
	}

	void DFSPrint(void) {
		for (auto &a:vertexs_) {
			cout << "start time:" << a->s
				<< " end time:" << a->f
				<< " parent:" << a->parent
				<< endl;
		}
	}

	int OutDegree(int p) {
		return edges_[p].size();
	}
	int InDegree(int p) {
		int count = 0;
		for (auto &a:edges_) {
			for (auto &b:a) {
				if (b->end == p) {
					++count;
				}
			}
		}
		return count;
	}

	void AddEdge(int s,int e) {
		Edge *inserted = new Edge;
		inserted->start = s;
		inserted->end = e;
		inserted->weight = 1;
		edges_[s].push_back(inserted);
	}

	LinkGraph *Reverse(void) {
		auto tmp = new LinkGraph(vertexs_.size());
		tmp->vertexs_.resize(vertexs_.size());
		tmp->edges_.resize(edges_.size());
		for (size_t i = 0; i < vertexs_.size(); ++i) {
			tmp->vertexs_.push_back(vertexs_[i]->Copy());
		}
		for (size_t i = 0; i < edges_.size(); ++i) {
			for (auto a:edges_[i]) {
				int t = a->end;
				tmp->edges_[t].push_back(a->Reverse());
			}
		}
		return tmp;
	}

	void InsertEdge(int s,int e,int weight) {
		auto inserted = new Edge;
		inserted->start = s;
		inserted->end = e;
		inserted->weight = weight;
		edges_[s].push_back(inserted);
	}

	void DoubleInsertEdge(int s,int e,int weight) {
		InsertEdge(s,e,weight);
		InsertEdge(e,s,weight);
	}

	void Print(void) {
		int i = 0;
		for (auto a:edges_) {
			cout << "start:" << i++ << " ";
			for (auto b:a) {
				cout << b->end << " ";
			}
			cout << endl;
		}
	}

private:

	void DFSVisit(int x,int *time) {
		++*time;
		vertexs_[x]->s = *time;
		vertexs_[x]->cl = kGray;
		for (auto &a:edges_[x]) {
			if (vertexs_[a->end]->cl == kWhite) {
				vertexs_[a->end]->parent = x;
				DFSVisit(a->end,time);
			}
		}
		vertexs_[x]->cl = kBlack;
		++*time;
		vertexs_[x]->f = *time;
	}

	vector<Vertex *> vertexs_;
	vector<list<Edge *>> edges_;
};

int main() {
	LinkGraph lg(10);

	for (int i = 0; i < 4; ++i) {
		lg.InsertEdge(i,i + 1,1);
	}
	for (int i = 0; i < 4; ++i) {
		lg.InsertEdge(i + 5,i + 6,1);
	}
	// lg.InsertEdge(2,5,1);
	// lg.InsertEdge(3,5,1);
	lg.InsertEdge(5,8,1);
	lg.Print();
	cout << "OutDegree:" << lg.OutDegree(4)
	   	<< " InDegree:" << lg.InDegree(4) << endl;
	lg.BFS(2);
	lg.BFSPrint(2,9);
	cout << "-------------------------------" << endl;
	lg.DFS();
	lg.DFSPrint();
	// lg.Reverse()->Print();
}
