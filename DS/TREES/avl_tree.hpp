#ifndef _MY_AVL_TREE
#define _MY_AVL_TREE

#include <iostream>
#include <memory>
#include "iterator.hpp"

template<typename Key, typename Value>
class AVLTree;

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
		void setHeight(size_t h) { height = h; }
};

template<typename Key, typename Value>
class AVLTree{
	public:
		using key_type = Key;
		using value_type = Value;
		using iterator = Iterator<std::shared_ptr<AVLNode<Key,Value>>>;
		using node_type = AVLNode<Key,Value>;
		
		AVLTree();
		size_t size() const noexcept { return sz; }
		bool is_empty() const noexcept { return size() == 0; }
		void insert(const std::pair<Key,Value>&);
		void remove(const Key&);
		void remove(iterator&);
		
		iterator find(const Key&);
		iterator begin();
		iterator end();
	protected:
		bool __insert(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& __search(std::shared_ptr<node_type>&, const Key&);
		void setHeight(std::shared_ptr<node_type>&);
		size_t height(std::shared_ptr<node_type>&);
		bool isBalanced(std::shared_ptr<node_type>&);
		std::shared_ptr<node_type>& tallGrandchild(std::shared_ptr<node_type>&);
		void rebalance(std::shared_ptr<node_type>&);
		std::shared_ptr<node_type> restructure(std::shared_ptr<node_type>&);
	private:
		size_t sz; // size of tree
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

// returns the height of a node.
template<typename Key, typename Value>
size_t AVLTree<Key,Value>::height(std::shared_ptr<AVLTree<Key,Value>::node_type>& node)
{
	return (!node) ? 0 : node->height;
}

// set new height for a node.
template<typename Key, typename Value>
void AVLTree<Key,Value>::setHeight(std::shared_ptr<AVLTree<Key,Value>::node_type>& node)
{
	if (node){
		size_t left_child_height = height(node->left_child);
		size_t right_child_height = height(node->right_child);
		node->setHeight(1 + ((left_child_height > right_child_height) ? left_child_height : right_child_height));
	}
}

// checks if a node is balanced i.e. the absolute difference of the
// height of its left and right children is 1 or 0.
template<typename Key, typename Value>
bool AVLTree<Key,Value>::isBalanced(std::shared_ptr<typename AVLTree<Key,Value>::node_type>& node)
{
	int abs_diff = std::abs(static_cast<int>(height(node->left_child) - height(node->right_child)));
	return (abs_diff==0 or abs_diff==1);
}

// returns the tallest grandchild of a node.
template<typename Key, typename Value>
std::shared_ptr<typename AVLTree<Key,Value>::node_type>& AVLTree<Key,Value>::tallGrandchild(std::shared_ptr<AVLTree<Key,Value>::node_type>& node_z)
{
	auto l_node = node_z->left_child;
	auto r_node = node_z->right_child;
	if (height(l_node) >= height(r_node))
		return (height(l_node->left_child) >= height(l_node->right_child)) ? l_node->left_child : l_node->right_child;
	return (height(r_node->left_child) >= height(r_node->right_child)) ? r_node->left_child : r_node->right_child;
}

// rebalances the nodes in the tree.
template<typename Key, typename Value>
void AVLTree<Key,Value>::rebalance(std::shared_ptr<AVLTree<Key,Value>::node_type>& node)
{
	auto node_z = node;
	while((node_z->parent).lock() != super_root_node) {
		node_z = (node_z->parent).lock();
		setHeight(node_z);
		if (!isBalanced(node_z)) {
			auto node_x = tallGrandchild(node_z);
			node_z = restructure(node_x);
			setHeight(node_z->left_child);
			setHeight(node_z->right_child);
			setHeight(node_z);
		}
	}
	root_node = node_z;
}

// restructures a sub-tree using the tri-node restructure method.
template<typename Key, typename Value>
std::shared_ptr< typename AVLTree<Key,Value>::node_type> AVLTree<Key,Value>::restructure(std::shared_ptr<AVLTree<Key,Value>::node_type>& node)
{
	auto node_x = node;
	auto node_y = (node_x->parent).lock();
	auto node_z = (node_y->parent).lock();
	std::shared_ptr<AVLTree<Key,Value>::node_type> a, b, c, T_0, T_1, T_2, T_3;

	// get the order of the nodes according to inorder traversal.
	if (node_y == node_z->right_child){
		a = node_z;
		if (node_x == node_y->right_child){
			b = node_y;
			c = node_x;
		}
		else {
			b = node_x;
			c = node_y;
		}
	}
	else {
		c = node_z;
		if (node_x == node_y->right_child){
			a = node_y;
			b = node_x;
		}
		else{
			a = node_x;
			b = node_y;
		}
	}
	// the subtrees.
	T_0 = a->left_child;
	T_1 = (node_x == b->left_child) ? node_x->right_child : b->left_child;
	T_2 = (node_x == b->right_child) ? node_x->left_child : b->right_child;
	T_3 = c->right_child;
	
	b->parent = (node_z->parent).lock();
	if(node_z == ((node_z->parent).lock())->right_child)
		((node_z->parent).lock())->right_child = b;
	else
		((node_z->parent).lock())->left_child = b;
	b->right_child = c;
	b->left_child = a;
	a->parent = b;
	c->parent = b;
	a->left_child = T_0;
	if (T_0) T_0->parent = a;
	a->right_child = T_1;
	if (T_1) T_1->parent = a;
	c->left_child = T_2;
	if (T_2) T_2->parent = c;
	c->right_child = T_3;
	if (T_3) T_3->parent = c;
	return node_z;

}

// searches for a node in the tree, returns the
// node if found, else, returns an external node.
template<typename Key, typename Value>
std::shared_ptr<typename AVLTree<Key,Value>::node_type>& AVLTree<Key,Value>::__search(std::shared_ptr<AVLTree<Key,Value>::node_type>& node, const Key& key)
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
bool AVLTree<Key,Value>::__insert(std::shared_ptr<AVLTree<Key,Value>::node_type>& parent_node, std::shared_ptr<AVLTree<Key,Value>::node_type>& new_node)
{
	if ((parent_node->elem).first == (new_node->elem).first){
		(parent_node->elem).second = (new_node->elem).second;
		return false;
	}
	else if((new_node->elem).first < (parent_node->elem).first) {
		if(!(parent_node->left_child)){
			parent_node->left_child = new_node;
			new_node->parent = parent_node;
			setHeight(new_node);
			++sz;
			return true;
		}
		return __insert(parent_node->left_child, new_node);
	}
	else{
		if(!(parent_node->right_child)){
			new_node->parent = parent_node;
			parent_node->right_child = new_node;
			setHeight(new_node);
			++sz;
			return true;
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
		setHeight(root_node);
		return;
	}
	auto new_node = std::make_shared<AVLTree<Key,Value>::node_type>();
	new_node->elem = entry;
	bool required_rebalance = __insert(root_node, new_node);
	if (required_rebalance)
		rebalance(new_node);
	return;
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

			auto child_parent = (child->parent).lock();
			if (child->right_child){
				if(child == child_parent->right_child)
					child_parent->right_child = child->right_child;
				else
					child_parent->left_child = child->right_child;
				child->right_child->parent = child_parent;
			}
			else
				(((child->parent).lock())->right_child).reset();
			rebalance(child_parent);
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
				rebalance(child);
			}
			else {
				// else remove the node and the parent's reference to it.
				if (node == parent_node->left_child)
					parent_node->left_child = nullptr;
				else
					parent_node->right_child = nullptr;
				rebalance(parent_node);
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
typename AVLTree<Key,Value>::iterator AVLTree<Key,Value>::find(const Key& key)
{
	auto node = __search(root_node, key);
	if (!node)
		return end();
	AVLTree<Key,Value>::iterator iter(node);
	return iter;
}

template<typename Key, typename Value>
typename AVLTree<Key,Value>::iterator AVLTree<Key,Value>::begin()
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
typename AVLTree<Key,Value>::iterator AVLTree<Key,Value>::end()
{
	// the end iterator points to thw super root sentinel node.
	AVLTree<Key,Value>::iterator iter(super_root_node);
	return iter;
}
#endif // _MY_AVL_TREE

