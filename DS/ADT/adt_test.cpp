#include <iostream>
#include <vector>
#include <algorithm>
//#include "priority_queue.hpp"
#include "heap_priority_queue.hpp"

int p_test()
{
	PriorityQueue<int> p{};
	p.insert(5);
	p.insert(9);
	p.insert(2);
	p.insert(7);
	p.insert(10);
	p.insert(2);
	p.insert(9);
	std::cout << p << std::endl;
	while(!p.is_empty()){
		std::cout << "min: " << p.min() << std::endl;
		p.remove_min();
	}
	std::cout << p << std::endl;
	
	return 0;
}
/*
void foo()
{
	std::vector<int> v{1, 2, 3, 4, 5};
	for (auto& i:v)
		std::cout << i << " ";
	std::endl;
	std::swap(*v.cbegin(), *(v.cbegin()+2));
	for (auto& i:v)
		std::cout << i << " ";
	std::cout << std::endl;
}
*/
int main()
{
	p_test();
	//foo();

	return 0;
}
