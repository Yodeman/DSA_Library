#ifndef __CBTREE
#define __CBTREE

/* Implementation of complete binary tree abstract data
 * structure.
 * Author: pauli
*/
#include <iostream>
#include <memory>

template<typename T>
class CBTNode{
	public:
		T elem;
		std::shared_ptr<CBTNode<T>> left;
		std::shared_ptr<CBTNode<T>> right;
		std::shared_ptr<CBTNode<T>> parent;

		CBTNode<T>(): elem{}, left{nullptr}, right{nullptr}, parent{nullptr}
		{}
};

template<typename T>
class CompleteBinaryTree{
	public:
		size_t size() const noexcept { return sz; }
		bool empty() const noexcept { return sz==0; }
		std::shared_ptr<CBTNode<T>> left(const std::shared_ptr<CBTNode<T>> n);
		std::shared_ptr<CBTNode<T>> right(const std::shared_ptr<CBTNode<T>> n);
		std::shared_ptr<CBTNode<T>> parent(const std::shared_ptr<CBTNode<T>> n);
		bool hasLeft(const std::shared_ptr<CBTNode<T>> n) const noexcept;
		bool hasRight(const std::shared_ptr<CBTNode<T>> n) const noexcept;
		bool isRoot(const std::shared_ptr<CBTNode<T>> n) const noexcept;
		std::shared_ptr<CBTNode<T>> root() noexcept;
		std::shared_ptr<CBTNode<T>> last() noexcept;
		void addLast(const T& e) noexcept;
		void removeLast();
		void swap(const std::shared_ptr<CBTNode<T>> p, const std::shared_ptr<CBTNode<T>> q);

	private:
		size_t sz;
		std::shared_ptr<CBTNode<T>> rt_node;
};
#endif // __CBTREE
