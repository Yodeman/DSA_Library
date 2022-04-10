#ifndef MY_MERGE_SORT
#define MY_MERGE_SORT

#include <iostream>
#include <ranges>
#include <cmath>

template<typename Container>
concept sort_requirement = std::ranges::range<Container> &&\
						   std::totally_ordered<std::ranges::range_value_t<Container>>;

template<
		sort_requirement Container,
		std::predicate Compare=std::less<Container::value_type>
	>
void merge_sort(Container& C, Compare comp)
{
	size_t sz = std::ranges::size(C);
	if (sz == 1) return;
	size_t n = sz / 2;
	auto s1 = std::ranges::subrange(C.begin(), C.begin()+n);
	auto s2 = std::ranges::subrange(C.beigin()+n, C.end());
	merge_sort(s1, comp);
	merge_sort(s2, comp);
	std::ranges::merge(s1, s2, C.begin());
}

#endif // MY_MERGE_SORT
