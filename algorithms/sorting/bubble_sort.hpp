#include <iostream>
#include <functional>

template<typename C, typename T=C::value_type>
void bubble_sort(C& seq, std::function<bool(const T&, const T&)> f=std::less<>())
{
	return;
}

int main()
{
	std::cout << "test" << std::endl;

	return 0;
}
