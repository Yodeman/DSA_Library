#ifndef MY_MERGE_SORT
#define MY_MERGE_SORT

#include <iostream>
#include <iterator>
#include <ranges>
#include <cmath>

template<typename Container>
concept sort_requirement = std::ranges::range<Container> &&\
						   std::totally_ordered<std::ranges::range_value_t<Container>>;

template<
		sort_requirement Container,
		std::forward_iterator Iter,
		typename Compare = std::less<std::ranges::range_value_t<Container>>
	>
void my_merge(Container& C1, Container& C2, Iter O, Compare comp = {})
{
	auto iter1 = C1.begin();
	auto iter2 = C2.begin();
	while ((iter1 != C1.end()) && (iter2 != C2.end())) {
		if (comp(*iter1, *iter2)) {
			*O = *iter1;
			++iter1;
		} else {
			*O = *iter2;
			++iter2;
		}
		++O;
	}
	while (iter1 != C1.end()) {
		*O = *iter1;
		++iter1;
		++O;
	}
	while (iter2 != C2.end()) {
		*O = *iter2;
		++iter2;
		++O;
	}
}

template<
		sort_requirement Container,
		typename Compare = std::less<std::ranges::range_value_t<Container>>,
		typename Elem_type = std::ranges::range_value_t<Container>
		//std::predicate Compare = std::less<std::ranges::range_value_t<Container>>
	>
void merge_sort(
			Container& C,
			Compare comp = {}
		)
{
	size_t sz = std::ranges::size(C);
	if (sz == 1) return;
	size_t n = sz / 2;
	std::vector<Elem_type> s1(n);
	s1.shrink_to_fit();
	std::vector<Elem_type> s2(sz-n);
	s2.shrink_to_fit();
	std::ranges::copy(std::ranges::subrange(C.begin(), C.begin()+n), s1.begin());
	std::ranges::copy(std::ranges::subrange(C.begin()+n, C.end()), s2.begin());
	merge_sort(s1, comp);
	merge_sort(s2, comp);
	my_merge(s1, s2, C.begin(), comp);
}

#endif // MY_MERGE_SORT
