#include <iostream>
#include "iterator.hpp"
#include "../LINKED_LIST/doubly_linked_list.hpp"

template<typename KEY, typename VALUE>
using TYPE = std::shared_ptr<DNode<std::pair<KEY, VALUE>>>;

template<typename KEY, typename VALUE>
class Map{
	public:
		int size() const { return data.size(); }
		bool is_empty() const { return data.is_empty(); }
		Iterator<TYPE<KEY,VALUE>> find(const KEY&);
		Iterator<TYPE<KEY,VALUE>> put(const KEY&, const VALUE&);
		void erase(const KEY&);
		void erase(const Iterator<TYPE<KEY,VALUE>>&);
		Iterator<TYPE<KEY,VALUE>> begin() { return data.begin(); };
		Iterator<TYPE<KEY,VALUE>> end() { return data.end(); };
	private:
		DLinkedList<std::pair<KEY, VALUE>> data;
};

template<typename KEY, typename VALUE>
Iterator<TYPE<KEY,VALUE>> Map<KEY,VALUE>::find(const KEY& k)
{
	auto i = begin(); 
	while(i != end()) {
		if ((*i).first == k)
			break;
		//++i;
		i++;
	}
	return i;
}

template<typename KEY, typename VALUE>
Iterator<TYPE<KEY,VALUE>> Map<KEY,VALUE>::put(const KEY& k, const VALUE& v)
{
	auto i = find(k);
	if (i == end()) {
		data.addBack(std::make_pair(k,v));
		//return --i;
		return i--;
	}
	(*i).second = v;
	return i;

}

template<typename KEY, typename VALUE>
void Map<KEY,VALUE>::erase(const KEY& k)
{
	for(auto i=begin(); i!=end(); ++i){
		if((*i).first == k) {
			data.erase(*i);
			return;
		}
	}
	throw std::runtime_error("key not in storage!!!");
}

template<typename KEY, typename VALUE>
void Map<KEY,VALUE>::erase(const Iterator<TYPE<KEY,VALUE>>& p)
{
	for(auto i=begin(); i!=end(); ++i) {
		if(i==p){
			data.erase(*i);
			return;
		}
	}
	throw std::runtime_error("Iterator not pointing to elements in storage!!!");
}
