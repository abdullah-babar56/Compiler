#include "lexer.hpp"
#include "pattern.hpp"
#include <regex>
#include <iostream>

Lexer::Lexer(const std::string& source) : source_(source), line_(1), column_(1), pos_(0) {}

void Lexer::skipWhitespace() {
    std::smatch match;
    const std::string current = source_.substr(pos_);
    if (std::regex_search(current, match, std::regex("^\\s+"))) {
        for (char c : match.str()) {
            if (c == '\n') {
                line_++;
                column_ = 1;
            } else {
                column_++;
            }
        }
        pos_ += match.length();
    }
}

void Lexer::handleMultiLineComment() {
    std::smatch match;
    const std::string current = source_.substr(pos_);
    if (std::regex_search(current, match, std::regex("^/\\*"))) {
        pos_ += 2;
        column_ += 2;
        size_t end_pos = source_.find("*/", pos_);
        if (end_pos == std::string::npos) {
            throw LexerError("Unclosed multi-line comment at line " + std::to_string(line_) +
                            ", column " + std::to_string(column_));
        }
        std::string comment_content = source_.substr(pos_, end_pos - pos_);
        for (char c : comment_content) {
            if (c == '\n') {
                line_++;
                column_ = 1;
            } else {
                column_++;
            }
        }
        pos_ = end_pos + 2;
        column_ += 2;
    }
}

Token Lexer::getNextToken() {
    const std::string current = source_.substr(pos_);
    for (const auto& pattern_pair : Patterns::tokenPatterns) {
        const std::regex& pattern = pattern_pair.first;
        TokenType type = pattern_pair.second;
        std::smatch match;
        if (std::regex_search(current, match, pattern)) {
            std::string token_value = match.str();
            Token token{type, token_value, line_, column_};
            if (type == TokenType::T_COMMENT) {
                for (char c : token_value) {
                    if (c == '\n') {
                        line_++;
                        column_ = 1;
                    } else {
                        column_++;
                    }
                }
                pos_ += match.length();
                return {TokenType::T_COMMENT, "", line_, column_}; // Return empty token for comments
            }
            if (type == TokenType::T_INVALID_IDENTIFIER) {
                std::cerr << "Error: Invalid identifier '" << token_value << "' at line " << line_
                          << ", column " << column_ << std::endl;
            }
            for (char c : token_value) {
                if (c == '\n') {
                    line_++;
                    column_ = 1;
                } else {
                    column_++;
                }
            }
            pos_ += match.length();
            return token;
        }
    }
    std::string unknown_char(1, source_[pos_]);
    std::cerr << "Error: Unknown token at line " << line_ << ", column " << column_
              << " -> '" << unknown_char << "'" << std::endl;
    Token token{TokenType::T_UNKNOWN, unknown_char, line_, column_};
    pos_++;
    column_++;
    return token;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos_ < source_.length()) {
        skipWhitespace();
        if (pos_ >= source_.length()) break;
        handleMultiLineComment();
        if (pos_ >= source_.length()) break;
        Token token = getNextToken();
        if (token.type != TokenType::T_COMMENT) {
            tokens.push_back(token);
        }
    }
    tokens.push_back({TokenType::T_EOF, "", line_, column_});
    return tokens;
}