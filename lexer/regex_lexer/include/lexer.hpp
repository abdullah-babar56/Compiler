#pragma once
#include <string>
#include <vector>
#include "token.hpp"
#include "exception.hpp"

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source_;
    int line_;
    int column_;
    size_t pos_;
    void skipWhitespace();
    void handleMultiLineComment();
    Token getNextToken();
};