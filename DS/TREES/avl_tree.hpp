#ifndef _MY_AVL_TREE
#define _MY_AVL_TREE

#include <iostream>
#include <memory>
#include "iterator.hpp"

template<typename Key, typename Value>
class AVLTree{};

template<typename Key, typename Value>
class AVLNode{
	public:
		friend class AVLTree<Key,Value>;

		std::pair<Key,Value> elem;
		std::shared_ptr<AVLNode<Key,Value>> left_child;
		std::shared_ptr<AVLNode<Key,Value>> right_child;
		std::weak_ptr<AVLNode<Key,Value>> parent;
	protected:
		size_t height;
	private:
		size_t setHeight(size_t h) { height = h; }
};

template<typename Key, typename Value>
class AVLTree{
	public:
		using key_type = Key;
		using value_type = Value;
		using iterator = typename Iterator<std::shared_ptr<AVLNode<Key,Value>>>;
		using node_type = typename AVLNode<Key,Value>;
		
		AVLTree();
		constexpr size_t size() const noexcept { return sz; }
		constexpr bool is_empty() const noexcept { return size() == 0; }
		void insert(const std::pair<Key,Value>&);
		void remove(const Key&);
		void remove(iterator&);
		
		iterator find(const Key&);
		iterator begin();
		iterator end();
	protected:
		void __insert(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& __search(std::shared_ptr<node_type>& const Key&);
		void setHeight(std::shared_ptr<node_type>&);
		size_t height(std::shared_ptr<node_type>&);
		bool isBalanced(std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& tallGrandchild(std::shared_ptr<node_type>&);
		void rebalance(std::shared_ptr<node_type>&);
	private:
		constexpr size_t sz; // size of tree
		std::shared_ptr<node_type> root_node, super_root_node;
};

// constructor
template<typename Key, typename Value>
AVLTree<Key,Value>::AVLTree() : sz{0}
{
	super_root_node = std::make_shared<AVLTree<Key,Value>::node_type>();
	root_node = std::make_shared<AVLTree<Key,Value>::node_type>();
	super_root_node->left_child = root_node;
	root_node->parent = super_root_node;
}

// searches for a node in the tree, returns the
// node if found, else, returns an external node.
template<typename Key, typename Value>
std::shared_ptr<AVLTree<Key,Value>::node_type>& AVLTree<Key,Value>::__search(std::shared_ptr<AVLTree<Key,Value>::node_type>& node, const Key& key)
{
	if(node) {
		if (key < (node->elem).first)
			return __search(node->left_child, key);
		else if (key > (node->elem).first)
			return __search(node->right_child, key);
		return node;
	}
	return node;
}

// inserts a node into the tree, if node already exists,
// it replaces the value of the node in the tree, else,
// inserts the node at the right position.
template<typename Key, typename Value>
void AVLTree<Key,Value>::__insert(std::shared_ptr<AVLTree<Key,Value>::node_type>& parent_node, std::shared_ptr<AVLTree<Key,Value>::node_type>& new_node)
{
	if ((parent_node->elem).first == (new_node->elem).first){
		(parent_node->elem).second = (new_node-elem).second;
		return;
	}
	else if((new_node->elem).first < (parent_node->elem).first) {
		if(!(parent_node->left_child)){
			parent_node->left_child = new_node;
			new_node->parent = parent_node;
			return;
		}
		return __insert(parent_node->left_child, new_node);
	}
	else{
		if(!(parent_node->right_child)){
			new_node->parent = parent_node;
			parent_node->right_child = new_node;
			return;
		}
		return __insert(parent_node->right_child, new_node);
	}
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::insert(const std::pair<Key,Value>& entry)
{
	if(sz==0) {
		root_node->elem = entry;
		++sz;
		return;
	}
	auto new_node = std::make_shared<AVLTree<Key,Value>::node_type>();
	new_node->elem = entry;
	__insert(root_node, new_node);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::remove(const Key& key)
{
	auto node = __search(root_node, key);
	if(node){
		auto parent_node = (node->parent).lock();
		if((node->left_child != nullptr) && (node->right_child != nullptr)){
			// if both children are internal node, get the internal node that followa
			// current node in an inorder traversal and then replace current node
			// with it.
			auto child = node->right_child;
			while(child->left_child)
				child = child->left_child;
			// remove the entry at the current node and replace with entry at
			// it's successor in inorder traversal.
			node->elem = child->elem;
			// if child node has a right child, move the right child's subtree up to
			// occupy the position of the about to be removed child node.
			if (child->right_child){
				auto child_parent = (child->parent).lock();
				if(child == child_parent->right_child)
					child_parent->right_child = child->right_child;
				else
					child_parent->left_child = child->right_child;
				child->right_child->parent = child_parent;
			}
			else
				(((child->parent).lock())->right_child).reset();
			(child->right_child).reset();
			(child->left_child).reset();
			(child->parent).reset();
			child.reset();
			--sz;
			return;
		}
		else if((node->left_child == nullptr) || (node->right_child == nullptr)){
			// if one of the child is an internal node, get the non-external child
			// and move it up to occupy the parent node position.
			auto child = (node->left_child == nullptr) ? node->right_child : node->left_child;
			if(child){
				if(node == parent_node->left_child) {
					parent_node->left_child = child;
				}
				else {
					parent_node->right_child = child;
				}
				child->parent = parent_node;
			}
			else {
				// else remove the node and the parent's reference to it.
				if (node == parent_node->left_child)
					parent_node->left_child = nullptr;
				else
					parent_node->right_child = nullptr;
			}
			node.reset();
			--sz;
			return;
		}
	}
	throw std::runtime_error("attempt to delete element not in storage!!!");
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::remove(AVLTree<Key,Value>::iterator& iter)
{
	auto key = (*iter).first;
	++iter;
	remove(key);
}

template<typename Key, typename Value>
AVLTree<Key,Value>::iterator AVLTree<Key,Value>::find(const Key& key)
{
	auto node = __search(root_node, key);
	if (!node)
		return end();
	AVLTree<Key,Value>::iterator iter(node);
	return iter;
}

template<typename Key, typename Value>
AVLTree<Key,Value>::iterator AVLTree<Key,Value>::begin()
{
	auto n = root_node;
	// the first element is the element that comes first in the
	// inorder traversal of the tree.
	while(n->left_child)
		n = n->left_child;
	AVLTree<Key,Value>::iterator iter(n);
	return iter;
}

template<typename Key, typename Value>
AVLNode<Key,Value>::iterator AVLNode<Key,Value>::end()
{
	// the end iterator points to thw super root sentinel node.
	AVLTree<Key,Value>::iterator iter((root_node->parent).lock());
	return iter;
}
#endif // _MY_AVL_TREE

