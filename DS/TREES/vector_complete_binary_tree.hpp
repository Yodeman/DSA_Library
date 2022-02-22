#ifndef __VCBTREE
#define __VCBTREE

/* Implementation of complete binary tree abstract data
 * data using a dynamic sequence as the underlying data
 * storage. This implementation provides an ammortized
 * O(1) for the add and remove operation. The indices of
 * the nodes is gotten as follows:
 *		if V is root node, V's index = 1
 *		if V is left child of U, V's index = 2 * U's index
 *		if V is right child of U, V's index = (2 * U's index) + 1
 * 
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

template<typename T>
class VCompleteBinaryTree{
	public:
		using node_iter = typename std::vector<T>::iterator;

		VCompleteBinaryTree():data(1){}

		size_t size() const noexcept { return (data.size() - 1); }
		bool is_empty() const noexcept { return size()==0; }
		node_iter left(const node_iter& n) const { return node(2 * index(n));}
		node_iter right(const node_iter& n) const { return node((2 * index(n)) + 1); }
		node_iter parent(const node_iter& n) const { return node(static_cast<size_t>(floor(index(n)/2.0))); }
		bool has_left(const node_iter& n) const noexcept { return (2 * index(n)) <= size(); }
		bool has_right(const node_iter& n) const noexcept { return ((2 * index(n)) + 1) <= size(); }
		bool is_root(const node_iter& n) const noexcept { return index(n)==1; }
		node_iter root() const noexcept { return node(1); }
		node_iter last() noexcept { return node(size()); }
		void add_last(const T& e) noexcept { data.push_back(e); }
		void remove_last() { data.pop_back(); }
		void swap(const node_iter& p, const node_iter& q) { std::swap(*p, *q); }

		friend std::ostream& operator<<(std::ostream& os, VCompleteBinaryTree<T>& v)
		{
			for(auto& i : v.data)
				os << i << " ";
			os << '\n';
			return os;
		}

	protected:
		node_iter node(size_t i) const { return (data.begin() + i); }
		size_t index(const node_iter& p) const { return (p - data.begin()); }

	private:
		mutable std::vector<T> data;
};
#endif // __VCBTREE
