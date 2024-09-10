#include "censor.hpp"

#include <__algorithm/for_each.h>
#include <__algorithm/sort.h>

#include <cctype>

namespace { // static

void inline generalize_string(std::string &str){
    std::for_each(str.begin(), str.end(),
    [](char &chr){
        chr = (chr == '-' || chr == '_') ? ' ' :
        static_cast<char>(std::tolower(static_cast<unsigned char>(chr)));
    });
}

void generalize_list(std::vector<std::string> &list){
    std::erase_if(list, [](std::string const& str){
        return str.empty();
    });
    std::sort(list.begin(), list.end(),
    [](auto former, auto latter){
        return former.size() > latter.size();
    });
    std::for_each(list.begin(), list.end(), generalize_string);
}

void base_censor(std::vector<std::string> const& list, std::string &to_censor, const char censoring_char){
    const std::string original_input = to_censor;
    generalize_string(to_censor);

    // censor
    for (std::string const& bad_word: list){
        size_t pos = to_censor.find(bad_word);
        while (pos != std::string::npos){
            memset(&to_censor[pos], censoring_char, bad_word.size());
            pos = to_censor.find(bad_word);
        }
    }

    // if not an asterisk, be original char.
    for (size_t i = 0; i < to_censor.size(); i++){
        if (to_censor[i] != censoring_char){
            to_censor[i] = original_input[i];
        }
    }
}

} // ! end of static

[[nodiscard]] std::string censor::quick(std::vector<std::string> list, std::string to_censor, const char censoring_char){
    generalize_list(list);
    base_censor(list, to_censor, censoring_char);
    return to_censor;
}

censor::censor(std::vector<std::string> _list, char _censoring_char): list(std::move(_list)), censoring_char(_censoring_char){
    generalize_list(list);
}

void censor::set_list(std::vector<std::string> const& _list){
    list = _list;
    generalize_list(list);
}

[[nodiscard]] std::string censor::operator()(std::string input) const {
    base_censor(list, input, censoring_char);
    return input;
}
