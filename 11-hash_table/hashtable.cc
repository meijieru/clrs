#include <climits>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;

class DirectAddressing
{
	public:
		DirectAddressing(size_t size):size_(size){
			index_table_ = new size_t[size];
		}

		~DirectAddressing(){
			delete [] index_table_;
		}

		void Insert(int target){
			index_table_[target] = T_.size();
			T_.push_back(target);
		}

		int *Search(int target) const {
			size_t index = index_table_[target];
			if((index<T_.size())&&T_[index]==target)
				return const_cast<int *>(&T_[index]);
			return nullptr;
		}

		void Delete(int target){
			size_t index = index_table_[target];
			if((index<T_.size())&&T_[index]==target){
				swap(T_.back(),T_[index]);
				T_.pop_back();
			}
			index_table_[T_[index]] = index;
			index_table_[target] = UINT_MAX; 
		}
	private:
		size_t *index_table_;
		size_t size_;
		vector<int> T_;
};

class ChainningHash
{
	public:
		ChainningHash(size_t n,size_t m){
			if(n > p_)
				exit(0);
			size_ = n;
			T_.resize(size_);
			m_ = m;
			a_ = rand()%(p_-1)+1;
			b_ = rand()%p_;
		}
		
		void Insert(int target){
			auto &l = T_[Hash(target)];
			if(l.end()==find(l.begin(),l.end(),target)) // if target already in the list,do nothing
				l.push_front(target);
		}

		int *Search(int target){
			auto &l = T_[Hash(target)];
			auto it = find(l.begin(),l.end(),target);
			if(l.end()==it)
				return nullptr;
			return &*it;
		}

		void Delete(int target){
			auto &l = T_[Hash(target)];
			for(auto it=l.begin(),last=l.before_begin();it!=l.end();it++,last++){
				if(*it==target){
					l.erase_after(last);
					return;
				}
			}
		}

	private:
		int Hash(int target){
			return ((a_*target+b_)%p_)%m_;
			// return target%m_;
		}

		size_t size_;
		vector<forward_list<int>> T_;
		size_t a_;
		size_t b_;
		size_t m_;
		size_t p_ = 10007; // must > size
};

class OpenAddressHash{
	public:
		OpenAddressHash(size_t size):size_(size){
			T_.resize(size);
			for(auto &a:T_)
				a = kNil;
		}

		void Insert(int target){
			for(int i=0;i<m_;i++){
				int pos = Hash(target,i);
				if(T_[pos]==kNil || T_[pos]==kDelete){
					T_[pos] = target;
					return;
				}
			}
			cout << "overflow" << endl;
		}

		int *Search(int target){
			int pos = Hash(target,0);
			for(int i=0;i<m_&&T_[pos]!=kNil;i++){
				pos = Hash(target,i);
				if(T_[pos]==target)
					return &T_[pos];
			}
			return nullptr;
		}

		void Delete(int target){
			for(int i=0;i<m_;i++){
				int pos = Hash(target,i);
				if(T_[pos]==target)
					T_[pos] = kDelete;
			}
		}

	private:
		int Hash1(int target){
			return target%m_;
		}
		
		int Hash2(int target){
			return target%m_;
		}

		int Hash(int target,int i){
			return (Hash1(target)+i*Hash2(target))%m_;
			// return (Hash1(target)+i)%m_;
		}

		const int kNil = INT_MAX;
		const int kDelete = INT_MIN;
		const int m_ = 31;
		int size_;
		vector<int> T_;
};

int main(){
	DirectAddressing direct_addressing(100);
	for(int i=0;i<100;i+=4)
		direct_addressing.Insert(i);
	for(int i=0;i<100;i+=4)
		cout << *direct_addressing.Search(i) << endl;
	direct_addressing.Delete(24);
	direct_addressing.Delete(96);
	int *loc;
	for(int i=0;i<100;i+=4)
		if((loc = direct_addressing.Search(i))!=nullptr)
			cout << *loc << endl;
	cout << "tag1" << endl;


	ChainningHash chainning_hash(21,5);
	for(int i=0;i<100;i+=4)
		chainning_hash.Insert(i);
	for(int i=0;i<100;i+=4)
		cout << *chainning_hash.Search(i) << endl;
	chainning_hash.Delete(96);
	chainning_hash.Delete(24);
	for(int i=0;i<100;i+=4)
		if((loc = chainning_hash.Search(i))!=nullptr)
			cout << *loc << endl;
	cout << "tag2" << endl;

	OpenAddressHash open_address_hash(30);
	for(int i=0;i<100;i+=4)
		open_address_hash.Insert(i);
	for(int i=0;i<100;i+=4)
		cout << *open_address_hash.Search(i) << endl;
	open_address_hash.Delete(24);
	open_address_hash.Delete(96);
	for(int i=0;i<100;i+=4)
		if((loc = open_address_hash.Search(i))!=nullptr)
			cout << *loc << endl;
	cout << "tag3" << endl;


}
