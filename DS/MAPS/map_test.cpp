#include <iostream>
#include "map.hpp"

int main()
{
	try{
		Map<int, std::string> m{};
		m.put(5, "A");
		m.put(7, "B");
		m.put(2, "C");
		m.put(2, "E");
		std::cout << m.size() << std::endl;
		auto it = m.find(2);
		std::cout << (*it).second << std::endl;
		m.erase(5);
		std::cout << m.size() << std::endl;
		m.erase(it);
		std::cout << m.size() << std::endl;
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
