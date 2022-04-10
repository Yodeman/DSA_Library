#ifndef MY_MERGE_SORT
#define MY_MERGE_SORT

#include <iostream>
#include <ranges>

template<typename Container>
concept sort_requirement = std::ranges::range<Container> &&\
						   std::totally_ordered<Container::value_type>

template<sort_requirement Container, std::predicate Compare>
void merge_sort(Container C, Compare func)
{
	// pass...
}

#endif // MY_MERGE_SORT
