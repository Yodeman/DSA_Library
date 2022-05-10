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

std::pair<int, int> get_index(const std::string& s)
{
    std::string::size_type pos = s.find_first_of(',');
    int i = std::stoi(s.substr(1, pos));
    int j = std::stoi(s.substr(pos+1, s.size()-1));

    return std::make_pair(i,j);
}

std::unordered_map<std::string, int> lcs_lengths(const std::string& S1, const std::string& S2)
{
    std::unordered_map<std::string, int> L;
    auto n = S1.size();
    auto m = S2.size();

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

std::tuple<int, int, int, std::string> lcs(const std::string& S1, const std::string& S2)
{
    int i, j, n, m;
    std::string subsequence{};

    std::unordered_map<std::string, int> lengths = lcs_lengths(S1, S2);
    auto longest = *(std::max_element(
                            lengths.begin(), lengths.end(),
                            [](auto& k1, auto& k2){ return k1.second < k2.second; }
                        )
                    );
    std::tie(i, j) = get_index(longest.first);
    n = i;
    m = j;
    
    // construct the subsequence from back to front.
    while ((n!=-1 && m!=-1)) {
        if (S1.at(n)==S2.at(m)) {
            subsequence += S1.at(n);
            --n;
            --m;
        }
        else {
            if (lengths[make_index(n, m-1)] >= lengths[make_index(n-1, m)]) {
                --m;
            }
            else {
                --n;
            }
        }
    }
    std::reverse(subsequence.begin(), subsequence.end());

    return std::make_tuple(i, j, longest.second, subsequence);
}

int main()
{
    const std::string S1 = "atcaatgatcaacgtaagcttctaagcatgatcaaggtgctcacacagtttatccacaacctgagtggatgacatcaagataggtcgttgtatctccttcctctcgtactctcatgacca";
    const std::string S2 = "cggaaagatgatcaagagaggatgatttcttggccatatcgcaatgaatacttgtgacttgtgcttccaattgacatcttcagcgccatattgcgctggccaaggtgacggagcgggatt";
    int i, j, l;
    std::string longest_common_subsequence;
    std::tie(i, j, l, longest_common_subsequence) = lcs(S1, S2);
    std::cout << "The longest common subsequence is of length " << l
            << " and found at the first " << i
            << " characters of the first sequence,\n and first " << j
            << " characters of the second sequence.\n";
    std::cout << "\nsubsequence: " << longest_common_subsequence
            << "\nsize: "<< longest_common_subsequence.size() << std::endl;


    return 0;
}
