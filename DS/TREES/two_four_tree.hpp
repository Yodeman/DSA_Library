#ifndef MY_TWO_FOUR_TREE
#define MY_TWO_FOUR_TREE

#include <iostream>
#include <concepts>
#include <valarray>
#include <memory>

template<std::totally_ordered T>
struct TwoFourTreeNode{
	std::array<std::unique_ptr<T>, 4> entries{};
	std::array<std::shared_ptr<TwoFourTreeNode<T>>, 5> children{};
	std::weak_ptr<TwoFourTreeNode<T>> parent;
};

template<std::totally_ordered T>
class TwoFourTree{
	private:
		size_t size() { return sz; }
		bool is_empty() { return sz==0; }
		void insert(const T&);
		void remove(const T&);
		std::unique_ptr<T> find(const T&);

	public:
		size_t sz = 0;
		std::shared_ptr<TwoFourTreeNode<T>> root_node = nullptr;
		void restructure(std::shared_ptr<TwoFourTreeNode<T>>&);
		std::unique_ptr<T> __search(const T&);
};

template<std::totally_ordered T>
std::unique_ptr<T> TwoFourTree<T>::__search(const T&)
{
	// pass.
}

template<std::totally_ordered T>
void TwoFourTree<T>::insert(const T& entry)
{
	// pass.
}

#endif	//MY_TWO_FOUR_TREE
