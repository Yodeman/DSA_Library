#include <iostream>
#include <unordered_map>

/*
 * Implementation of longest common subsequence algorithm
*/

std::string make_index(int i, int j)
{
    std::string s{};
    s += '(';
    s += std::to_string(i);
    s += ", ";
    s += std::to_string(j);
    s += ')';

    return s;
}

std::unordered_map<std::string, int> lcs(const std::string& S1, const std::string& S2)
{
    std::unordered_map<std::string, int> L;
    auto n = S1.size();
    auto m = S2.size();

    //for (size_t i : std::span<int, n-1>{})
    //    L[std::make_pair(i, -1)] = 0;
    //for (size_t j : std::span<int, m-1>{})
    //    L[std::make_pair(-1, j)] = 0;
    for (int i=0; i < n; ++i) {
        for (int j=0; j < m; ++j) {
            if (S1.at(i) == S2.at(j))
                L[make_index(i, j)] = L[make_index(i-1, j-1)] + 1;
            else
                L[make_index(i, j)] = std::max(L[make_index(i-1, j)], L[make_index(i, j-1)]);
        }
    }

    return L;
}

int main()
{
    const std::string S1 = "atcaatgatcaacgtaagcttctaagcatgatcaaggtgctcacacagtttatccacaac\
                            ctgagtggatgacatcaagataggtcgttgtatctccttcctctcgtactctcatgacca";
    const std::string S2 = "cggaaagatgatcaagagaggatgatttcttggccatatcgcaatgaatacttgtgactt\
                            gtgcttccaattgacatcttcagcgccatattgcgctggccaaggtgacggagcgggatt";
    std::unordered_map<std::string, int> L = lcs(S1, S2);

    for (auto& elem : L) {
        std::cout << elem.first << ": " << elem.second << std::endl;
    }

    return 0;
}
