#include <iostream>
#include <memory>
#include "listException.hpp"
#include "../MAPS/iterator.hpp"

template<typename T>
struct DNode{
    T elem;
	std::weak_ptr<DNode<T>> prev;
	std::shared_ptr<DNode<T>> next;

    DNode<T>(): elem{}, prev{}, next{nullptr}{}
};


template<typename T>
class DLinkedList{
    public:
		DLinkedList();
		~DLinkedList();

		bool is_empty() const;
		const T& front() const;
		const T& back() const;
		void addFront(const T&);
		void addBack(const T&);
		void removeFront();
		void removeBack();

		/* erase function is to be used with sparingly
		 * as it first ensures that the element to be
		 * deleted is in the list (the cost is high
		 * if list is large)
		 */
		void erase(T n);
		void insertBefore(std::shared_ptr<DNode<T>> n, const T& e);
		void insertAfter(std::shared_ptr<DNode<T>> n, const T& e);
		Iterator<std::shared_ptr<DNode<T>>> begin() {
			Iterator<std::shared_ptr<DNode<T>>> temp(header->next);
			return temp;
		}
		Iterator<std::shared_ptr<DNode<T>>> end() {
			Iterator<std::shared_ptr<DNode<T>>> temp(trailer);
			return temp;
		}
		size_t size() const { return sz; }

    protected:
		void add(std::shared_ptr<DNode<T>>, const T&);
		void remove(std::shared_ptr<DNode<T>>);

    private:
		size_t sz;
		std::shared_ptr<DNode<T>> header;
		std::shared_ptr<DNode<T>> trailer;
};

template<typename T>
DLinkedList<T>::DLinkedList()
{
    sz = 0;
    header = std::make_shared<DNode<T>>();
    trailer = std::make_shared<DNode<T>>();
    header->next = trailer;
    trailer->prev = header;
}

template<typename T>
DLinkedList<T>::~DLinkedList()
{
    while(!is_empty()) removeFront();
    header.reset(); //delete header;
    trailer.reset(); //delete trailer;
}

template<typename T>
bool DLinkedList<T>::is_empty() const
{
    return sz==0;
}

template<typename T>
const T& DLinkedList<T>::front() const
{
    if (is_empty()) throw EmptyList("front: Empty list!!!");
    return header->next->elem;
}

template<typename T>
const T& DLinkedList<T>::back() const
{
    if (is_empty()) throw EmptyList("front: Empty list!!!");
    auto ptr = (trailer->prev).lock();
    return ptr->elem;
}

template<typename T>
void DLinkedList<T>::addFront(const T& e)
{
    add(header, e);
}

template<typename T>
void DLinkedList<T>::addBack(const T& e)
{
    add((trailer->prev).lock(), e);
}

template<typename T>
void DLinkedList<T>::insertBefore(std::shared_ptr<DNode<T>> n, const T& e)
{
	add((n->prev).lock(), e);
}

template<typename T>
void DLinkedList<T>::insertAfter(std::shared_ptr<DNode<T>> n, const T& e)
{
	add(n, e);
}

template<typename T>
void DLinkedList<T>::removeFront()
{
    if (is_empty()) throw EmptyList("removeFront: Empty list!!!");
    remove(header->next);
}

template<typename T>
void DLinkedList<T>::removeBack()
{
    if (is_empty()) throw EmptyList("removeBack: Empty list!!!");
    remove((trailer->prev).lock());
}

template<typename T>
void DLinkedList<T>::add(std::shared_ptr<DNode<T>> n, const T& e)
{
	std::shared_ptr<DNode<T>> new_node = std::make_shared<DNode<T>>();
    new_node->elem = e;
    new_node->next = n->next;
    n->next->prev = new_node;
    n->next = new_node;
    new_node->prev = n;
    ++sz;
}

template<typename T>
void DLinkedList<T>::remove(std::shared_ptr<DNode<T>> n)
{
    ((n->prev).lock())->next = n->next;
    n->next->prev = n->prev;
    n.reset(); //delete n;
    --sz;
}

template<typename T>
void DLinkedList<T>::erase(T n)
{
	auto node = header->next;
	while (node->next) {
		if (node->elem == n) {
			remove(node);
			return;
		}
		node = node->next;
	}
}
