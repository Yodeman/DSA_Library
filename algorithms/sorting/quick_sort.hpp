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
		typename Compare=std::less<std::iter_value_t<Iter>>,
		std::totally_ordered Elem_type=std::iter_value_t<Iter>
	>
void quick_sort_step(Iter b, Iter e, Compare comp={}, bool verbose=false)
{
	if (b >= e) return;
	Elem_type pivot;
	size_t sz = std::distance(b, e) + 1;
	size_t n = sz/2;
	auto equal_op = std::equal_to<std::iter_value_t<Iter>>{};
	auto piter = (b + n);
	if (sz==1) {
		if (comp(*b, *e) || equal_op(*b, *e)) {
			return;
		} else {
			std::iter_swap(b, e);
			return;
		}
	} else {
		piter = comp(*b, *piter) ? (comp(*piter, *e) ? piter : (comp(*b, *e)\
				) ? e : b) : (comp(*b, *e) ? b : (comp(*piter, *e) ? e : piter));

		pivot = *piter;
		if (!equal_op(*piter, *b)) {
			std::iter_swap(b, piter);
		}
		piter = b;
	}

	auto left_iter = b + 1;
	auto right_iter = e;

	if (verbose)
		std::cout << "size: " << sz << " pivot: " << pivot << std::endl;

	while (left_iter < right_iter) {
		if (verbose) {
			std::cout << "Before swapping: \n";
			for (auto i=b; i!=e+1; ++i)
				std::cout << *i << " ";
			std::cout << std::endl;
		}

		// move right iterator towards the left hand side unless the element pointed to is smaller
		// than or equal to the pivot.
		while((left_iter < right_iter) && (!comp(*right_iter, pivot) && !equal_op(pivot, *right_iter))) {
			--right_iter;
		}
		// move left iterator towards the right hand side unless the element pointed to is larger
		// than the pivot.
		while ((left_iter < right_iter) && (comp(*left_iter, pivot) || equal_op(*left_iter, pivot))){
			//if (equal_op(*left_iter, pivot) && comp(*right_iter, pivot))
			//	break;
			++left_iter;
		}
		if (left_iter < right_iter) {
			std::iter_swap(left_iter, right_iter);
			++left_iter;
			--right_iter;

			if (verbose) {
				std::cout << "After swapping: \n";
				for (auto i=b; i!=e+1; ++i)
					std::cout << *i << " ";
				std::cout << std::endl;
			}
		}
	}
	if (comp(*left_iter, *piter))
		std::iter_swap(piter, left_iter);
	quick_sort_step(b, left_iter-1, comp, verbose);
	quick_sort_step(left_iter, e, comp, verbose);
}

template<
		sort_requirement Container,
		std::totally_ordered Elem_type = std::ranges::range_value_t<Container>,
		typename Compare = std::less<std::ranges::range_value_t<Container>>
	>
void quick_sort(Container& C, bool verbose=false, Compare comp={})
{
	if (std::ranges::size(C) <= 1)
		return;
	quick_sort_step(std::ranges::begin(C), std::ranges::end(C)-1, comp, verbose);
}

#endif // MY_QUICK_SORT
