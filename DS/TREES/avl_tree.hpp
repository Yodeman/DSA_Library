#ifndef _MY_AVL_TREE
#define _MY_AVL_TREE

#include <iostream>
#include <memory>

template<typename Key, typename Value>
class AVLTree{};

template<typename Key, typename Value>
class AVLNode{
	public:
		friend class AVLTree<Key,Value>;

		std::pair<Key,Value> elem;
		left_child;
		right_child;
		std::weak_ptr<AVLNode<Key,Value>> parent;
	protected:
		size_t height;
	private:
		size_t getHeight(){ return height; }
		size_t setHeight(size_t h) { height+= h; }

};

#endif // _MY_AVL_TREE

