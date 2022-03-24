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
			for (auto i=0; i<(s.heads).size(); ++i){
				auto n = s.heads[i];
				os << "level " << i+1 << std::endl;
				while(n){
					os << "\tvalue: " << n->value << ", levels: "
						<< (n->successors).size() << "\n";
					n = n->successors[i];
				}
				os << "\n";
			}
			os << "size: " << s.size() << " maximum level: " << s.max_level() << std::endl;
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
	std::shared_ptr<SkipListNode<T>> node;
	bool was_set = false;
	for(auto k=maxLevel; k > 0; --k){
		if (!was_set) node = heads[k-1];
		while (node && (elem > node->value)){
			if(node->successors[k-1]){
				was_set = true;
				node = node->successors[k-1];
				continue;
			}
			break;
		}
	}
	return node;
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
	//auto e = __skip_search(elem);
	//if (e->value == elem) return;

	size_t level = __random_level();	// new node level.
	
	std::shared_ptr<SkipListNode<T>> new_node = std::make_shared<SkipListNode<T>>(elem, level);
	// If level is greater than the current maximum level,
	// insert new level to the head array.
	if (level > maxLevel){
		for (size_t i=level; i > maxLevel; --i)
			heads.push_back(nullptr);
		maxLevel = level;
	}

	std::shared_ptr<SkipListNode<T>> node_before, node_after;
	bool was_set = false; // checks if a new small node was set during the
						 // traversal of nodes at level, this helps to avoid
						 // the costly linear traversal of nodes at level 1

	for(auto k= maxLevel; k > 0; --k) {
		if (!was_set)
			node_before = heads[k-1];
			// If node_before is the immediate node at the current
			// level in the heads array, check if it is greater
			// than the new node's value, if true and if new node
			// has that same level, then relink the nodes.
			if (node_before && (node_before->value > elem)) {
				if (k <= level){
					new_node->successors[k-1] = node_before;
					heads[k-1] = new_node;
				}
			}
		// Make the newly inserted level point to the new node
		// if new level were inserted to the heads array.
		if (!node_before) {
			heads[k-1] = new_node;
		}
		
		else {
			node_after = node_before->successors[k-1];
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
	
	bool was_set = false;	// check if new node with lesser value
							// was seen during traversal of node at a level
							// this helps to avoid the costly linear traversal
							// of nodes at level 1.
	std::shared_ptr<SkipListNode<T>> node_before, cur_node;
	for (auto k = level; k > 0; --k){
		if (!was_set){
			node_before = heads[k-1];
			cur_node = node_before->successors[k-1];
			// cases where the first node in the heads array is the node
			// to be deleted.
			if (node_before->value == elem) {
				heads[k-1] = cur_node;
				(node_before->successors[k-1]).reset();
				continue;
			}
		}
		//else {
		// move the node before to the greatest node that is
		// less than or equal to the current node's value.
		while(cur_node && (cur_node->value <= elem)) {
			if (cur_node->successors[k-1] && (cur_node->successors[k-1]->value <= elem)){
				was_set = true;
				node_before = cur_node;
				cur_node = cur_node->successors[k-1];
				continue;
			}
			break;
		}
		if (cur_node->value == elem){
			node_before->successors[k-1] = cur_node->successors[k-1];
			(cur_node->successors[k-1]).reset();
		} 
	}
	--sz;
}
#endif //MY_SKIP_LIST
