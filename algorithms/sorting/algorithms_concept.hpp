#ifndef MY_ALGORITHM_CONCEPT
#define MY_ALGORITHM_CONCEPT

#include <iostream>
#include <concepts>
#include <ranges>

template<typename Container>
concept sort_requirement = std::ranges::range<Container> &&\
						   std::totally_ordered<std::ranges::range_value_t<Container>>;

#endif // MY_ALGORITHM_CONCEPT
