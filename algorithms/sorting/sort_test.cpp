#include <iostream>
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

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(1, 100);
	std::array<int, 21> a {
			30, 32, 3, 46, 90, 10, 1, 0, 88, 67, 77, 5, 2, 1,
			29, 10, 18, 86, 54, 43, 50
		};
	std::vector<int> v;
	std::deque<int> d;
	std::forward_list<int> f;
	std::list<int> l;
	std::unordered_set<int> us;
	std::stack<int> s;
	std::queue<int> q;
	std::priority_queue<int> pq;

	for (auto i=0; i < 21; ++i) {
		auto j = dis(gen);
		v.push_back(j);
		d.push_back(j);
		l.push_back(j);
		us.emplace(j);
		s.emplace(j);
		q.emplace(j);
		pq.emplace(j);
		f.push_front(j);
	}
	
	std::cout << "Test with std::vector:\n";
	std::cout << "\tBefore: ";
	print(v);
	merge_sort(v);
	std::cout << "\tAfter: ";
	print(v);

	std::cout << "Test with std::deque:\n";
	std::cout << "\tBefore: ";
	print(d);
	merge_sort(d);
	std::cout << "\tAfter: ";
	print(d);

	std::cout << "Test with std::list:\n";
	std::cout << "\tBefore: ";
	print(l);
	merge_sort(l);
	std::cout << "\tAfter: ";
	print(l);

	/*std::cout << "Test with std::unordered_set:\n";
	std::cout << "\tBefore: ";
	print(us);
	merge_sort(us);
	std::cout << "\tAfter: ";
	print(us);*/

	std::cout << "Test with std::forward_list:\n";
	std::cout << "\tBefore: ";
	print(f);
	merge_sort(f);
	std::cout << "\tAfter: ";
	print(f);
	
	/*
	// uncommenting this part will cause compilation to fail,
	// since the containers do not satisfy the requirements
	// of the algorithm.
	std::cout << "Test with std::stack:\n";
	std::cout << "\tBefore: ";
	print(s);
	//merge_sort(s);
	std::cout << "\tAfter: ";
	print(s);

	std::cout << "Test with std::queue:\n";
	std::cout << "\tBefore: ";
	print(q);
	//merge_sort(q);
	std::cout << "\tAfter: ";
	print(q);

	std::cout << "Test with std::priority_queue:\n";
	std::cout << "\tBefore: ";
	print(pq);
	//merge_sort(pq);
	std::cout << "\tAfter: ";
	print(pq);
	*/

	return 0;
}
