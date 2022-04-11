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
#include "merge_sort.hpp"

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
	auto start = std::chrono::steady_clock::now();
	merge_sort(C);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "\tAfter: ";
	//print(C);
	std::cout << "\telapsed time: " << elapsed_seconds.count() <<"s\n\n";
}

int main()
{
	constexpr unsigned int sz = 100000;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(1, 1000);
	//std::array<int, 21> a {
	//		30, 32, 3, 46, 90, 10, 1, 0, 88, 67, 77, 5, 2, 1,
	//		29, 10, 18, 86, 54, 43, 50
	//	};
	std::array<int, sz> a;
	std::vector<int> v(sz);
	v.shrink_to_fit();
	std::deque<int> d(sz);
	d.shrink_to_fit();
	std::forward_list<int> f(sz);
	std::list<int> l(sz);
	//std::unordered_set<int> us;
	//std::stack<int> s;
	//std::queue<int> q;
	//std::priority_queue<int> pq;

	for (size_t i=0; i < 100000; ++i) {
		auto j = dis(gen);
		a.at(i) = j;
		v.push_back(j);
		d.push_back(j);
		l.push_back(j);
		//us.emplace(j);
		//s.emplace(j);
		//q.emplace(j);
		//pq.emplace(j);
		f.push_front(j);
	}
	
	std::cout << "Test with std::array\n";
	deco(a);

	std::cout << "Test with std::vector\n";
	deco(v);

	std::cout << "Test with std::deque:\n";
	deco(d);

	std::cout << "Test with std::list:\n";
	deco(l);

	//std::cout << "Test with std::unordered_set:\n";
	//deco(us);

	std::cout << "Test with std::forward_list:\n";
	deco(f);
	
	
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
