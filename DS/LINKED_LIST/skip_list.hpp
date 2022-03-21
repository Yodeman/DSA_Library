#ifndef MY_SKIP_LIST
#define MY_SKIP_LIST

#include <iostream>
#include <concepts>
#include <memory>
#include <vector>
#include <random>
#include <limits>

template<std::totally_ordered T>
struct SkipListNode{
	T value;
	std::vector<std::shared_ptr<SkipListNode<T>>> successors;

	SkipListNode(T elem, size_t levels) :
		value{elem}, successors(levels, nullptr) {
		successors.shrink_to_fit();
	}
};

template<std::totally_ordered T>
class SkipList{
	public:
		size_t size() const noexcept { return sz; }
		bool is_empty() const noexcept { return sz==0; }
		size_t max_level() const noexcept { return maxLevel; }
		void insert(const T& elem);
		std::shared_ptr<SkipListNode<T>> find(const T& elem);
		void remove(const T& elem);
	private:
		size_t sz;
		size_t maxLevel;
		// heads is an array of pointer to the first node in the
		// corresponding level (array index).
		std::vector<std::shared_ptr<SkipListNode<T>>> heads;
		
		std::mt19937 gen();
		std::uniform_int_distribution<size_t> distribution(1);
		size_t __random_level();
		std::shared_ptr<SkipListNode<T>> __skip_search(const T& elem);
};

template<std::totally_ordered T>
size_t SkipList<T>::__random_level()
{
	/*
	 * Simulates the head or tail probability using odd or even
	 * value returned by the random number generator.
	 * Randomly choose the level of a new node by incrementing
	 * level if the returned value is even and stops if the
	 * returned value is odd or max level is reached.
	*/
	size_t level = 1;
	while( (distribution(gen) % 2) == 0)
		++level;
	return level;
}

template<std::totally_ordered T>
std::shared_ptr<SkipListNode<T>> SkipList<T>::__skip_search(const T& elem)
{
	/*
	 * Searches for an element in a skip list, it returns the element
	 * if found, else returns the largest elem that is less than
	 * or equal to elem
	*/
	size_t k = maxLevel;
	auto e = heads[k];
	while(k >= 0){
		while (elem >= e->successors[k]->value){
			e = e->successors[k];
		}
		--k;
	}
	return e;
}

template<std::totally_ordered T>
std::shared_ptr<SkipListNode<T>> find(const T& elem)
{
	if (sz==0) return nullptr;
	auto e = __skip_search(elem);
	if (e->value == elem)
		return e;
	return nullptr;
}

template<std::total_ordered T>
void SkipList<T>::insert(const T& elem)
{
	size_t level = __random_level();	// new node level.
	
	std::shared_ptr<SkipListNode<T>> new_node = std::make_shared<SkipListNode<T>>(elem, level);
	// If level is greater than the current maximum level,
	// insert new level to the head array.
	--level;
	if (level > maxLevel){
		for (size_t i=level; i > maxLevel; --i)
			heads.push_back(nullptr);
		maxLevel = level;
	}

	std::shared_ptr<SkipListNode<T>> x;
	std::shared_ptr<SkipListNode<T>> z;
	bool was_set = false; // checks if a new small node was set.

	for(auto k=maxLevel; k >= 0; --k) {
		if (!was set)
			x = heads[k];
		// Make the newly inserted level point to the new node
		// if new level were inserted to the heads array.
		if (!x) {
			heads[k] = new_node;
		}
		else {
			z = x->successors[k];
			if((z && (z->value >= elem)) or !z){
				if (k <= level){
					new_node->successors[k] = z;
					x->successors[k] = new_node;
				}
			}
			else{
				x = z;
				was_set = true;
				if (k <= level){
					new_node->successors[k] = z->successors[k];
					z->successors[k] = new_node;
				}					
			}
		}
	}
}

template<std::totally_ordered T>
void SkipList<T>::remove(const T& elem)
{
	if(sz==0) throw std::runtime_error("List is empty!!!");
	auto& node = __skip_search(elem);
	if (node->value != elem)
		throw std::runtime_error("deletion of elem not in list!!!");
	size_t levels = (node->successors).size() - 1;

	for (auto k = levels; k >= 0; --k){
		auto x = heads[k];
		auto y = x->successors[k];
		while(x) {
			if (y && y->value < elem){
				x = y;
				y = x->successors[k];
			}
			else if (y && y->value >= elem)
				break
		}
		if (y->value == elem){
			x->successors[k] = y->successors[k];
			(y->successors[k]).reset();
		}
	}
	node.reset();
}
#endif //MY_SKIP_LIST
