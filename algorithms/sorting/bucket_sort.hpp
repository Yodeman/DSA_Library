#ifndef MY_BUCKET_SORT
#define MY_BUCKET_SORT

/*
 * Implemenation of bucket sort algorithm which sorts a sequence of entries, each a key-value pair.
 * The algorithm is not based on comparisons, but on using keys as indices into a bucket array that
 * has cells indexed from 0 to N (N being the maximum of the keys). An entry with key k is placed in
 * the bucket which it self is a sequence (of entries with k). After inserting each entry of the
 * input sequence, the entries are inserted back in to the input sequence in sorted order by
 * enumerating the contents of the buckets in order.
*/

#include <iostream>
#include <concepts>
#include <utility>
#include <ranges>
#include <type_traits>

template<
			typename Container,
			typename T1=std::tuple_element<0, Container::value_type>,
			typename T2=std::tuple_element<1, Container::value_type>
		>
concept bucket_sort_requirement = std::output_range<Container::iterator> && std::is_same_v<int, T1> &&\
								  std::is_same_v<std::pair<T1,T2>, Container::value_type> &&\
								  std::totally_ordered<T1> && std::totally_ordered<T2>;

template<
			bucket_sort_requirement Container,
			size_t N=1001
		>
void bucket_sort(Container& C)
{
	auto max_elem = std::max_element(C.begin(), C.end());
	if (max_elem.first > N)
		std::throw runtime_error("sequence contains key larger than allowed range.");

	std::vector<std::vector<Container::value_type>> bucket(N);

	for (auto e : C) {
		bucket.at(e.first).push_back(e);
	}

	for (size_t i{0}, Container::iterator d = C.begin(); i < N; ++i) {
		for (auto e = bucket[i].begin(); e != bucket[i].end(); ++e) {
			*d = *e;
			++d;
		}
	}
}

#endif //MY_BUCKET_SORT
