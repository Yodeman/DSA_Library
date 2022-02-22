#ifndef __MY_DEQUE__
#define __MY_DEQUE__

#include <iostream>
#include "../LINKED_LIST/doubly_linked_list.hpp"

namespace mydeque{
	class EmptyDeque: public std::runtime_error{
		public:
			EmptyDeque(const std::string err): std::runtime_error{err}
			{}
	};

	template<typename T>
	class Deque{
		public:
			Deque();

			size_t size() const { return data.size(); };
			bool empty() const { return data.is_empty(); };
			const T& front() const;
			const T& back() const;
			void eraseBack();
			void eraseFront();
			void insertFront(const T&);
			void insertBack(const T&);

		private:
			DLinkedList<T> data;
	};

	template<typename T>
	Deque<T>::Deque(): data{}{}

	template<typename T>
	const T& Deque<T>::front() const
	{
		if (empty()) throw EmptyDeque("front: Deque is empty!!!");
		return data.front();
	}

	template<typename T>
	const T& Deque<T>::back() const
	{
		if (empty()) throw EmptyDeque("back: Deque is empty!!!");
		return data.back();
	}

	template<typename T>
	void Deque<T>::eraseBack()
	{
		if (empty()) throw EmptyDeque("eraseBack: Deque id empty!!!");
		data.removeBack();
	}

	template<typename T>
	void Deque<T>::eraseFront()
	{
		if (empty()) throw EmptyDeque("eraseFront: Deque is empty!!!");
		data.removeFront();
	}

	template<typename T>
	void Deque<T>::insertFront(const T& e)
	{
		data.addFront(e);
	}

	template<typename T>
	void Deque<T>::insertBack(const T& e)
	{
		data.addBack(e);
	}
}; // mydeque

#endif // __MY_DEQUE__
