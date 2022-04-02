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
	// do nothing if entry already exists, else insert
	// the new entry.
	for (char i = 0; i < 4; ++i) {
		if (*(entries[i]) == entry)
			return;

	}

}

#endif	//MY_TWO_FOUR_TREE
