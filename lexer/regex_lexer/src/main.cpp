#include <iostream>
#include <fstream>
#include <string>
#include "lexer.hpp"
#include "utilis.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string source_code((std::istreambuf_iterator<char>(input_file)),
                            std::istreambuf_iterator<char>());
    input_file.close();

    try {
        Lexer lexer(source_code);
        std::vector<Token> tokens = lexer.tokenize();

        for (const auto& token : tokens) {
            if (token.type != TokenType::T_COMMENT) {
                std::cout << "Token(" << tokenTypeToString(token.type) << ", \"" << token.value
                          << "\") at line " << token.line << ", column " << token.column << std::endl;
            }
        }
    } catch (const LexerError& e) {
        std::cerr << "Lexical error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}