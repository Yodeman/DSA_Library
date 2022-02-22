#ifndef __HEAP_PQ
#define __HEAP_PQ

#include <iostream>
#include "../TREES/vector_complete_binary_tree.hpp"

template<typename T, typename Comp=std::less<const T&>>
class PriorityQueue{
	public:
		using node_iter = typename VCompleteBinaryTree<T>::node_iter;
		size_t size() const noexcept { return tree.size(); }
		bool is_empty() const noexcept { return tree.is_empty(); }
		void insert(const T& value);
		const T& min() const;
		void remove_min();
		void up_bubble(const node_iter& n);
		void down_bubble(const node_iter& n);

		friend std::ostream& operator<<(std::ostream& os, PriorityQueue<T, Comp>& p)
		{
			os << p.tree;
			return os;
		}
	private:
		mutable VCompleteBinaryTree<T> tree;
};

template<typename T, typename Comp>
void PriorityQueue<T, Comp>::insert(const T& value)
{
	tree.add_last(value);
	up_bubble(tree.last());
}

template<typename T, typename Comp>
const T& PriorityQueue<T, Comp>::min() const
{
	return *(tree.root());
}

template<typename T, typename Comp>
void PriorityQueue<T, Comp>::remove_min()
{
	tree.swap(tree.root(), tree.last());
	tree.remove_last();
	down_bubble(tree.root());
}

template<typename T, typename Comp>
void PriorityQueue<T, Comp>::up_bubble(const node_iter& n)
{
	if(tree.is_root(n) || Comp()(*(tree.parent(n)), *n)) return;
	tree.swap(tree.parent(n), n);
	up_bubble(tree.parent(n));

}

template<typename T, typename Comp>
void PriorityQueue<T,Comp>::down_bubble(const node_iter& n)
{
	if (tree.has_left(n)){
		auto s = tree.left(n);
		if (tree.has_right(n))
			s = (Comp()(*(tree.left(n)),*(tree.right(n)))) ? tree.left(n) : tree.right(n);
		if (!Comp()(*n, *s)){
			tree.swap(n, s);
			down_bubble(s);
		}
	}

}

#endif //__HEAP_PQ
