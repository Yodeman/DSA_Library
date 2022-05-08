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
            typename T1 = typename std::tuple_element<0, typename Container::value_type>::type,
            typename T2= typename std::tuple_element<1, typename Container::value_type>::type
        >
concept bucket_sort_requirement = std::input_or_output_iterator<typename Container::iterator> &&\
                                  std::is_same_v<int, T1> &&\
                                  std::is_same_v<std::pair<T1,T2>, typename Container::value_type>;

template<
            bucket_sort_requirement Container,
            size_t N=1001
        >
void bucket_sort(Container& C)
{
    auto max_elem = std::max_element(C.begin(), C.end());
    if ((*max_elem).first > N)
        throw std::runtime_error("sequence contains key larger than allowed range.");

    std::vector<std::vector<typename Container::value_type>> bucket(N);
    bucket.shrink_to_fit();

    for (auto e : C) {
        bucket.at(e.first).push_back(e);
    }

    auto iter = C.begin();

    for (size_t i{0}; i < N; ++i) {
        bucket[i].shrink_to_fit();
        for (auto e = bucket[i].begin(); e != bucket[i].end(); ++e) {
            *iter = *e;
            ++iter;
        }
    }
}

#endif //MY_BUCKET_SORT
