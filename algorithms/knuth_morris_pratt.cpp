/*
 * Implementation of Hnuth-Morris-Pratt pattern matching
 * algorihm. The algorithm icrementally process the text
 * by comparing it to the pattern. Each time there is a
 * match, the current indices (at the text and pattern)
 * is incremented, else, a function is called to determine
 * the new index in the pattern where comparison will start.
 *
 * Knuth-Morris-Pratt algorithm makes use of the result of
 * previous comparisons rather than starting comparison
 * afresh, thereby making the algorithm to run faster.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<size_t> KMPFailure(const std::string& S)
{
    /*
     * Called by the main pattern matching function in order
     * to determine how much the patter needs to be shifted
     * due to the mismatch that occurs.
     *
     * This function finds the longest prefix of string S of
     * length L (L is the length of S) which is a suffix of
     * subsequence of S formed from the first M characters of
     * S (M -> 0...L)
    */

    std::vector<size_t> lengths(S.size(), 0);
    lengths.shrink_to_fit();
    size_t i = 1, j=0;

    while (i < S.size()) {
        if (S.at(j)==S.at(i)) {
            // j+1 characters have been matched
            lengths.at(i) = j+1;
            ++i;
            ++j;
        }
        else if (j > 0) {
            j = lengths.at(j-1);
        }
        else {
            ++i;
        }
    }

    return lengths;
}

std::vector<size_t> KMP_pattern_match(const std::string& text, const std::string& pattern)
{
    std::vector<size_t> match_indices, lengths = KMPFailure(pattern);
    size_t i = 0, j = 0;
    size_t p_s = pattern.size() - 1;

    while (i < text.size()) {
        if (text.at(i)==pattern.at(j)) {
            if (j==p_s) {
                match_indices.push_back(i-p_s);
                j = 0;
                ++i;
            }
            else {
                ++i;
                ++j;
            }
        }
        else if (j > 0) {
            j = lengths.at(j-1);
        }
        else {
            ++i;
        }
    }
    match_indices.shrink_to_fit();

    return match_indices;
}

int main(int argc, char* argv[])
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
	return 0;
}
