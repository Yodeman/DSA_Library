#ifndef MY_SPLAY_TREE
#define MY_SPLAY_TREE

#include <iostream>
#include <memory>
#include <concepts>
#include "iterator.hpp"

template<std::totally_ordered Key, typename Value>
struct SplayTreeNode{
	std::pair<Key, Value> elem{};
	std::shared_ptr<SplayTreeNode<Key,Value>> left_child = nullptr;
	std::shared_ptr<SplayTreeNode<Key,Value>> right_child = nullptr;
	std::weak_ptr<SplayTreeNode<Key,Value>> parent = nullptr;
};

template<std::totally_ordered Key, typename Value>
class SplayTree{
	public:
		using key_type = Key;
		using value_type = Value;
		using iterator = Iterator<std::shared_ptr<SplayTreeNode<Key,Value>>;
		using node_type = SplayTreeNode<Key,Value>;
		
		// ctors
		SplayTree();

		// member functions
		size_t size() const noexcept { return sz; }
		bool is_empty() const noexcept { return sz==0; }
		void insert(const std::pair<Key,Value>&);
		void remove(const Key&);
		
		iterator find(const Key&);
		iterator begin();
		iterator end();
	
	protected:
		bool __insert(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& __search(std::shared_ptr<node_type>&, const Key&);
		void restructure(std::shared_ptr<node_type>&);
		void zig(std::shared_ptr<node_type>&);
		void zig_zig(std::shared_ptr<node_type>&);
		void zig_zag(std::shared_ptr<node_type>&);

	private:
		size_t sz;
		std::shared_ptr<node_type> root_node, super_root_node;

};

template<std::totally_ordered Key, typename Value>
SplayTree<Key,Value>::SplayTree() : sz{0}
{
	super_root_node = std::make_shared<SplayTree<Key,Value>::node_type>();
	root_node = std::make_shared<SplayTree<Key,Value>::node_type>();
	super_root_node->left_child = root_node;
	root_node->parent = super_root_node;
}

template<std::totally_ordered Key, typename Value>
std::shared_ptr<SplayTree<Key,Value>::node_type>& SplayTree<Key,Value>::__search(
		std::shared_ptr<SplayTree<Key,Value>::node_type>& node, const Key& key)
{
	if(node) {
		// search the left subtree if key is less
		if (key < (node->elem).first)
			return __search(node->left_child, key);
		// search the right subtree instead
		else if (key > (node->elem).first)
			return __search(node->right_child, key);
	}
	return node;
}

/*
 * inserts a new node into the tree, if a new node was successfully
 * inserted, it returns true to trigger the restructuring method.
*/ 
template<std::totally_ordered Key, typename Value>
bool SplayTree<Key,Value>::__insert(std::shared_ptr<SplayTree<Key,Value>::node_type>& parent_node,
		std::shared_ptr<SplayTree<Key,Value>::node_type>& new_node)
{
	// if the algorithm finds a node with same key, it updates its
	// value with the value of the new node.
	if((parent_node->elem).first == (new_node->elem).first){
		(parent_node->elem).second = (new_node->elem).second;
		return false;
	}
	else if((new_node->elem).first < (parent_node->elem).first){
		if (!(parent_node->left_child)){
			parent_node->left_child = new_node;
			new_node->parent = parent_node;
			++sz;
			return true;
		}
		// search for the right position on the left subtree.
		return __insert(parent_node->left_child, new_node);
	}
	else {
		if(!(parent_node->right_child)) {
			parent_node->right_child = new_node;
			new_node->parent = parent_node;
			++sz;
			return true;
		}
		// search for the right position on the right subtree.
		return __insert(parent_node->right_child, new_node);
	}
}

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::insert(const std::pair<Key,Value>& entry)
{
	if(sz==0) {
		root_node->elem = entry;
		++sz;
		return;
	}
	auto new_node = std::make_shared<SplayTree<Key,Value>::node_type>();
	new_node->elem = entry;
	bool required_restructuring = __insert(root_node, new_node);
	if (required_restructuring)
		restructure(new_node);
	return;
}

#endif //MY_SPLAY_TREE
