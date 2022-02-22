#include <iostream>
#include "./array_vector.hpp"

int main()
{
	ArrayVector<int> v;
	ArrayVector<int> u{v};
	std::cout << "test" << std::endl;

	return 0;
}
