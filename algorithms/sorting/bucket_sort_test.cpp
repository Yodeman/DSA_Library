#include <iostream>
#include <vector>
#include <valarray>
#include <deque>
#include <random>
#include <ranges>
#include "bucket_sort.hpp"

template<std::ranges::range Container>
void print(Container& C)
{
    std::cout << "\t";
    for(auto& i : C)
        std::cout << "(" << i.first << ", " << i.second << ") ";
    std::cout << std::endl;
}

template<std::ranges::range Container>
void sort_test(Container& C)
{
    std::cout << "Before: \n";
    print(C);
    bucket_sort(C);
    std::cout << "Sorted sequence: \n";
    print(C);
    std::cout << "\n";
}

int main()
{
    constexpr unsigned int sz = 20;//10'000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<>dis(1, sz);
    std::uniform_real_distribution<>real_dis(1.0, static_cast<double>(sz));

    std::array<std::pair<int, double>, sz> a;
    std::vector<std::pair<int, double>> v(sz);
    v.shrink_to_fit();
    std::deque<std::pair<int, double>> d(sz);
    d.shrink_to_fit();

    for (size_t i=0; i < sz; ++i) {
        auto j = dis(gen);
        auto r = real_dis(gen);
        a.at(i) = std::make_pair(j, r);
        v.at(i) = std::make_pair(j, r);
        d.at(i) = std::make_pair(j, r);
    }

    std::cout << "Test with std::array\n";
    sort_test(a);

    std::cout << "Test with std::vector\n";
    sort_test(v);

    std::cout << "Test with std::deque:\n";
    sort_test(d);
}
