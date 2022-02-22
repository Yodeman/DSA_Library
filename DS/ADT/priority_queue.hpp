#include <iostream>
#include "../LINKED_LIST/doubly_linked_list.hpp"

template<typename T, typename Compare=std::less<const T&>>
class PriorityQueue{
	public:
		size_t size() const { return data.size(); }
		size_t is_empty() const { return data.is_empty(); }
		void insert(const T&);
		const T& min() const;
		void remove_min();

		friend std::ostream& operator<<(std::ostream& os, PriorityQueue<T>& p) {
			for(auto i : p.data)
				os << i->elem << " ";
			os << '\n';
			return os;
		}

	private:
		DLinkedList<T> data;
};

template<typename T, typename Compare>
void PriorityQueue<T, Compare>::insert(const T& value)
{
	for (auto i=data.begin(); i!=data.end(); ++i) {
		if (!Compare()(static_cast<const T&>((*i)->elem), value)) {
			data.insertBefore(*i, value);
			return;
		}
	}
	data.addBack(value);
}

template<typename T, typename Compare>
const T& PriorityQueue<T, Compare>::min() const
{
	return data.front();
}

template<typename T, typename Compare>
void PriorityQueue<T, Compare>::remove_min()
{
	data.removeFront();
}
