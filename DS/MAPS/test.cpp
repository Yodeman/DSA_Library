#include <iostream>
#include <limits>
#include <typeinfo>
#include <memory>

template<typename T>
void type_test()
{
	std::cout << typeid(T).name() << " -> byte size: " << sizeof(T) 
			  << " ,number of bits: " << std::numeric_limits<T>::digits
			  << std::endl;
}

int main()
{
	type_test<char>();
	type_test<int>();
	type_test<float>();
	type_test<double>();
	type_test<size_t>();
	type_test<long long int>();
	std::cout << "\n---------------------------------------------" << std::endl;
	std::pair<int, int> p{};
	std::cout << p.first << ", " << p.second << std::endl;
	int a = 10, b = 5;
	std::shared_ptr<int> sa = std::make_shared<int>(a);
	std::shared_ptr<int> sb = std::make_shared<int>(b);
	std::weak_ptr<int> w{sa};
	std::weak_ptr<int> w2{w};
	std::cout << *(w.lock()) << std::endl;
	w.reset();
	std::cout << *(w2.lock()) << std::endl;

	return 0;
}
