#ifndef MY_RB_TREE
#define MY_RB_TREE

#include <iostream>
#include <concepts>
#include <memory>
#include "iterator.hpp"

/*
 * Every external nodes i.e. null pointers are black nodes.
*/ 
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
		void __resolve_double_red(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		void __resolve_double_black(std::shared_ptr<node_type>&, std::shared_ptr<node_type>&);
		void __restructure(
					std::shared_ptr<node_type>&, std::shared_ptr<node_type>&, std::shared_ptr<node_type>&,
					bool is_double_red_violation
				);
		void __recolor(
					std::shared_ptr<node_type>&, std::shared_ptr<node_type>&, std::shared_ptr<node_type>&,
					bool is_double_red_violation
				);
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
 * restructures the position of nodes in the tree using the tri-node
 * restructuring algorithm,
*/
template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::__restructure(
			std::shared_ptr<RBTree<Key,Value>::node_type>& grandparent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& parent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& node,
			bool is_double_red_violation
		)
{
	std::shared_ptr<RBTree<Key,Value>::node_type> a, b, c, T0, T1, T2, T3;

	// get the order of the three nodes (i.e. grandparent, parent and node)
	// according to the inorder traversal
	if (parent_node == grandparent_node->right_child) {
		a = grandparent_node;
		if (node == parent_node->right_child) {
			b = parent_node;
			c = node;
		} else {
			b = node;
			c = parent_node;
		}
	} else {
		c = grandparent_node;
		if (node == parent_node->right_child) {
			a = parent_node;
			b = node;
		} else {
			a = node;
			b = parent_node;
		}
	}

	// get the subtrees rooted at grandparent, parent and node
	T0 = a->left_child;
	T1 = (node == b->left_child) ? node->right_child : b->left_child;
	T2 = (node == b->right_child) ? node->left_child : b->right_child;
	T3 = c->right_child;

	// restructure the nodes and the subtrees.
	b->parent = (grandparent_node->parent).lock();
	if (grandparent_node == ((grandparent_node->parent).lock())->right_child) {
		((grandparent_node->parent).lock())->right_child = b;
	} else {
		((grandparent_node->parent).lock())->left_child = a;
	}
	b->right_child = c;
	b->left_child = a;

	if (is_double_red_violation) {
		b->is_red = false;
		a->is_red = true;
		c->is_red = true;
	} else {
		a->is_red = false;
		c->is_red = false;
		b->is_red = grandparent_node->is_red;
	}

	a->parent = b;
	c->parent = b;
	a->left_chile = T0;
	if (T0) T0->parent = a;
	a->right_child = T1;
	if (T1) T1->parent = a;
	c->left_child = T2;
	if (T2) T2->parent = c;
	c->right_child = T3;
	if (T3) T3->parent = c;

	return;
}

/*
 * resolves double red internal property violation by recoloring
 * the nodes and perfoming restructuring.
*/
template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::__recolor(
			std::shared_ptr<RBTree<Key,Value>::node_type>& parent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& sibling,
			bool is_double_red_violation
		)
{
	if ((parent_node == root_node) || ()) {
		parent_node->is_red = false;
		return;
	}

	auto grandparent_node = (parent_node->parent).lock();

	if (is_double_red_violation) {
		node->is_red = false;
		sibling->is_red = false;
		parent_node->is_red  = true;

		// resolve double red internal property violation
		// if it has occured due to recoloring
		if ((grandparent_node->is_red) && (parent_node->is_red))
			__resolve_double_red(grandparent_node, parent_node);
	} else {
		if (node)
			node->is_red = false;
		sibling->is_red = true;
		if (parent_node->is_red) {
			parent_node->is_red = false;
		} else {
			__resolve_double_black(grandparent_node, parent_node);
		}
	}
}

template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::__resolve_double_red(
			std::shared_ptr<RBTree<Key,Value>::node_type>& parent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& node
		)
{
	auto grand_parent = (parent_node->parent).lock();
	auto sibling = (grand_parent->right_child == parent_node) ? grand_parent->left_child : grand_parent->right_child;
	if (!sibling || !(sibling->is_red)) {
		// case 1: the sibling of the parent_node is black.
		__restructure(grand_parent, parent, node, true);
	} else {
		// case 2: the sibling of the parent_node is red.
		__recolor(grand_parent, parent_node, sibling, true);
	}
}

template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::__resolve_double_black(
			std::shared_ptr<RBTree<Key,Value>::node_type>& parent_node,
			std::shared_ptr<RBTree<Key,Value>::node_type>& node
		)
{
	auto sibling = (node == parent_node->right_child) ? parent_node->left_child : parent_node->right_child;

	// case 1: sibling of node is black and has a red child
	if ((sibling && !(sibling->is_red)) &&\
			((sibling->left_child && sibling->left_childe->is_red) ||\
			 (sibling->right_child && sibling->right_child->is_red))) {
		std::shared_ptr<RBTree<Key,Value>::node_type> red_child = nullptr;
		if (sibling->left_child && sibling->left_child->is_red) {
			red_child = sibling->left_child;
		} else {
			red_child = sibling->right_child;
		}
		__restructure(parent_node, sibling, red_child, false);
		if (node)
			node->is_red = false;
		return;
	} else if((!sibling || !(sibling->is_red)) &&\
			((sibling && (sibling->left_child && !(sibling->left_child->is_red))) &&\
			 ((sibling && (sibling->right_child && !(sibling->right_child->is_red)))))) {
		// case 2: sibling of node is black and its children are black
		__recolor(parent_node, node, sibling, false);
	} else if (sibling && sibling->is_red){
		// dase 3: execute readjustment if sibling of node is a red node,
		auto sibling_child = (sibling == parent_node->left_child) ? sibling->left_child : sibling->right_child;
		bool sibling_child_color = !sibling_child ? false : sibling_child->is_red;
		__restructure(parent_node, sibling, sibling_child, false);
		sibling->is_red = false;
		parent_node->is_red = true;
		if (sibling_child)
			sibling_child->is_red = sibling_child_color;
		__resolve_double_black(parent_node, node);
	}
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

	// check and resolve double red internal property violation
	// if it has occured due to the newly inserted node.
	auto parent_node = (node->parent).lock();
	if (parent_node->is_red)
		__resolve_double_red(parent_node, new_node);
}

template<std::totally_ordered Key, typename Value>
void RBTree<Key,Value>::remove(const Key& key)
{
	auto node = __search(root_node, key);
	if (node) {
		auto parent_node = (node->parent).lock();
		if ((node->left_child != nullptr) && (node->right_child != nullptr)){
			// if both children of the current node are internal nodes, replace the
			// current node with the node preceeding it in inorder traversal

			auto child = node->right_child;
			while(child->left_child)
				child = child->left_child;
			node->entry = child->entry;

			// if the child node has a right child, move the right child up to occupy
			// the position of the about to be deleted child node.
			auto child_parent = (child->parent).lock();
			auto child_right_child = child->right_child;
			if (child_right_child) {
				if (child == child_parent->right_child) {
					child_parent->right_child = child_right_child;
				} else {
					child_parent->left_child = child_right_child;
				}
				// check and resolve double black if it has occured due
				// to the removed node.
				if (!(child->is_red && child_right_child->is_red)) {
					child_right_child->is_red = false;
				} else if (!(child->is_red) && !(child_right_child->is_red)) {
					__resolve_double_black(child_parent, child_right_child);
				}
			} else {
				(child_parent->right_child).reset();
				if (!(child->is_red))
					__resolve_double_black(child_parent, child_right_child);
			}
			(child->right_child).reset();
			(child->right_child).reset();
			(child->parent).reset();
			child.reset();
			--sz;
			return;
		} else if ((node->left_child == nullptr) || (node->left_child == nullptr)) {
			// either of the children of the about to be deleted node is an internal
			// node, move the child (the internal node) to occupy its parent position.
			auto child = (node->left_child == nullptr) ? node->right_child : node->left_child;
			if (child) {
				if (node == parent_node->left_child) {
					parent_node->left_child  = child;
				} else {
					parent_node->right_child = child;
				}
				child->parent = parent_node;
				// check and resolve double black if it has occured due
				// to the removed node.
				if (!(node->is_red && child->is_red)) {
					child->is_red = false;
				} else if (!(node->is_red) && !(child->is_red)){
					__resolve_double_black(parent_node, child);
				}
			} else {
				if (node == parent_node->left_child) {
					parent_node->left_child = nullptr;
				} else {
					parent_node->right_child = nullptr;
				}
				if (!(node->is_red))
					__resolve_double_black(parent_node, child);
			}
			(node->left_child).reset();
			(node->right_child).reset();
			(node->parent).reset();
			--sz;
			return;
		}
	}
	throw std::runtime_error("entry with the specified key doesn't exists!!!");
}

template<std::totally_ordered Key, typename Value>
typename RBTree<Key,Value>::iterator RBTree<Key,Value>::find(const Key& key)
{
	auto node = __search(root_node, key);
	if (!node)
		return end();
	RBTree<Key,Value>::iterator iter(node);
	return iter;
}

template<std::totally_ordered Key, typename Value>
typename RBTree<Key,Value>::iterator RBTree<Key,Value>::begin()
{
	auto node = root_node;
	// the first element is the left-most node in the tree.
	while(node->left_child)
		node = node->left_child;
	RBTree<Key,Value>::iterator iter(node);
	return iter;
}

template<std::totally_ordered Key, typename Value>
typename RBTree<Key,Value>::iterator RBTree<Key,Value>::end()
{
	RBTree<Key,Value>::iterator iter(super_root_node);
	return iter;
}

#endif //MY_RB_TREE
