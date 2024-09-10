#pragma once
#include <string>
#include <vector>

struct censor {
private:
    std::vector<std::string> list;

public:
    [[nodiscard]] static std::string quick(std::vector<std::string> list, std::string to_censor, char censoring_char);

    char censoring_char;

    censor(std::vector<std::string> _list, char _censoring_char);

    void set_list(std::vector<std::string> const& _list);

    [[nodiscard]] std::string operator()(std::string input) const;
};
