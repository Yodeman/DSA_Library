#ifndef MY_TWO_FOUR_TREE
#define MY_TWO_FOUR_TREE

#include <iostream>
#include <concepts>
#include <valarray>
#include <memory>

template<std::totally_ordered T>
struct TwoFourTreeNode{
	std::array<std::unique_ptr<T>, 4> entries{};
	std::array<std::shared_ptr<TwoFourTreeNode<T>>, 5> children{};
	std::weak_ptr<TwoFourTreeNode<T>> parent;
};

template<std::totally_ordered T>
class TwoFourTree{
	private:
		size_t size() { return sz; }
		bool is_empty() { return sz==0; }
		void insert(const T&);
		void remove(const T&);
		std::unique_ptr<T> find(const T&);

	public:
		size_t sz = 0;
		std::shared_ptr<TwoFourTreeNode<T>> root_node = nullptr;
		void restructure(std::shared_ptr<TwoFourTreeNode<T>>&);
		std::unique_ptr<T> __search(std::shared_ptr<TwoFourTreeNode<T>>&);
};

template<std::totally_ordered T>
std::shared_ptr<TwoFourTreeNode<T>> TwoFourTree<T>::__search(
			std::shared_ptr<TwoFourTreeNode<T>>& parent_node,
			std::shared_ptr<TwoFourTreeNode<T>>& node,
			const T& entry
		)
{
	if (node) {
		// check the entries of a node to see if it contains the entry
		// of interest.
		for (char i = 0; i<4; ++i) {
			if ((node->entry)[i] && (*((node->entries)[i]) == entry))
				return node;
		}
		// if the node doesn't contain the entry of interest, pick the child
		// node that is most likely to contain the entry of interest.
		for (char i = 0; i < 5; ++i){
			auto child_node = (node->children)[i];
			// since child nodes are ordered in increasing order, once the
			// iteration hits a null child node from left, it stops since
			// its siblings on its right hand side are definitely going to
			// be null.
			if (!child_node) {
				parent_node = node;
				node = child_node;
				break;
			}
			// if the child node contains some entries, since the entries are
			// ordered in increasing order, checks if the last non-null entry
			// in the node is greater than the entry of interest, then, the
			// node is most likely to contain the entry of interest, else,
			// move to the next sibling.
			else if (child_node) {
				auto ri = (child_node->entries).rbegin();
				while ((ri != (child_node->entries).rend()) && (*ri == nullptr)) ++ri;
				if (*ri && *(*ri) >= entry) {
					parent_node = node;
					node = child_node;
					break;
				}
				continue;
			}
		}
		return __search(parent_node, node, entry);
	}
	return parent_node;
}

template<std::totally_ordered T>
void TwoFourTree<T>::insert(const T& entry)
{
	if (sz==0){
		root_node = std::make_shared<TwoFourTreeNode<T>>();
		(root_node->entries)[0] = std::make_unique<T>(entry);
		++sz;
		return;
	}
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	auto swap_idx = -1 // the index that holds the entry greater than the new entry
	for (char i = 0; i < 4; ++i) {
		// do nothing if entry already exists, else insert
		// the new entry.
		if (entries[i] && *(entries[i]) == entry) {
			return;
		}
		else if (!entries[i] || *(entries[i]) > entry) {
			swap_idx = i;
			break;
		}
	}

	if (!entries[swap_idx]) {
		entries[swap_idx] = std::make_unique<T>(entry);
		++sz;
	}
	else {
		for (char i = 3; i > swap_idx; --i)
			entries[i] = std::move(entries[i-1]);
		entries[swap_idx] = std::make_unique<T>(entry);
		++sz;
	}
	// perfom split operation if the number of entries in a node is 4.
	if (entries[3])
		restructure(node);

}

template<std::totally_ordered T>
std::unique_ptr TwoFourTree<T>::find(const T& entry)
{
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	for (auto i=0; i < 3; ++i) {
		if (entries[i] && *(entries[i]) == entry)
			return std::make_unique<T>(entry);
	}

	return nullptr;
}

template<std::totally_ordered T>
void TwoFourTree<T>::remove(const T& entry)
{
	auto node = __search(nullptr, root_node, entry);
	auto entries = node->entries;
	auto idx = -1;
	for (auto i=0; i<3; ++i) {
		if (entries[i] && *(entries[i])==entry) {
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		throw std::runtime_error("the given entry doesn't exists in the tree!!!");
	}
	else {
		for (auto i=idx; i < 3; ++i) {
			entries[i] = std::move(entries[i+1]);
		}
		--sz;
	}
	if (!entries[0])
		restructure(node);
}

template<std::totally_ordered T>
void TwoFourTree<T>::restructure(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	// overflow??
	if ((node->entries)[3]) {
		__resolve_overflow(node);
	}
	// underflow??
	else if (!(node->entries)[0]){
		__resolve_underflow(node);
	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::__resolve_overflow(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	if (node == root_node){
		// new nodes to store entries.
		std::shared_ptr<TwoFourTreeNode<T>> new_node = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> first_node_from_left = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> second_node_from_left = std::make_shared<TwoFourTreeNode<T>>();
		
		// move entries.
		(new_node->entries)[0] = std::move((root_node->entries)[2]);
		(first_node_from_left->entries)[0] = std::move((root_node->entries)[0]);
		(first_node_from_left->entries)[1] = std::move((root_node->entries)[1]);
		(second_node_from_left->entries)[0] = std::move((root_node->entries)[3]);

		(new_node->children)[0]  = first_node_from_left;
		(new_node->children)[1] = second_node_from_left;
		first_node_from_left->parent = new_node;
		second_node_from_left->parent = new_node;
		root_node.reset();
		root_node = new_node;
		return;

	}
	else {
		int entry_idx = node_idx = 3;
		auto parent_node = (node->parent).lock();
		std::shared_ptr<TwoFourTreeNode<T>> first_node = std::make_shared<TwoFourTreeNode<T>>();
		std::shared_ptr<TwoFourTreeNode<T>> second_node = std::make_shared<TwoFourTreeNode<T>>();
		// look for the right position in parent node entries to insert the entry at index 2 of
		// the overflowing child node.
		while(entry_idx > 0) {
			if (((parent_node->entries)[entry_idx]) && (*((parent_node->entry)[entry_idx]) > *((node->entry)[2]))) {
				parent_node->entries[entry_idx+1] = std::move((parent_node->entries)[entry_idx]);
			}
			else if (((parent_node->entries)[entry_idx]) && (*((parent_node->entry)[entry_idx]) < *((node->entry)[2]))) {
				(parent_node->entries)[entry_idx+1] = std::move((node->entries)[2]);
				break;
			}
			--entry_idx;
		}
		
		// create space for the new node that will be inserted in the children of the parent node.
		while(node_idx > 0) {
			if ((parent_node->children)[node_idx] != node)
				(parent_node->children)[node_idx+1] = std::move((parent_node->children)[node_idx]);
			else
				break;
			--node_idx
		}

		(first_node->entries)[0] = std::move((node->entries)[0]);
		(first_node->entries)[1] = std::move((node->entries)[1]);
		(first_node->children)[0] = std::move((node->children)[0]);
		(first_node->children)[1] = std::move((node->children)[1]);
		(first_node->children)[2] = std::move((node->children)[2]);

		(second_node->entries)[0] = std::move((node->entries)[3]);
		(second_node->children)[0] = std::move((node->children)[3]);
		(second_node->children)[1] = std::move((node->children)[4]);

		node.reset();
		(parent_node->children)[node_idx] = first_node;
		(parent_node->children)[node_idx+1] = second_node;
		
		// restructure parent node if it has overflown.
		if ((parent_node->entries)[3])
			__resolve_overflow(parent_node);

	}
}

template<std::totally_ordered T>
void TwoFourTree<T>::__resolve_underflow(std::shared_ptr<TwoFourTreeNode<T>>& node)
{
	// pass
}

#endif	//MY_TWO_FOUR_TREE
