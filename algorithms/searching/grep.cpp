/*
 * A simple implementation of the grep search utility, which searches
 * for occurrence of a pattern in a file using BOYER-MOORE pattern
 * match algorithm. This utility reports the line and column numbers
 * of where the pattern matches.
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "boyer_moore.hpp"

namespace fs = std::filesystem;

std::vector<fs::path> VALID_FILES;
std::unordered_map<std::string, bool> VALID_EXTS{
    {".txt", true}, {".py", true}, {".cpp", true},
    {".cc", true}, {".h", true}, {".hpp", true},
    {".c", true}
};

void get_valid_files(const fs::path& f_path)
{
    if (fs::is_regular_file(f_path)) {
        if (VALID_EXTS[f_path.extension().string()])
            VALID_FILES.push_back(f_path);
    }
    else if (fs::is_directory(f_path)) {
        for (auto piter=fs::directory_iterator(f_path); piter!=fs::directory_iterator(); ++piter){
            get_valid_files(f_path/(*piter));
        }
    }
}

std::vector<std::pair<size_t, std::vector<size_t>>> search_file(const fs::path& f_path, const std::string_view& pattern)
{
    std::ifstream istrm(f_path, std::ios::in);
    std::string line;
    std::vector<std::pair<size_t, std::vector<size_t>>> match_indices;
    size_t line_no = 0;

    if (istrm.is_open()) {
        while (!istrm.eof()) {
            std::getline(istrm, line);
            std::vector<size_t> indices = boyer_moore_match(line, pattern);
            match_indices.push_back(std::make_pair(line_no, indices));
            ++line_no;
        }
    }
    match_indices.shrink_to_fit();
    return match_indices;
}

void print_matches(const std::vector<std::pair<size_t, std::vector<size_t>>>& indices, size_t pattern_length, const fs::path& fname)
{
    std::cout << fname << ": " << std::endl;
    for (auto iter=indices.begin(); iter!=indices.end(); ++iter) {
        if ((*iter).second.size()) {
            for (auto idx : (*iter).second)
                std::cout << "\t line " << (*iter).first << " column " << idx << " - " << (idx + pattern_length) << std::endl;
        }
    }
    std::cout << "\n" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "USAGE: grep.exe (path to a directory | path to file) (pattern)";
        std::exit(-1);
    }

    std::vector<std::pair<size_t, std::vector<size_t>>> match_indices;

    fs::path dir{argv[1]};
    dir = fs::absolute(dir);
    const std::string_view pattern = argv[2];
    std::cout << "\nsearching for \"" << pattern << "\" in " << dir << "\n" << std::endl;

    get_valid_files(dir); // get all valid files in the specified directory.

    for (auto fiter=VALID_FILES.begin(); fiter != VALID_FILES.end(); ++fiter) {
        match_indices = search_file(*fiter, pattern);
        print_matches(match_indices, pattern.size(), *fiter);
    }

    return 0;
}
