#include "pattern.hpp"

const std::vector<std::pair<std::regex, TokenType>> Patterns::tokenPatterns = {
    // Comments (single-line) - put BEFORE operator "/" rule
    {std::regex("^//[^\\n]*"), TokenType::T_COMMENT},

    // Keywords (word boundary so intValue doesn't become 'int')
    {std::regex("^fn\\b"), TokenType::T_FUNCTION},
    {std::regex("^int\\b"), TokenType::T_INT},
    {std::regex("^float\\b"), TokenType::T_FLOAT},
    {std::regex("^string\\b"), TokenType::T_STRING},
    {std::regex("^bool\\b"), TokenType::T_BOOL},
    {std::regex("^return\\b"), TokenType::T_RETURN},
    {std::regex("^if\\b"), TokenType::T_IF},
    {std::regex("^else\\b"), TokenType::T_ELSE},
    {std::regex("^for\\b"), TokenType::T_FOR},
    {std::regex("^while\\b"), TokenType::T_WHILE},
    {std::regex("^break\\b"), TokenType::T_BREAK},
    {std::regex("^continue\\b"), TokenType::T_CONTINUE},
    {std::regex("^true\\b|^false\\b"), TokenType::T_BOOLLIT},

    // Literals: floats and hex first
    {std::regex("^\\.[0-9]+([eE][+-]?[0-9]+)?"), TokenType::T_FLOATLIT},
    {std::regex("^[0-9]+\\.[0-9]+([eE][+-]?[0-9]+)?"), TokenType::T_FLOATLIT},
    {std::regex("^[0-9]+\\.[0-9]+"), TokenType::T_FLOATLIT},
    {std::regex("^0[xX][0-9a-fA-F]+"), TokenType::T_INTLIT},

    // Invalid identifier that starts with digit AND has at least one letter/underscore after digits
    {std::regex("^[0-9]+[a-zA-Z_][a-zA-Z0-9_]*"), TokenType::T_INVALID_IDENTIFIER},

    // Decimal integer literal (pure digits)
    {std::regex("^[0-9]+"), TokenType::T_INTLIT},

    // String literal (no raw newline inside)
    {std::regex("^\"([^\"\\\\\\n]|\\\\.)*\""), TokenType::T_STRINGLIT},

    // Invalid identifier that starts with a letter/underscore BUT contains at least one invalid char
    {std::regex("^[a-zA-Z_][a-zA-Z0-9_][^a-zA-Z0-9_\\s;{}()\\[\\],=+\\-/%&|^~<>?:.\"]+[a-zA-Z0-9_]*"), TokenType::T_INVALID_IDENTIFIER},

    // Valid identifiers (only reach here when there are no invalid chars)
    {std::regex("^[a-zA-Z_][a-zA-Z0-9_]*"), TokenType::T_IDENTIFIER},

    // Operators (multi-character first)
    {std::regex("^=="), TokenType::T_EQUALSOP},
    {std::regex("^\\+\\+"), TokenType::T_INCREMENT},
    {std::regex("^\\+\\="), TokenType::T_PLUS_ASSIGN},
    {std::regex("^--"), TokenType::T_DECREMENT},
    {std::regex("^\\-\\="), TokenType::T_MINUS_ASSIGN},
    {std::regex("^<<"), TokenType::T_LEFTSHIFT},
    {std::regex("^>>"), TokenType::T_RIGHTSHIFT},
    {std::regex("^<="), TokenType::T_LTE},
    {std::regex("^>="), TokenType::T_GTE},
    {std::regex("^!="), TokenType::T_NEQ},
    {std::regex("^&&"), TokenType::T_AND},
    {std::regex("^\\|\\|"), TokenType::T_OR},

    // Single-character operators
    {std::regex("^="), TokenType::T_ASSIGNOP},
    {std::regex("^\\+"), TokenType::T_PLUS},
    {std::regex("^-"), TokenType::T_MINUS},
    {std::regex("^\\*"), TokenType::T_MULT},
    {std::regex("^/"), TokenType::T_DIV},
    {std::regex("^%"), TokenType::T_MOD},
    {std::regex("^<"), TokenType::T_LT},
    {std::regex("^>"), TokenType::T_GT},
    {std::regex("^!"), TokenType::T_NOT},

    // Bitwise operators (single char; & and | after &&/||)
    {std::regex("^&"), TokenType::T_BITAND},
    {std::regex("^\\|"), TokenType::T_BITOR},
    {std::regex("^\\^"), TokenType::T_BITXOR},
    {std::regex("^~"), TokenType::T_BITNOT},

    // Punctuation
    {std::regex("^\\("), TokenType::T_PARENL},
    {std::regex("^\\)"), TokenType::T_PARENR},
    {std::regex("^\\{"), TokenType::T_BRACEL},
    {std::regex("^\\}"), TokenType::T_BRACER},
    {std::regex("^\\["), TokenType::T_BRACKL},
    {std::regex("^\\]"), TokenType::T_BRACKR},
    {std::regex("^,"), TokenType::T_COMMA},
    {std::regex("^;"), TokenType::T_SEMICOLON},
    {std::regex("^:"), TokenType::T_COLON},
    {std::regex("^\\?"), TokenType::T_QUESTION},
    {std::regex("^\\."), TokenType::T_DOT}
};


