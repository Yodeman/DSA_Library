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
		using iterator = Iterator<std::shared_ptr<AVLNode<Key,Value>>>;
		using node_type = AVLNode<Key,Value>
		
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

#endif // _MY_AVL_TREE

