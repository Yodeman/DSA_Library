#include <iostream>
#include "splay_tree.hpp"

void splaytree_test1()
{
	SplayTree<int, char> s{};
    s.insert(std::make_pair<int, char>(44, 'a'));
    s.insert(std::make_pair<int, char>(17, 'b'));
    s.insert(std::make_pair<int, char>(78, 'c'));
    s.insert(std::make_pair<int, char>(32, 'd'));
    s.insert(std::make_pair<int, char>(50, 'e'));
    s.insert(std::make_pair<int, char>(88, 'f'));
    s.insert(std::make_pair<int, char>(48, 'g'));
    s.insert(std::make_pair<int, char>(62, 'h'));
    s.insert(std::make_pair<int, char>(54, 'f'));

    std::cout << "size: " << s.size() << std::endl;

    for(auto& i : s)
		std::cout << "key: " <<i.first << ", value: " << i.second << std::endl;

    auto iter = s.find(62);
    std::cout << (*iter).first << " " << (*iter).second << std::endl;

    try{
		std::cout << "removing " << 32 << std::endl;
        s.remove(32);
        std::cout << "size: " << s.size() << std::endl;
		std::cout << "removing " << 90 << std::endl;
        s.remove(90);
    }
    catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
	std::cout << "removing " << (*iter).first << std::endl;
    s.remove((*iter).first);
    std::cout << "size: " << s.size() << std::endl;
    for(auto& i : s)
		std::cout << "Key: " << i.first << ", Value: " << i.second << std::endl;
}

void splaytree_test2()
{
	SplayTree<std::string, int> s{};
    s.insert(std::make_pair<std::string, int>("Paul", 20));
    s.insert(std::make_pair<std::string, int>("Peter", 16));
    s.insert(std::make_pair<std::string, int>("Lydia", 18));
    s.insert(std::make_pair<std::string, int>("Mary", 22));
    s.insert(std::make_pair<std::string, int>("Emmanuel", 10));
    s.insert(std::make_pair<std::string, int>("Nathaniel", 20));
    s.insert(std::make_pair<std::string, int>("Paul", 21));

	std::cout << "size: " << s.size() << std::endl;

    for(auto& i : s)
		std::cout << "key: " <<i.first << ", value: " << i.second << std::endl;
	auto iter = s.find("Paul");
    std::cout << (*iter).first << " " << (*iter).second << std::endl;

    try{
		std::cout << "removing Nathaniel" << std::endl;
        s.remove("Nathaniel");
        std::cout << "size: " << s.size() << std::endl;
		std::cout << "removing Esther" << std::endl;
        s.remove("Esther");
    }
    catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
	std::cout << "removing " << (*iter).first << std::endl;
    s.remove((*iter).first);
    std::cout << "size: " << s.size() << std::endl;
    for(auto& i : s)
        std::cout << "Key: " << i.first << ", Value: " << i.second << std::endl;
}

int main()
{
	splaytree_test1();
	std::cout << "\n";
	splaytree_test2();
	return 0;
}
