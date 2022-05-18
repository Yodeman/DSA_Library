#include <iostream>
#include <vector>
#include "knuth_moris_pratt.hpp"
#include "boyer_moore.hpp"

void kmp_test(int argc, char* argv[])
{
	std::string text, pattern;
	if (argc < 3) {
		std::cerr << "usage with large file: " << "knuth_morris_pratt.exe"
		        << " path/to/text/file/  pattern" << std::endl;
		std::cout << "Enter the text: ";
		std::cin >> text;
        std::cout << " Enter the pattern: ";
        std::cin >> pattern;
	}
	else {
		std::string fname = argv[1];
		pattern = argv[2];
		std::ifstream istrm(fname, std::ios::in);
		if(!istrm.is_open()){
			std::cerr << "failed to open " << fname << std::endl;
			return -1;
		}
		for(std::string line; std::getline(istrm, line);)
			text += line;
	}

	auto match_indices = KMP_pattern_match(text, pattern);
	for(auto& elem: match_indices)
		std::cout << elem << " ";
    std::cout << std::endl;
}

void bm_test()
{
    constexpr std::string_view pattern{"oom"};
    constexpr std::string_view text{"boomzoomloomfoammoondoomgloomroom"};
    std::vector<uint8_t> idxs = boyer_moore_match(text, pattern);
    for (auto i : idxs)
        std::cout << static_cast<size_t>(i) << " ";
    std::cout << std::endl;
    return 0;
}
