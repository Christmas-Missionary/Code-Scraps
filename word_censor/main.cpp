// https://old.reddit.com/r/dailyprogrammer/comments/106gse/9202012_challenge_100_intermediate_bad_word_filter/

#define _LIBCPP_REMOVE_TRANSITIVE_INCLUDES
#include <__algorithm/count_if.h>
#include <fstream>
#include <iostream>
#include "censor.hpp"

static std::vector<std::string> get_all_bad_words(std::string const& file_path){
    std::ifstream reader(file_path);
    std::vector<std::string> res;
    res.reserve(
        static_cast<unsigned>(
        std::count_if(
        std::istreambuf_iterator<char>{reader}, {},
        [](char chr) {return chr == '\n';})) + 1U);
    reader.seekg(0);
    std::string line;
    while (std::getline(reader, line)){
        if (line == "\r"){
            continue;
        }
        if (*(line.end() - 1) == '\r'){
            line.pop_back();
        }
        res.push_back(line);
    }
    reader.close();
    return res;
}

std::string get_file_path(){
    std::string file_path {};
    std::cout << "Please drag and drop the list you want.\n";
    std::cin >> file_path;
    if (file_path.at(file_path.size() - 1) == '\47'){
        file_path.pop_back();
    }
    if (file_path.at(0) == '\47'){
        file_path.erase(0, 1);
    }
    return file_path;
}

int main(){
    std::string file_path = get_file_path();

    censor filter = {get_all_bad_words(file_path), '*'};
    std::string full_input;
    std::cout << "Type stuff please, or type q to quit program.\n";
    while (true) {
        std::getline(std::cin, full_input);
        if (full_input == "--quit"){
            return 0;
        }
        if (full_input == "--reload"){
            filter.set_list(get_all_bad_words(file_path));
            std::cout << "Reloaded.\n";
            continue;
        }
        if (full_input == "--new"){
            file_path = get_file_path();
            filter.set_list(get_all_bad_words(file_path));
            std::cout << "New list loaded.\n";
            continue;
        }

        if (full_input.size() >= 10 && full_input.substr(0, 9) == "--censor "){
            filter.censoring_char = full_input[9];
            std::cout << "Changed censoring char to " << filter.censoring_char << ".\n";
            continue;
        }
        std::cout << filter(full_input) << '\n';
    }

    return 0;
}
