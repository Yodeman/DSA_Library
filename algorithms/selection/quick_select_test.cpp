#include <iostream>
#include <vector>
#include <valarray>
#include <deque>
#include <random>
#include <ranges>
#include "quick_select.hpp"

/*
template<std::ranges::range Container>
void print(Container& C)
{
	std::cout << "\t";
	for(auto& i : C)
		std::cout << i << " ";
	std::cout << std::endl;
}
*/

template<std::ranges::range Container>
void select_test(Container& C, size_t K)
{
	std::cout << "Selecting Kth min element in: ";
	//print(C);
	std::cout << "with K = " << K << std::endl;
	auto km = quick_select(C, K);
	std::cout << "Kth min element = " << km << std::endl;
	//quick_sort(C);
	//std::cout << "Sorted sequence: ";
	//print(C);
	std::cout << "\n";
}

int main()
{
	constexpr unsigned int sz = 10'000;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(1, sz);
	//std::array<int, 20> a {
	//		4, 10, 5, 6, 3, 2, 10, 2, 3, 9, 10, 7, 2, 2, 4, 1, 2, 5, 6, 6
	//	};
	
	std::array<int, sz> a;
	std::vector<int> v(sz);
	v.shrink_to_fit();
	std::deque<int> d(sz);
	d.shrink_to_fit();

	for (size_t i=0; i < sz; ++i) {
		auto j = dis(gen);
		a.at(i) = j;
		v.at(i) = j;
		d.at(i) = j;
	}

	auto k = dis(gen);

	std::cout << "Test with std::array\n";
	select_test(a, k);
	
	std::cout << "Test with std::vector\n";
	select_test(v, k);

	std::cout << "Test with std::deque:\n";
	select_test(d, k);
}
