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
		void zig(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		void zig_zig(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		void zig_zag(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);

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
typename std::shared_ptr<SplayTree<Key,Value>::node_type>& SplayTree<Key,Value>::__search(
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

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::restructure(std::shared_ptr<SplayTreeNode<Key,Value>>& node_x)
{
	while ((node_x->parent).lock() != super_root_node) {
		auto parent_node = (node_x->parent).lock();
		auto grand_parent_node = (parent_node->parent).lock();

		// pick the child on the same side as node_x
		auto child_node = (node_x == parent_node->left_child) ? node_x->left_child : node_x->right_child;
		
		// perform zig-zig if both parent_node and node_x are on same side
		if (((parent_node == grand_parent_node->left_child) && (node_x == parent_node->left_child)) || \
			((parent_node == grand_parent_node->right_child) && (node_x == parent_node->right_child)))
		{
			zig_zig(grand_parent_node, parent_node, node_x);
		}
		// perform zig-zag if parent node is left child of grandparent and node_x is right child
		// of parent or if parent is right child of grandparent and node_x is left child of parent.
		else if (((parent_node == grand_parent_node->left_child) && (node_x == parent_node->right_child)) || \
				((parent_node == grand_parent_node->right_child) && (node_x == parent_node->left_child)))
		{
			zig_zag(grand_parent_node, parent_node, node_x);
		}
		else {
			zig(parent_node, node_x, child_node);
		}
	}
}

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::zig_zig(std::shared_ptr<SplayTree<Key,Value>::node_type>& grand_parent_node,
									std::shared_ptr<SplayTree<Key,Value>::node_type>& parent_node,
									std::shared_ptr<SplayTree<Key,Value>::node_type>& node_x
								)
{
	// subtree rooted at each grandparent, parent and node_x in inorder traversal.
	std::shared_ptr<SplayTree<Key,Value>::node_type> T_0, T_1, T_2, T_3;
}

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::zig_zag(std::shared_ptr<SplayTree<Key,Value>::node_type>& grand_parent_node,
									std::shared_ptr<SplayTree<Key,Value>::node_type>& parent_node,
									std::shared_ptr<SplayTree<Key,Value>::node_type>& node_x
								)
{
	// subtree rooted at each grandparent, parent and node_x in inorder traversal.
	std::shared_ptr<SplayTree<Key,Value>::node_type> T_0, T_1, T_2, T_3;
	T_0 = (parent_node == grand_parent_node->right_child) ? grand_parent_node->left_child : parent_node->left_child;
	T_1 = node_x->left_child;
	T_2 = node_x->right_child;
	T_3 = (parent_node == grand_parent_node->right_child) ? parent_node->right_child : grand_parent_node->right_child;

	// replace grandparent with node_x
	if (grand_parent_node == ((grand_parent_node->parent).lock())->left_child)
		((grand_parent_node->parent).lock())->left_child = node_x;
	else
		((grand_parent_node->parent).lock())->right_child = node_x;
	node_x->parent = (grand_parent_node->parent).lock();

	// make grandparent and parent the children of node_x and in right position
	if (node_x == parent_node->left_child) {
		node_x->left_child = grand_parent_node;
		node_x->right_child = parent_node;
		grand_parent_node->parent = node_x;
		parent_node->parent = node_x;
		grand_parent_node->left_child = T_0;
		if (T_0) T_0->parent = grand_parent_node;
		grand_parent_node->right_child = T_1;
		if (T_1) T_1->parent = grand_parent_node;
		parent_node->left_child = T_2;
		if (T_2) T_2->parent = parent_node;
		parent_node->right_child = T_3;
		if (T_3) T_3->parent = parent_node;
	}
	else {
		node_x->left_child = parent_node;
		node_x->right_child = grand_parent_node;
		parent_node->parent = node_x;
		grand_parent_node->parent = node_x;
		parent_node->left_child = T_0;
		if (T_0) T_0->parent = parent_node;
		parent_node->right_child = T_1;
		if (T_1) T_1->parent = parent_node;
		grand_parent_node->left_child = T_2;
		if (T_2) T_2->parent = grand_parent_node;
		grand_parent_node->right_child = T_3;
		if (T_3) T_3->parent = grand_parent_node;
	}
}

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::zig(std::shared_ptr<SplayTree<Key,Value>::node_type>& parent_node,
								std::shared_ptr<SplayTree<Key,Value>::node_type>& node_x,
								std::shared_ptr<SplayTree<Key,Value>::node_type>& child_node
							)
{
	// subtrees rooted at each parent, node_x and child in inorder traversal.
	std::shared_ptr<SplayTree<Key,Value>::node_type> T_0, T_1, T_2, T_3;
	T_0 = (node_x == parent_node->right_child) ? parent->left_child : child_node->left_child;
	T_1 = (child_node == node_x->right_child) ? node_x->left_child : child_node->right_child;
	T_2 = (child_node == node_x->right_child) ? child_node->left_child : node_x->right_child;
	T_3 = (child_node == node_x->right_child) ? child_node->right_child : parent_node->right_child;
	
	// replace parent with node_x
	if (parent_node == ((parent_node->parent).lock())->left_child)
		((parent_node->parent).lock())->left_child = node_x;
	else
		((parent_node->parent).lock())->right_child = node_x;
	node_x->parent = (parent_node->parent).lock();

	// make parent and child the children of node_x and in right position
	if (node_x == parent_node->left_child) {
		node_x->right_child = parent_node;
		node_x->left_child = child_node;
		parent_node->parent = node_x;
		parent_node->left_child = T_2;
		if (T_2) T_2->parent = parent_node;
		parent_node->right_child = T_3;
		if (T_3) T_3->parent = parent_node;
		child_node->left_child = T_0;
		if (T_0) T_0->parent = child_node;
		child_node->right_child = T_1;
		if (T_1) T_1->parent = child_node;
	}
	else {
		node_x->left_child = parent_node;
		node_x->right_child = child;
		parent_node->parent = node_x;
		parent_node->left_child = T_0;
		if (T_0) T_0->parent = parent_node;
		parent_node->right_child = T_1;
		if (T_1) T_1->parent = parent_node;
		child_node->left_child = T_2;
		if (T_2) T_2->parent = child_node;
		child_node->right_child = T_3;
		if (T_3) T_3->parent = child_node;
	}
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
	// splay the newly inserted node.
	if (required_restructuring)
		restructure(new_node);
	return;
}

template<std::totally_ordered Key, typename Value>
void SplayTree<Key,Value>::remove(const Key& key)
{
	auto node = __search(root_node, key);
	if (node) {
		auto parent_node = (node->parent).lock();
		if((node->left != nullptr) && (node->right != nullptr)) {
			// if both children are internal nodes, get the child that follows
			// the current node in the inorder traversal and then replace the
			// current node with that child.
			auto child = node->right_child;
			while(child->left_child)
				child = child->left_child;
			node->elem = child->elem;

			// if the leftmost node has a right child, move the right child's
			// subtree up to occupy the correct position of the about to be
			// removed node.
			auto child_parent = (child->parent).lock();
			if (child->right_child) {
				if(child == child_parent->right_child)
					child_parent->right_child = child->right_child;
				else
					child_parent->left_child = child->right_child;
			}
			else
				(child_parent->right_child).reset();
			// splay the parent of the deleted node.
			restructure(child_parent);

			(child->left_child).reset();
			(child->right_child).reset();
			(child->parent).reset();
			child.reset();
			--sz;
			return;
		}
		else if((node->right_child == nullptr) || (node->left_child == nullptr)) {
			// if one of the children of the  node to be deleted is an internal node
			// move the subree rooted at that child's node up to occupy its parent
			// position.
			auto child = (node-left_child == nullptr) ? node->right_child : node->left_child;
			if (child) {
				if (node == parent_node->left_child)
					parent_node->left_child = child;
				else
					parent_node->right_child = child;
				child->parent = parent_node;
			}
			else {
				if (node == parent_node->left_child)
					parent_node->left_child = nullptr;
				else
					parent_node->right_child = nullptr;
			}
			// splay the parent of the deleted node.
			restructure(parent_node);
			--sz;
			return;
		}
	}
	throw std::runtime_error("entry with key doest not exists in the tree!!!");
}

/*
 * Search for key in the tree, if the key is found at a node,
 * splaying is performed on the node, else the parent of the
 * external node at which the search terminates unsuccessfully
 * is splayed.
*/ 
template<std::totally_ordered Key, typename Value>
typename SplayTree<Key,Value>::iterator SplayTree<Key,Value>::find(const Key& key)
{
	auto node = root_node;
	std::shared_ptr<SplayTreeNode<Key,Value>> parent;
	while(node) {
		if (key < (node->elem).first) {
			parent_node = node;
			node = node->left_child;
		}
		else if (key > (node->elem).first) {
			parent_node = node;
			node = node->right_child;
		}
		else
			break;
	}
	if (node && ((node->elem).first == key)){
		restructure(node);
		SplayTree<Key,Value>::iterator iter(node);
		return iter;
	}
	else {
		restructure(parent_node)
		return end();
	}
}

template<std::totally_ordered Key, typename Value>
typename SplayTree<Key,Value>::iterator SplayTree<Key,Value>::begin()
{
	auto node = root_node;
	while(node->left_child)
		node = node->left_child;
	SplayTree<Key,Value>::iterator iter(node);
	return iter;
}

template<std::totally_ordered Key, typename Value>
typename SplayTree<Key,Value>::iterator SplayTree<Key,Value>::end()
{
	SplayTree<Key,Value>::iterator iter(super_root_node);
	return iter;
}

#endif //MY_SPLAY_TREE
