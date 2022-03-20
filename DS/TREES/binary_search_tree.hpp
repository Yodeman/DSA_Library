#ifndef _MY_BINARY_SEARCH_TREE
#define _MY_BINARY_SEARCH_TREE

#include <iostream>
#include <memory>
#include "iterator.hpp"

template<typename Key, typename Value>
struct BSTNode{
	std::pair<Key, Value> elem;
	std::shared_ptr<BSTNode<Key,Value>> left_child;
	std::shared_ptr<BSTNode<Key,Value>> right_child;
	std::weak_ptr<BSTNode<Key,Value>> parent;

	BSTNode(): elem{}, left_child{nullptr}, right_child{nullptr}, parent{}
	{}
};

template<typename Key, typename Value>
class SearchTree{
	public:
		SearchTree();
		size_t size() const noexcept { return sz; }
		bool is_empty() const noexcept { return size() == 0; }
		void insert(const std::pair<Key,Value>&);
		void remove(const Key&);
		void remove(Iterator<std::shared_ptr<BSTNode<Key,Value>>>&);

		Iterator<std::shared_ptr<BSTNode<Key,Value>>> find(const Key&);
		Iterator<std::shared_ptr<BSTNode<Key,Value>>> begin();
		Iterator<std::shared_ptr<BSTNode<Key,Value>>> end();

	private:
		size_t sz;	// size of the tree
		std::shared_ptr<BSTNode<Key,Value>>& __search(std::shared_ptr<BSTNode<Key,Value>>&, const Key&);
		void __insert(std::shared_ptr<BSTNode<Key,Value>>&, std::shared_ptr<BSTNode<Key,Value>>&);
		std::shared_ptr<BSTNode<Key,Value>> root_node, super_root_node;
		
};

template<typename Key, typename Value>
SearchTree<Key, Value>::SearchTree(): sz{0}{
	// a super root node is created to aid the implementation of
	// iterator to the search tree.
	super_root_node = std::make_shared<BSTNode<Key,Value>>();
	root_node = std::make_shared<BSTNode<Key,Value>>();
	root_node->parent = super_root_node;
	super_root_node->left_child = root_node;
}

template<typename Key, typename Value>
std::shared_ptr<BSTNode<Key,Value>>& SearchTree<Key,Value>::__search(std::shared_ptr<BSTNode<Key,Value>>& node, const Key& key) {
	if(node) {
		if (key < (node->elem).first)
			return __search(node->left_child, key);
		else if (key > (node->elem).first)
			return __search(node->right_child, key);
		else return node;
	}
	return node;
}

template<typename Key, typename Value>
void SearchTree<Key,Value>::__insert(std::shared_ptr<BSTNode<Key,Value>>& parent_node, std::shared_ptr<BSTNode<Key,Value>>& new_node) {
	if((new_node->elem).first < (parent_node->elem).first) {
		if (!(parent_node->left_child)){
			parent_node->left_child = new_node;
			new_node->parent = parent_node;
			return;
		}
		__insert(parent_node->left_child, new_node);
	}
	else {
		if(!(parent_node->right_child)){
			parent_node->right_child = new_node;
			new_node->parent = parent_node;
			return;
		}
		__insert(parent_node->right_child, new_node);
	}
}

template<typename Key, typename Value>
void SearchTree<Key,Value>::insert(const std::pair<Key,Value>& entry){
	if (sz==0) {
		root_node->elem = entry;
		++sz;
		return;
	}
	auto iter = find(entry.first);
	if (iter == end()) {
		auto new_node = std::make_shared<BSTNode<Key,Value>>();
		new_node->elem = entry;
		__insert(root_node, new_node);
		++sz;
		return;
	}
	++sz;
	(*iter).second = entry.second;
	return;
}

template<typename Key, typename Value>
void SearchTree<Key,Value>::remove(const Key& key){
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
			// the it's successor in inorder traversal.
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
void SearchTree<Key,Value>::remove(Iterator<std::shared_ptr<BSTNode<Key,Value>>>& iter) {
	auto key = (*iter).first;
	++iter;
	remove(key);
}

template<typename Key, typename Value>
Iterator<std::shared_ptr<BSTNode<Key,Value>>> SearchTree<Key,Value>::find(const Key& key) {
	auto node = __search(root_node, key);
	if(node) {
		Iterator<std::shared_ptr<BSTNode<Key,Value>>> iter(node);
		return std::move(iter);
	}
	else
		return end();
}

template<typename Key, typename Value>
Iterator<std::shared_ptr<BSTNode<Key,Value>>> SearchTree<Key,Value>::begin(){
	auto n = root_node;
	// the first element is according to the inorder traversal
	// i.e. the leftmost child.
	while(n->left_child)
		n = n->left_child;
	Iterator<std::shared_ptr<BSTNode<Key,Value>>> iter(n);
	return std::move(iter);
}

template<typename Key, typename Value>
Iterator<std::shared_ptr<BSTNode<Key,Value>>> SearchTree<Key,Value>::end(){
	// the end iterator points to the super root sentinel node. This is
	// done inorder to avoid the subtle bug that might occure when the user
	// attempts to increment an iterator pointing the the rightmost node in
	// the tree.
	auto node = (root_node->parent).lock();
	Iterator<std::shared_ptr<BSTNode<Key,Value>>> iter(node);
	return std::move(iter);
}

#endif // _MY_BINARY_SEARCH_TREE
