#include <iostream>
#include "hashmap.hpp"

template<typename KEY>
struct MyHash{
	using result_type = size_t;
	constexpr size_t operator()(const KEY& k){
		return k % 13;
	}
};

int main()
{
	HashMap<int,std::string, MyHash<int>, 13> h{};
	//HashMap<int, std::string> h{};
	h[1] = "paul";
	h[1] = "Paul";
	h[5] = "Lydia";
	h[12] = "Mary";
	h[14] = "Peter";
	/*
	for(auto& i : h)
		std::cout << "key: " << i.first << ", value: " << i.second << std::endl;
	*/
	/*
	auto it=h.begin();
	while(it!=h.end()){
		std::cout << "key: " << (*it).first << ", value: " << (*it).second << std::endl;
		it++;
	}
	*/
	auto it=h.end();
	while(it!=h.begin()){
		it--;
		std::cout << "key: " << (*it).first << ", value: " << (*it).second << std::endl;
	}

	return 0;
}
