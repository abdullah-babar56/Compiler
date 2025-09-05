#pragma once

#include <regex>
#include <vector>
#include "token.hpp"

class Patterns {
public:
    static const std::vector<std::pair<std::regex, TokenType>> tokenPatterns;
};
