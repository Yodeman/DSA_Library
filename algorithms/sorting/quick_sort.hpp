#ifndef MY_QUICK_SORT
#define MY_QUICK_SORT

/*
 * Implementation of quick sort algorithm, this implementation
 * uses median-of-three heuristic to select the pivot in order
 * to ensure the pivot falls in the middle of the sequence.
 * thereby causing the algorithm to run in it's average time
 * complexity O(nlogn).
*/ 

#include "algorithms_concept.hpp"

template<
		iterator_requirement2 Iter,
		typename Compare=std::less<std::iter_value_t<Iter>>
	>
void quick_sort_step(Iter b, Iter e, Compare comp={})
{
	if (b >= e) return;
	size_t sz = std::ranges::distance(b, e);
	auto pivot = (*b + *(b + sz/2) + *e) / 3;
	auto left_iter = b;
	auto right_iter = e;
	while (left_iter <= right_iter) {
		// scan left for larger element
		while ((left_iter <= right_iter) && !comp(pivot, *left_iter)) {
			++left_iter;
		}
		// scan right for lesser element
		while((left_iter <= right_iter) && !comp(*right_iter, pivot)) {
			--right_iter;
		}
		if (left_iter < right_iter) {
			auto temp = std::move(*left_iter);
			*left_iter = std::move(*right_iter);
			*right_iter = std::move(temp);
		}
	}
	quick_sort_step(b, left_iter-1, comp);
	quick_sort_step(left_iter+1, e, comp);

}

template<
		sort_requirement Container,
		typename Elem_type = std::ranges::range_value_t<Container>,
		typename Compare = std::less<std::ranges::range_value_t<Container>>
	>
void quick_sort(Container& C, Compare comp={})
{
	if (std::ranges::size(C) <= 1)
		return;
	quick_sort_step(std::ranges::begin(C), std::ranges::end(C)-1, comp);
}

#endif // MY_QUICK_SORT
