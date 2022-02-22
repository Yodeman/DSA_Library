#include <iostream>
#include "listException.hpp"

template<typename T>
struct CNode{
    T elem;
    CNode<T>* next;
};

template<typename T>
class CLinkedList{
    public:
	CLinkedList();
	~CLinkedList();
	
	bool is_empty() const { return sz==0; };
	const T& front() const;	// element after cursor
	const T& back() const;	// elememt at cursor
	void advance();
	void add(const T&);
	void remove();	// remove node after cursor
	size_t size() { return sz; }

    private:
	CNode<T>* cursor;
	size_t sz;
};

template<typename T>
CLinkedList<T>::CLinkedList(): sz{0}, cursor{nullptr}{}

template<typename T>
CLinkedList<T>::~CLinkedList()
{
    while(!is_empty()) remove();
    delete cursor;
}

template<typename T>
const T& CLinkedList<T>::front() const
{
    if(is_empty()) throw EmptyList("front: Empty list!!!");
    return cursor->next->elem;
}

template<typename T>
const T& CLinkedList<T>::back() const
{
    if (is_empty()) throw EmptyList("back: Empty list!!!");
    return cursor->elem;
}

template<typename T>
void CLinkedList<T>::advance()
{
    if (is_empty()) throw EmptyList("advance: Empty list!!!");
    cursor = cursor->next;
}

template<typename T>
void CLinkedList<T>::add(const T& e)
{
    CNode<T>* new_node = new CNode<T>{};
    new_node->elem = e;
    if (is_empty()){
	new_node->next = new_node;
	cursor = new_node;
    }
    else{
	new_node->next = cursor->next;
	cursor->next = new_node;
    }
    ++sz;
}

template<typename T>
void CLinkedList<T>::remove()
{
    if (is_empty()) throw EmptyList("remove: Empty list!!!");
    CNode<T>* old = cursor->next;
    if (old == cursor) cursor = nullptr;
    else
	cursor->next = old->next;
    delete old;
    --sz;
}
