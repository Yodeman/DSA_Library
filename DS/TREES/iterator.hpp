#ifndef _MY_TREE_ITERATOR
#define _MY_TREE_ITERATOR

#include <iterator>

template<typename T>
class Iterator: public std::iterator<std::forward_iterator_tag, T>{
	public:
		explicit Iterator(T& n): node{n}{}
		Iterator<T>& operator++();
		Iterator<T> operator++(int);
		auto& operator*() { return node->elem; }
		bool operator==(Iterator<T>);
		bool operator!=(Iterator<T>);
	private:
		T node;
};

template<typename T>
Iterator<T> Iterator<T>::operator++(int) {
	Iterator<T> retval{node};
	++(*this);
	return retval;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++(){
	// incrementing the iterator should nove the iterator to the
	// next element in the inorder traversal.
	
	if(node->right_child){
		// if current node has a right child, locates the leftmost
		// child in the current node's right subtree (the leftmost
		// child or the immediate right child of the current node
		// will be the current node's successor in inorder traversal).
		node = node->right_child;
		while(node->left_child) {
				node = node->left_child;
		}
	}
	else {
		// move up the three to locate the yet to be visited node
		// (root node of the subtree).
		T parent_node = (node->parent).lock();
		if (parent_node) {
			while(node == parent_node->right_child) {
				node = parent_node;
				parent_node = (parent_node->parent).lock();
			}
			node = parent_node;
		}
	}
	return *this;
}

template<typename T>
bool Iterator<T>::operator==(Iterator<T> other){
	return (node == other.node);
}

template<typename T>
bool Iterator<T>::operator!=(Iterator<T> other){
	return !(*this == other);
}

#endif // _MY_TREE_ITERATOR
