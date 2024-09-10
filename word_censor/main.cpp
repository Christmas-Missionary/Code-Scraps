/*

Write a program called 'censor' that takes in one argument on the command line.
This argument is the filename of a newline-separated wordlist of profanity such as
http://urbanoalvarez.es/blog/2008/04/04/bad-words-list/ or
http://www.bannedwordlist.com/SwearWordResources.htm
The program should then read a text from standard in, and print it out again,
but replacing every instance of a word in the wordlist with a 'censored' version.
The censored version of a word has the same first character as the word,
and the rest of the characters are replaced with '*'.
For example, the 'censored' version of 'peter' would be 'p****'

Example:
>echo 'You jerkface!' | censor badwords.txt
You j***face!

*/

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

int main(){
    std::string file_path {};

    std::cout << "Please drag and drop the list you want.\n";
    std::cin >> file_path;
    // std::cin.ignore();


    censor filter = {get_all_bad_words(file_path), '*'};
    std::string full_input;
    std::cout << "Type shit please, or type q to quit program.\n";
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
        if (full_input.size() >= 10 && full_input.substr(0, 9) == "--censor "){
            filter.censoring_char = full_input[9];
            std::cout << "Changed censoring char to " << filter.censoring_char << ".\n";
            continue;
        }
        std::cout << filter(full_input) << '\n';
    }

    return 0;
}
