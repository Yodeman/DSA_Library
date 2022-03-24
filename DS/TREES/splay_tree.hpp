#ifndef MY_SPLAY_TREE
#define MY_SPLAY_TREE

#include <iostream>
#include <memory>
#include <concepts>
#include "iterator.hpp"

template<std::totally_ordered Key, typename Value>
struct SplayTreeNode{
	std::pair<Key, Value> elem;
	std::shared_ptr<SplayTreeNode<Key,Value>> left_child;
	std::shared_ptr<SplayTreeNode<Key,Value>> right_child;
	std::weak_ptr<SplayTreeNode<Key,Value>> parent;
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

#endif //MY_SPLAY_TREE
