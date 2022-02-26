#include <iostream>
//#include <random>
//#include <memory>
//#include "./binary_tree.hpp"
//#include "binary_search_tree.hpp"
#include "avl_tree.hpp"

/*
void foo1()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 40);
	BinaryTree<int> bt{};
	std::cout << "order of insertion: ";
	for(int i=0; i<20; ++i) {
		auto r = dis(gen);
		bt.insert(r);
		std::cout << r << " ";
	}
	std::cout << "\n\nInorder traversal: \n\n";
	bt.inorder();
}
*/
/*
void foo()
{
	std::unique_ptr<int> sp1 = std::make_unique<int>(2);
	std::unique_ptr<int>& sp2 = sp1;
	sp1.reset();
	std::cout << (sp1 == sp2) << std::endl;
	std::cout << (sp1 == nullptr) << std::endl;
	std::cout << (sp2 == nullptr) << std::endl;
	if (sp2) std::cout << *sp2 << std::endl;
}
*/

void tree_test()
{
	/*
	//SearchTree<std::string, int> s{};
	AVLTree<std::string, int> s{};
	s.insert(std::make_pair<std::string, int>("Paul", 20));
	s.insert(std::make_pair<std::string, int>("Peter", 16));
	s.insert(std::make_pair<std::string, int>("Lydia", 18));
	s.insert(std::make_pair<std::string, int>("Mary", 22));
	s.insert(std::make_pair<std::string, int>("Emmanuel", 10));
	s.insert(std::make_pair<std::string, int>("Nathaniel", 20));
	s.insert(std::make_pair<std::string, int>("Paul", 21));
	std::cout << s.size() << std::endl;
	*/
	AVLTree<int, char> s{};
	s.insert(std::make_pair<int, char>(44, 'a'));
	s.insert(std::make_pair<int, char>(17, 'b'));
	s.insert(std::make_pair<int, char>(78, 'c'));
	s.insert(std::make_pair<int, char>(32, 'd'));
	s.insert(std::make_pair<int, char>(50, 'e'));
	s.insert(std::make_pair<int, char>(88, 'f'));
	s.insert(std::make_pair<int, char>(48, 'g'));
	s.insert(std::make_pair<int, char>(62, 'h'));
	s.insert(std::make_pair<int, char>(54, 'f'));
	for(auto& i : s)
		std::cout << "key: " <<i.first << ", value: " << i.second << std::endl;
	
	//auto i = s.begin();
	//while(i != s.end()) {
	//	std::cout << "key: " <<(*i).first << ", value: " << (*i).second << std::endl;
	//	++i;
	//}
	
	//auto iter = s.find("Nathaniel");
	auto iter = s.find(62);
	std::cout << (*iter).first << " " << (*iter).second << std::endl;
	
	try{
		//s.remove("Paul");
		s.remove(32);
		std::cout << s.size() << std::endl;
		//s.remove("Victor");
		s.remove(90);
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	s.remove(iter);
	//s.remove("Nathaniel");
	std::cout << s.size() << std::endl;
	for(auto& i : s)
		std::cout << "Key: " << i.first << ", Value: " << i.second << std::endl;
}

/*
void avl_node_test()
{
	struct Node{
		int elem;
		std::shared_ptr<Node>& left;// = nullptr;
		std::shared_ptr<Node>& right;// = nullptr;
		std::weak_ptr<Node>& parent;// = nullptr;
		Node(int e): elem{e}{}
	};

	std::shared_ptr<Node> node = std::make_shared<Node>(2);
	std::shared_ptr<Node> l_child = std::make_shared<Node>(3);
	std::shared_ptr<Node> r_child = std::make_shared<Node>(4);
	node->left = l_child;
	node->right = r_child;
	l_child->parent = node;
	r_child->parent = node;
	std::shared_ptr<Node> node2 = std::make_shared<Node>(5);
}
*/
int main()
{
	//foo();
	tree_test();
	//avl_node_test();
}
