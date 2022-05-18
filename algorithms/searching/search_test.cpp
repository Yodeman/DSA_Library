#include <iostream>
#include <vector>
#include "knuth_morris_pratt.hpp"
#include "boyer_moore.hpp"

void kmp_test()
{
	constexpr std::string_view pattern{"oom"};
    constexpr std::string_view text{"boomzoomloomfoammoondoomgloomroom"};

	auto match_indices = KMP_pattern_match(text, pattern);

	for(auto& elem: match_indices)
		std::cout << elem << " ";
    std::cout << std::endl;
}

void bm_test()
{
    constexpr std::string_view pattern{"oom"};
    constexpr std::string_view text{"boomzoomloomfoammoondoomgloomroom"};

    std::vector<size_t> idxs = boyer_moore_match(text, pattern);
    for (auto i : idxs)
        std::cout << static_cast<size_t>(i) << " ";
    std::cout << std::endl;
}

int main()
{
    kmp_test();
    bm_test();
}
