#include <climits>
#include <iostream>
#include <stack>

using namespace std;

class bTree{
	public:
		typedef struct node
		{
			struct node *parent;
			struct node *left;
			struct node *right;
			int data;
		}Node;

		bTree():root_(0){}
		~bTree(){
			Release(root_);
		}

		void Insert(int target){
			Node *last(kNil);
			Node *current(root_);	
			while(current!=kNil){
				last = current;
				if(target < current->data){
					current = current->left;
				}else{
					current = current->right;
				}
			}
			Node *inserted = new Node;
			inserted->parent = last;
			inserted->left = kNil;
			inserted->right = kNil;
			inserted->data = target;
			if(!last){
				root_ = inserted;
			}else if(inserted->data < last->data){
				last->left = inserted;
			}else{
				last->right = inserted;
			}
		}

		Node *Search(int target){
			Node *tmp = root_;
			while(tmp!=kNil&&tmp->data!=target){
				if(target<tmp->data){
					tmp = tmp->left;
				}else{
					tmp = tmp->right;
				}
			}
			return tmp;
		}

		Node *Max(void){
			return Max(root_);
		}

		Node *Max(Node *ptr){
			while(ptr->right!=kNil){
				ptr = ptr->right;
			}
			return ptr;
		}

		Node *Min(void){
			return Min(root_);
		}

		Node *Min(Node *ptr){
			while(ptr->left!=kNil){
				ptr = ptr->left;
			}
			return ptr;
		}

		Node *Successor(Node *ptr){
			if(ptr->right!=kNil)
				return Min(ptr->right);
			Node *tmp = ptr->parent;
			while(tmp!=kNil&&tmp->right==ptr){
				ptr = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}

		Node *Predecessor(Node *ptr){
			if(ptr->left!=kNil)
				return Max(ptr->left);
			Node *tmp = ptr->parent;
			while(tmp!=kNil&&tmp->left==ptr){
				ptr = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}

		void Transplant(Node *dst,Node *src){
			if(!dst->parent){
				root_ = src;
			}else if(dst==dst->parent->left){
				dst->parent->left = src;
			}else{
				dst->parent->right = src;
			}
			if(src){
				src->parent = dst->parent;
			}
		}

		void Delete(Node *ptr){
			if(!ptr->left){
				Transplant(ptr,ptr->right);
			}else if(!ptr->right){
				Transplant(ptr,ptr->left);
			}else{
				Node *tmp = Min(ptr->right);
				{
					if(tmp->parent!=ptr){
						Transplant(tmp,tmp->right);
						tmp->right = ptr->right;
						tmp->right->parent = tmp;
					}
					Transplant(ptr,tmp);
					tmp->left = ptr->left;
					tmp->left->parent = tmp;
				}
			}
			delete ptr;
		}
		void Release(Node *ptr){
			if(ptr->left!=kNil)
				Release(ptr->left);
			if(ptr->right!=kNil)
				Release(ptr->right);
			delete ptr;
		}

		void PreOrderTreeWalk(){
			stack<Node *> s;
			s.push(root_);
			while(!s.empty()){
				Node *tmp = s.top();
				s.pop();

				if(tmp!=kNil){
					cout << tmp->data << endl;
					s.push(tmp->left);
					s.push(tmp->right);
				}
			}
		}

		void InOrderTreeWalk(){
			stack<pair<int,Node *>> s;
			const int kLeft=0,kRight=1;
			s.push({kLeft,root_});
			while(!s.empty()){
				auto tmp = s.top();
				s.pop();
				if(!tmp.second){
					// cout << "here" << endl;
					continue;
				}
				if(tmp.first==kLeft){
					s.push({kRight,tmp.second});
					s.push({kLeft,tmp.second->left});
					// cout << "here1" << endl;
				}
				else if(tmp.first==kRight){
					cout << tmp.second->data << endl;
					s.push({kLeft,tmp.second->right});
					// cout << "here2" << endl;
				}
			}
		}

		// void InOrderTreeWalkWithoutStack(void)
		// could add a member visited to the bTree

		void PostOrderTreeWalk(){
			stack<pair<int,Node *>> s;
			const int kLeft=0,kRight=1,kMiddle=2;
			s.push({kLeft,root_});
			while(!s.empty()){
				auto tmp = s.top();
				s.pop();
				if(!tmp.second)
					continue;
				if(tmp.first==kLeft){
					s.push({kRight,tmp.second});
					s.push({kLeft,tmp.second->left});
					// cout << "tag1" << endl;
				}
				else if(tmp.first==kRight){
					s.push({kMiddle,tmp.second});
					s.push({kLeft,tmp.second->right});
					// cout << "tag2" << endl;
				}
				else{
					cout << tmp.second->data << endl;
				}
			}
		}
	private:
		Node *root_;
		Node *kNil = 0;
};

int main()
{
	bTree tree;
	tree.Insert(5);
	tree.Insert(2);
	tree.Insert(8);
	tree.Insert(1);
	tree.Insert(9);
	tree.Insert(3);
	tree.Insert(6);
	tree.Insert(7);
	tree.PreOrderTreeWalk();
	cout << endl;
	tree.PostOrderTreeWalk();
	cout << endl;
	cout << tree.Successor(tree.Search(8))->data << endl;
	cout << tree.Predecessor(tree.Search(8))->data << endl;
	cout << "kk" << endl;
	tree.InOrderTreeWalk();
	cout << "tag1" << endl;
	tree.Delete(tree.Search(5));
	tree.PreOrderTreeWalk();
}
