#ifndef __MY_QUEUE__
#define __MY_QUEUE__

#include <iostream>
#include "../LINKED_LIST/circularly_linked_list.hpp"

namespace myqueue{
    class EmptyQueue: std::runtime_error{
	public:
	    EmptyQueue(const std::string err): std::runtime_error{err}
	    {}
    };

    template<typename T>
    class Queue{
	public:
	    Queue();

	    size_t size() const { return data.size(); };
	    bool empty() const { return data.is_empty(); };
	    const T& front() const;
	    void enqueue(const T&);
	    void dequeue(); 

	private:
	    CLinkedList<T> data;
    };

    template<typename T>
	Queue<T>::Queue(): data{}{}

	template<typename T>
	const T& Queue<T>::front() const
	{
		if (empty()) throw EmptyQueue("front: Queue is empty!!!");
		return data.front();
	}

	template<typename T>
	void Queue<T>::enqueue(const T& e)
	{
		data.add(e);
		data.advance();
	}

	template<typename T>
	void Queue<T>::dequeue()
	{
		if (empty()) throw EmptyQueue("dequeue: Queue is empty!!!");
		data.remove();
	}
}; // myqueue
#endif // __MY_QUEUE
