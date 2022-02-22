#ifndef __MY_STACK__
#define __MY_STACK__

#include <iostream>
#include "../LINKED_LIST/singly_linked_list.hpp"

namespace mystack{
    class EmptyStack: public std::runtime_error{
        public:
	    EmptyStack(const std::string err): std::runtime_error{err}
	    {}
    };
    
    template<typename T>
    class Stack{
	public:
	    Stack();
	    size_t size() const { return data.size(); };
	    bool empty() const { return data.is_empty(); };
	    const T& top() const;
	    void push(const T&);
	    void pop();
	private:
	    SLinkedList<T> data;
    };

    template<typename T>
    Stack<T>::Stack(): data{}{}

    template<typename T>
    const T& Stack<T>::top() const
    {
		if(empty()) throw EmptyStack("top: Stack is empty!!!");
		return data.front();
    }

    template<typename T>
    void Stack<T>::push(const T& e)
    {
		data.addFront(e);
    }

    template<typename T>
    void Stack<T>::pop()
    {
		if(empty()) throw EmptyStack("pop: Stack is empty!!!");
		data.removeFront();
    }
}; // mystack
#endif // __MY_STACK__
