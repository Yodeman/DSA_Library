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
		if (key < (node->entry).first) {
			return __search(node->left_child, key);
		} else if (key > (node->entry).first) {
			return __search(node->right_child, key);
		}
		return node;
	}
	return node;
}

/*
 * Inserts a new node into the tree, if the tree contains a node
 * with same key as the new node, it updates the existing node's
 * value with the new one.
*/
template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::__insert(
			std::shared_ptr<RBTree<Key,Value>::node_type> parent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type> new_node
		)
{
	if((parent_node->entry).first == (new_node->entry).first) {
		(parent_node->entry).second = (new_node->entry).second;
		return;
	} else if ((parent_node->entry).first < (new_node->entry).first) {
		if (!(parent_node->left_child)) {
			parent_node->left_child = new_node;
			new_node->parent = parent_node;
			++sz;
			return;
		}
		return __insert(parent_node->left_child, new_node);
	} else {
		if (!(parent_node->right_child)) {
			parent_node->right_child = new_node;
			new_node->parent = parent_node;
			++sz;
			return;
		}
		return __insert(parent_node->right_child, new_node);
	}
}

template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::insert(const Key& key, const Value& val)
{
	std::pair<Key,Value> new_entry {key, val};
	if (sz==0) {
		root_node->entry = new_entry;
		root_node->is_red = false;
		++sz;
		return;
	}
	auto new_node = std::make_shared<RBTree<Key,Value>::node_type>();
	new_node->entry = new_entry;
	__insert(new_node);

	// check and resolve double red if it has occured due to
	// the newly inserted node.
	auto parent_node = (node->parent).lock();
	if (parent_node->is_red)
		__resolve_double_red(parent_node, new_node);
}

#endif //MY_RB_TREE
