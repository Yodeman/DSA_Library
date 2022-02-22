#include <iostream>
#include "listException.hpp"

template<typename T>
struct SNode{
    T elem;
    SNode<T>* next;
};

template<typename T>
class SLinkedList;
template<typename T>
std::ostream& operator<<(std::ostream& os, SLinkedList<T>& l);

template<typename T>
class SLinkedList{
    public:
	SLinkedList();
	~SLinkedList();

	bool is_empty() const;
	const T& front() const;
	void addFront(const T&);
	void removeFront();
	size_t size();
	void reverse();
	SNode<T>* header() const { return head; }
	friend std::ostream& operator<< <> (std::ostream& os, SLinkedList<T>& l);
    private:
	size_t sz;
	SNode<T>* head;
};

template<typename T>
SLinkedList<T>::SLinkedList()
{
    head = new SNode<T>{};
    head->next = nullptr;
    sz = 0;
}

template<typename T>
SLinkedList<T>::~SLinkedList()
{
    while (!is_empty()) removeFront();
    delete head;
}

template<typename T>
bool SLinkedList<T>::is_empty() const
{
    return sz==0;
}

template<typename T>
const T& SLinkedList<T>::front() const
{
    if (is_empty()) throw EmptyList("front: Empty list!!!");
    return head->next->elem;
}

template<typename T>
void SLinkedList<T>::addFront(const T& e)
{
    SNode<T>* new_node = new SNode<T>{};
    new_node->elem = e;
    new_node->next = head->next;
    head->next = new_node;
    ++sz;
}

template<typename T>
void SLinkedList<T>::removeFront()
{
    if (is_empty()) throw EmptyList("removeFront: Empty list!!!");
    SNode<T>* u = head->next;
    head->next = u->next;
    delete u;
    --sz;
}

template<typename T>
size_t SLinkedList<T>::size()
{
    return sz;
}

template<typename T>
void SLinkedList<T>::reverse()
{
    if (is_empty()) return;
    SNode<T>* cur = head->next;
    SNode<T>* prv = nullptr;
    SNode<T>* nxt = nullptr;
    while(cur != nullptr){
	nxt = cur->next;
	cur->next = prv;
	prv = cur;
	cur = nxt;
    }
    head->next = prv;
    prv = nxt = cur = nullptr;
    //cur->next = prv;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, SLinkedList<T>& l)
{
    SNode<T>* n = l.head;
    while(n->next != nullptr){
	os << n->next->elem << " ";
	n = n->next;
    }
    return os;
}
