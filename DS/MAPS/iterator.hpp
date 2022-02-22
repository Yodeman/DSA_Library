#ifndef _MY_ITERATOR
#define _MY_ITERATOR
#include <iterator>
#include <memory>

template<typename T>
class Iterator: public std::iterator<std::bidirectional_iterator_tag, T>{
	public:
		explicit Iterator<T>(T n) : node{n}{}
		Iterator(const Iterator& lhs)=default;
		Iterator& operator=(const Iterator& lhs)=default;
		Iterator<T>& operator++() {
			node = node->next;
			return *this;
		}
		Iterator<T>& operator--() {
			node = (node->prev).lock();
			return *this;
		}
		Iterator<T> operator++(int) {
			Iterator<T> retval(*this);
			++(*this);
			return retval;
		}
		Iterator<T> operator--(int) {
			Iterator<T> retval(*this);
			--(*this);
			return retval;
		}

		bool operator==(Iterator<T> other) {
			return ((*(*this)).first == (*other).first && (*(*this)).second == (*other).second);
		}
		bool operator!=(Iterator<T> other) { return !(*this == other); }
		auto& operator*() { return node->elem; }

	private:
		T node;
};
#endif // _MY_ITERATOR
