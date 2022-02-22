#ifndef __MY_BINARY_TREE
#define __MY_BINARY_TREE

#include <iostream>
#include <memory>
#include <algorithm>
#include <tuple>

template<typename T>
class BTNode{
	public:

		T elem;
		std::shared_ptr<BTNode<T>> parent;
		std::shared_ptr<BTNode<T>> left;
		std::shared_ptr<BTNode<T>> right;

		BTNode():elem{}, parent{nullptr}, left{nullptr}, right{nullptr}
		{}
};

template<typename T>
class BTreeSort{
	public:
		bool operator()(std::tuple<T, size_t, size_t>& a, std::tuple<T, size_t, size_t>& b){
			if (std::get<2>(a) == std::get<2>(b)) {
				return std::get<1>(a) < std::get<1>(b);
			}
			else {
				return std::get<2>(a) < std::get<2>(b);
			}
		}
};

template<typename T>
class BinaryTree{
	public:

		BinaryTree();
		friend class BTreeSort<T>;

		size_t size() const { return sz; }			// total number of nodes in the tree.
		bool empty() const { return sz==0; }
		std::shared_ptr<BTNode<T>> root() const { return rt_node; };
		void insert(const T&);
		void inorder();
		
	private:
		size_t sz;
		size_t x = 0;
		std::vector<std::tuple<T, size_t, size_t>> data;
		std::shared_ptr<BTNode<T>> rt_node;			// root node.

		//utilites
		void __insert(std::shared_ptr<BTNode<T>>, const T&);
		void __inorder(std::shared_ptr<BTNode<T>> node, size_t y=0)
		{
			if(node != nullptr){
				__inorder(node->left, y+1);
				data.push_back(std::make_tuple(node->elem, x, y));
				//std::cout << node->elem << " ";// << " (x=" << x << ", y=" << y << ") ";
				x += 1;
				__inorder(node->right, y+1);
			}
		}
		/*
		bool sort_func(std::tuple<T, size_t, size_t>& first, std::tuple<T, size_t, size_t>& second)
		{
			if(std::get<2>(first) == std::get<2>(second)) {
				return std::get<1>(first) < std::get<1>(second);
			}
			else {
				return std::get<2>(first) < std::get<2>(second);
			}
		}
		*/
};

template<typename T>
BinaryTree<T>::BinaryTree() : sz{0}, rt_node{nullptr}
{}

template<typename T>
void BinaryTree<T>::insert(const T& e)
{
	if (rt_node == nullptr) {
		rt_node = std::make_shared<BTNode<T>>();
		rt_node->elem = e;
		++sz;
	}
	else
		__insert(rt_node, e);
}

template<typename T>
void BinaryTree<T>::__insert(std::shared_ptr<BTNode<T>> node, const T& e)
{
	if(e < node->elem){
		if(node->left == nullptr){
			std::shared_ptr<BTNode<T>> new_node = std::make_shared<BTNode<T>>();
			new_node->elem = e;
			new_node->parent = node;
			node->left = new_node;
			++sz;
			return;
		}
		else
			__insert(node->left, e);
	}
	else{
		if(node->right == nullptr){
			std::shared_ptr<BTNode<T>> new_node = std::make_shared<BTNode<T>>();
			new_node->elem = e;
			new_node->parent = node;
			node->right = new_node;
			++sz;
			return;
		}
		else
			__insert(node->right, e);
	}
}

template<typename T>
void BinaryTree<T>::inorder()
{
	__inorder(rt_node);
	if(x == sz) x = 0;
	BTreeSort<T> sort_func;
	//bool(BinaryTree::*pmf)(std::tuple<T, size_t, size_t>&, std::tuple<T, size_t, size_t>&){&sort_func};
	size_t max_x = std::get<1>(*std::max_element(data.begin(), data.end(),
					[](std::tuple<T, size_t, size_t>& a, std::tuple<T, size_t, size_t>& b){
					return std::get<1>(a) < std::get<1>(b);}
				));
	std::sort(data.begin(), data.end(), sort_func);
	//for(auto& [i, j, k] : data)
	//	std::cout << "( " << i << " " << j << " " << k << ")  ";
	auto iter = data.begin();
	bool inc;
	while(iter != data.end()){
		for (size_t j=0; j < max_x+1; ++j) {
			if (std::get<1>(*iter) == j) {
				std::cout << std::get<0>(*iter);
				if (std::get<2>(*iter) == std::get<2>(*(iter+1))){
					++iter;
					inc = true;
				}
			}
			else {
				std::cout << " ";
				inc = false;
			}
		}
		std::cout << "\n\n";
		if (!inc) ++iter;
	}

}

#endif // _MY_BINARY_TREE
