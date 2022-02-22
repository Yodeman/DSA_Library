#include <iostream>

size_t gcd(size_t n, size_t m)
{
    size_t max = (n > m) ? n : m;
    size_t min = (max == n) ? m : n;
    size_t result = max % min;

    if (result == 0) return min;
    else return gcd(min, result);
}

int main()
{
    std::cout << "GCD(80844, 25320): " << gcd(80844, 25320) << std::endl;

    return 0;
}
