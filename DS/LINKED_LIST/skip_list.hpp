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
		void insert(const T&);
		std::unique_ptr<T> find(const T&);
		void remove(const T&);

		friend std::ostream& operator<<(std::ostream& os, const SkipList<T>& s)
		{
			auto n = s.heads[0];
			while(n){
				os << "value: " << n->value << ", level: "
					<< (n->successors).size() << "\n";
				n = n->successors[0];
			}
			return os;
		}
	private:
		size_t sz;
		size_t maxLevel = 0;
		// heads is an array of pointer to the first node in the
		// corresponding level (array index).
		std::vector<std::shared_ptr<SkipListNode<T>>> heads;
		
		size_t __random_level();
		std::shared_ptr<SkipListNode<T>> __skip_search(const T&);
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
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distrib(1);

	size_t level = 1;
	while( (distrib(gen) % 2) == 0)
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
	std::shared_ptr<SkipListNode<T>> e;
	for(auto k=maxLevel; k > 0; --k){
		e = heads[k-1];
		while ((e->successors[k-1]) && (elem >= e->successors[k-1]->value)){
			e = e->successors[k-1];
		}
		--k;
	}
	return e;
}

template<std::totally_ordered T>
std::unique_ptr<T> SkipList<T>::find(const T& elem)
{
	/*
	 * finds the given element in a skip list, if it exists, 
	 * it returns a pointer to the node's value else,
	 * returns a pointer to null.
	*/
	if (sz==0) return nullptr;
	auto e = __skip_search(elem);
	if (e->value == elem)
		return std::make_unique<T>(e->value);
	return nullptr;
}

template<std::totally_ordered T>
void SkipList<T>::insert(const T& elem)
{
	auto e = __skip_search(elem);
	if (e->value == elem) return;

	size_t level = __random_level();	// new node level.
	
	std::shared_ptr<SkipListNode<T>> new_node = std::make_shared<SkipListNode<T>>(elem, level);
	// If level is greater than the current maximum level,
	// insert new level to the head array.
	if (level > maxLevel){
		for (size_t i=level; i > maxLevel; --i)
			heads.push_back(nullptr);
		maxLevel = level;
	}

	std::shared_ptr<SkipListNode<T>> node_before;
	std::shared_ptr<SkipListNode<T>> node_after;
	bool was_set = false; // checks if a new small node was set.

	for(auto k= maxLevel; k > 0; --k) {
		if (!was_set)
			node_before = heads[k-1];
		// Make the newly inserted level point to the new node
		// if new level were inserted to the heads array.
		if (!node_before) {
			heads[k-1] = new_node;
		}
		else {
			node_after = node_before->successors[k-1];
			// If node_before is the immediate node at the current
			// level in the heads array, check if it is greater
			// than the new node's value, if new has that same
			// level, then relink the nodes.
			if (node_before && (node_before->value > elem)) {
				if (k <= level && !was_set){
					new_node->successors[k-1] = node_before;
					heads[k-1] = new_node;
				}
			}
			// moves the node_before to the node with the greatest
			// value that is less than the new node's value.
			while (node_after && (elem >= node_after->value)){
				was_set = true;
				node_before = node_after;
				node_after = node_after->successors[k-1];
			}
			if (k <= level) {
				new_node->successors[k-1] = node_after;
				node_before->successors[k-1] = new_node;
			}
		}
	}
	++sz;
}

template<std::totally_ordered T>
void SkipList<T>::remove(const T& elem)
{
	if(sz==0) throw std::runtime_error("List is empty!!!");
	auto node = __skip_search(elem);
	if (node->value != elem)
		throw std::runtime_error("deletion of elem not in list!!!");
	size_t level = (node->successors).size();

	for (auto k = level; k > 0; --k){
		auto x = heads[k-1];
		auto y = x->successors[k-1];
		while(x) {
			if (y && y->value < elem){
				x = y;
				y = x->successors[k-1];
			}
			else if (y && y->value >= elem)
				break;
		}
		if (y->value == elem){
			x->successors[k-1] = y->successors[k-1];
			(y->successors[k-1]).reset();
		}
	}
	--sz;
	node.reset();
}
/*
template<std::totally_ordered T>
std::ostream& operator<<(std::ostream& os, const SkipList<T> s)
{
	auto n = s.heads[0];
	while (n) {
		os << n->value << " ";
		n = n->successors[0];
	}
	return os;
}
*/
#endif //MY_SKIP_LIST
