#ifndef MY_RB_TREE
#define MY_RB_TREE

#include <iostream>
#include <concepts>
#include <memory>
#include "iterator.hpp"

template<std::totally_ordered Key, typename Value>
struct RBTreeNode{
	std::pair<Key,Value> entry{};
	std::shared_ptr<RBTreeNode<Key,Value>> left_child = nullptr;
	std::shared_ptr<RBTreeNode<Key,Value>> right_child = nullptr;
	std::weak_ptr<RBTreeNode<Key,Value>> parent;
	bool is_red = true;
};

template<std::totally_ordered Key, typename Value>
class RBTree{
	public:
		using key_type = Key;
		using value_type = Value;
		using iterator = Iterator<std::shared_ptr<RBTreeNode<Key,Value>>>;

		RBTree();

		size_t size() const noexcept { return sz; }
		bool is_empty() const noexcept { return sz==0; }

		void insert(const Key&, const Value&);
		void remove(const Key&);
		iterator find(const Key&);
		iterator begin();
		iterator end();

	protected:
		using node_type = std::shared_ptr<RBTreeNode<Key,Value>>;
		void __insert(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& __search(std::shared_ptr<node_type>&, const Key&);
	private:
		size_t sz;
		// added super root node to aid the implementation of the iterator.
		node_type root_node, super_root_node;
};

template<std::totally_ordered Key, typename Value>
RBTree<Key,Value>::RBTree() : sz{0} {
	root_node = std::make_shared<RBTree<Key,Value>::node_type>();
	super_root_node = std::make_shared<RBTree<Key,Value>::node_type>();
	root_node->parent = super_root_node;
	super_root_node->left_child = root_node;
}

/*
 * Searches for entry with the given key using the inorder traversal
 * algorithm.
*/ 
template<std::totally_ordered Key, typename Value>
std::shared_ptr< typename RBTree<Key,Value>::node_type>& __search(
			std::shared_ptr<RBTree<Key,Value>::node_type>& node,
			const Key& key
		)
{
	if(node) {
		if (key < (node->entry).first)
			return __search(node->left_child, key);
		else if (key > (node->entry).first)
			return __search(node->right_child, key);
		return node;
	}
	return node;
}

#endif //MY_RB_TREE
