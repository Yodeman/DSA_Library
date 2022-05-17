/*
 * Implementation of Boyer-Moore pattern matching algorithm.
*/

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

std::unordered_map<char, uint8_t> last_occur(const std::string_view& pattern)
{
    /*
     * Computes and stores the index of last occurence of the
     * characters in the pattern.
    */
    std::unordered_map<char, uint8_t> l;
    for (auto iter_b = pattern.cbegin(); iter_b != pattern.cend(); ++iter_b) {
        l[*iter_b] = (iter_b - pattern.cbegin()) + 2;
    }
    return l;
}

std::vector<uint8_t> boyer_moore_match(const std::string_view& text, const std::string_view& pattern)
{
    std::vector<uint8_t> indices;
    std::unordered_map<char, uint8_t> last = last_occur(pattern);
    size_t i, j, pattern_size, text_size;
    text_size = (text.size() - 1);
    i = j = pattern_size = (pattern.size() - 1);

    while(i <= text_size) {
        if (pattern[j] == text[i]) {
            if (j == 0) {
                indices.push_back(i);
                i = i + pattern_size + 1;
                j = pattern_size;
            }
            else {
                --i;
                --j;
            }
        }
        else {
            i = i + pattern_size - std::min(j, static_cast<size_t>(last[text[i]])) + 1;
            j = pattern_size;
        }
    }
    indices.shrink_to_fit();
    return indices;
}

int main()
{
    constexpr std::string_view pattern{"oom"};
    constexpr std::string_view text{"boomzoomloomfoammoondoomgloomroom"};
    std::vector<uint8_t> idxs = boyer_moore_match(text, pattern);
    for (auto i : idxs)
        std::cout << static_cast<size_t>(i) << " ";
    std::cout << std::endl;
    return 0;
}
