#include <iostream>
#include <vector>

template<typename C>
void insertion_sort(C& arr)
{
    for (size_t i=1; i<arr.size(); ++i) {
	auto cur = arr[i];
	int j = i-1;

	while ((j >= 0) && (arr[j] > cur)) {
	    arr[j+1] = arr[j];
	    --j;
	}
	arr[j+1] = cur;
    }
}

int main()
{
    std::vector<int> v{3, 0, 10, 1, 7, 2, 3, 9, 8};
    std::cout << "Before sorting: " << std::endl;
    for (auto x : v)
	std::cout << x << " ";
    std::cout << "\n";
    insertion_sort(v);
    std::cout << "After sorting: \n";
    for (auto x : v)
	std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
