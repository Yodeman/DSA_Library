#ifndef MY_QUICK_SELECT
#define MY_QUICK_SELECT

/*
 * Implementation of deterministic quick select algorithm, which enforces the
 * algorithm to run in O(n) time.
*/

#include <iostream>
#include <concepts>
#include "../sorting/quick_sort.hpp"

template<typename Container>
concept quick_select_requirement = std::input_or_output_iterator<Container::iterator> &&\
								   std::totally_ordered<Container::value_type>;

template<
			quick_select_requirement Container,
			typename T=Container::value_type
		>
T quick_select (Container& C, size_t K)
{
	auto size = std::distance(C.begin(), C.end());

	if (size <= 10) {
		quick_sort(C);
		return C[K-1];
	}

	std::vector<T> medians(size/5);
	std::vector<T> less;
	std::vector<T> greater;
	std::array<T, 5> subset;

	for (size_t idx{0}, auto iter=C.begin(); iter!=C.end(); ++iter) {
		subset.at(idx) = *iter;
		if (((idx+1) % 5 == 0)) {
			medians.push_back(quick_select(subset, 3));
			idx = 0;
		}
	}

	auto pivot = quick_select(medians, size/10);

	for (auto iter=C.begin(); iter != C.end(); ++iter) {
		if ((*iter) < pivot) {
			less.push_back(*iter);
		} else {
			greater.push_back(*iter);
		}
	}
	if (K== less.size()) {
		return pivot;
	}
	else if(K < less.size()) {
		return quick_select(less, K);
	}
	else{
		return quick_select(greater, K-less.size());
	}
}


#endif // MY_QUICK_SELECT
