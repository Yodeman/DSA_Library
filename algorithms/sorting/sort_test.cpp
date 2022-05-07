#include <iostream>
#include <chrono>
#include <vector>
#include <valarray>
#include <list>
#include <forward_list>
#include <deque>
#include <unordered_set>
#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include "merge_sort.hpp"
#include "quick_sort.hpp"

template<std::ranges::range Container>
void print(Container& C)
{
	std::cout << "\t";
	for(auto& i : C)
		std::cout << i << " ";
	std::cout << std::endl;
}

template<std::ranges::range Container>
void deco(Container& C)
{
	//std::cout << "\tBefore: ";
	//print(C);

	Container C2 = C;

	auto start = std::chrono::steady_clock::now();
	//merge_sort(C);
	merge_sort(C, std::greater<std::ranges::range_value_t<Container>>{});
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "\telapsed time: " << elapsed_seconds.count() << "s\t(merge sort)\n";
	//std::cout << "\tSorted: " << std::is_sorted(std::begin(C), std::end(C)) << "\n";
	std::cout << "\tSorted: "
			<< std::is_sorted(std::begin(C), std::end(C), std::greater<std::ranges::range_value_t<Container>>{})
			<< "\n";
	//std::cout << "\tAfter:";
	//print(C);

	start = std::chrono::steady_clock::now();
	//quick_sort(C2);
	quick_sort(C2, false, std::greater<std::ranges::range_value_t<Container>>{});
	end = std::chrono::steady_clock::now();
	elapsed_seconds = end - start;
	std::cout << "\n\telapsed time: " << elapsed_seconds.count() << "s\t(quick sort)\n";
	//std::cout << "\tSorted: " << std::is_sorted(std::begin(C), std::end(C)) << "\n";
	std::cout << "\tSorted: "
			<< std::is_sorted(std::begin(C), std::end(C), std::greater<std::ranges::range_value_t<Container>>{})
			<< "\n";
	//quick_sort(C, std::greater<std::ranges::range_value_t<Container>>{});
	//std::cout << "\tAfter: ";
	//print(C2);
	std::cout << "\n\n";
}

int main()
{
	constexpr unsigned int sz = 10'000;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(1, 10);
	//std::array<int, sz> a {
	//		4, 10, 5, 6, 3, 2, 10, 2, 3, 9, 10, 7, 2, 2, 4, 1, 2, 5, 6, 6
	//	};
	
	std::array<int, sz> a;
	std::vector<int> v(sz);
	v.shrink_to_fit();
	std::deque<int> d(sz);
	d.shrink_to_fit();
	//std::forward_list<int> f(sz);
	//std::list<int> l(sz);
	//std::unordered_set<int> us;
	//std::stack<int> s;
	//std::queue<int> q;
	//std::priority_queue<int> pq;

	for (size_t i=0; i < sz; ++i) {
		auto j = dis(gen);
		a.at(i) = j;
		v.at(i) = j;
		d.at(i) = j;
		//l.push_back(j);
		//us.emplace(j);
		//s.emplace(j);
		//q.emplace(j);
		//pq.emplace(j);
		//f.push_front(j);
	}

	std::cout << "sorting containers of " << sz << " random values..." << "\n\n";

	std::cout << "Test with std::array\n";
	deco(a);
	
	std::cout << "Test with std::vector\n";
	deco(v);

	std::cout << "Test with std::deque:\n";
	deco(d);
	
	// uncommenting this section will cause compilation to
	// fail for quick sort but merge sort will compile.
	//std::cout << "Test with std::list:\n";
	//deco(l);

	//std::cout << "Test with std::unordered_set:\n";
	//deco(us);

	//std::cout << "Test with std::forward_list:\n";
	//deco(f);
	
	
	// uncommenting this part will cause compilation to fail,
	// since the containers do not satisfy the requirements
	// of the algorithm.
	//std::cout << "Test with std::stack:\n";
	//deco(s);

	//std::cout << "Test with std::queue:\n";
	//deco(q);

	//std::cout << "Test with std::priority_queue:\n";
	//deco(pq);

	return 0;
}
