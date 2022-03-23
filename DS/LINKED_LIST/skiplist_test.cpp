#include <iostream>
#include "./skip_list.hpp"

int main()
{
	SkipList<int> s{};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 10);
	for (auto i = 0; i<10; ++i){
		s.insert(distrib(gen));
	}
	std::cout << std::endl;
	s.insert(90);
	s.insert(2);
	std::cout << s << std::endl;
	auto f = s.find(2);
	if (f) {
		std::cout << "removing " << *f << std::endl;
		s.remove(*f);
	}
	std::cout << s << std::endl;
	std::cout << "removing 90" << std::endl;
	s.remove(90);
	std::cout << s << std::endl;

	return 0;
}
