#include <iostream>
#include "red_black_tree.hpp"

void tree_test()
{
	RBTree<int, char> s{};
	/*
	// to test all cases of insertion operation.
	s.insert(44, 'a');
	s.insert(17, 'b');
	s.insert(78, 'c');
	s.insert(32, 'd');
	s.insert(50, 'e');
	s.insert(88, 'f');
	s.insert(48, 'g');
	s.insert(62, 'h');
	s.insert(54, 'f');
	*/
	// to test all cases of removal operation.
	s.insert(14, 'a');
	s.insert(7, 'b');
	s.insert(16, 'c');
	s.insert(4, 'd');
	s.insert(12, 'e');
	s.insert(15, 'f');
	s.insert(18, 'g');
	s.insert(3, 'h');
	s.insert(5, 'i');
	s.insert(17, 'j');

	std::cout << "Tree size: " << s.size() << std::endl;

	for(auto& i : s)
		std::cout << "key: " <<i.first << ", value: " << i.second << std::endl;
	
	//auto i = s.begin();
	//while(i != s.end()) {
	//	std::cout << "key: " <<(*i).first << ", value: " << (*i).second << std::endl;
	//	++i;
	//}
	
	auto iter = s.find(62);
	if (iter != s.end())
		std::cout << (*iter).first << " " << (*iter).second << std::endl;
	
	try{
		std::cout << "removing 3" << std::endl;
		s.remove(3);
		std::cout << "removing 12" << std::endl;
		s.remove(12);
		std::cout << "removing 17" << std::endl;
		s.remove(17);
		std::cout << "removing 18" << std::endl;
		s.remove(18);
		std::cout << "removing 15" << std::endl;
		s.remove(15);
		std::cout << "removing 16" << std::endl;
		s.remove(16);
		std::cout << "removing 32" << std::endl;
		s.remove(32);
		std::cout << "removing 90" << std::endl;
		s.remove(90);
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	try {
		s.remove(iter);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Tree size: " << s.size() << std::endl;
	for(auto& i : s)
		std::cout << "Key: " << i.first << ", Value: " << i.second << std::endl;
}

int main()
{
	tree_test();

	return 0;
}
