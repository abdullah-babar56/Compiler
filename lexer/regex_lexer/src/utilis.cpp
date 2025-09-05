#include "utilis.hpp"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::T_FUNCTION: return "T_FUNCTION";
        case TokenType::T_INT: return "T_INT";
        case TokenType::T_FLOAT: return "T_FLOAT";
        case TokenType::T_STRING: return "T_STRING";
        case TokenType::T_BOOL: return "T_BOOL";
        case TokenType::T_RETURN: return "T_RETURN";
        case TokenType::T_IF: return "T_IF";
        case TokenType::T_ELSE: return "T_ELSE";
        case TokenType::T_FOR: return "T_FOR";
        case TokenType::T_WHILE: return "T_WHILE";
        case TokenType::T_BREAK: return "T_BREAK";
        case TokenType::T_CONTINUE: return "T_CONTINUE";
        case TokenType::T_IDENTIFIER: return "T_IDENTIFIER";
        case TokenType::T_INTLIT: return "T_INTLIT";
        case TokenType::T_FLOATLIT: return "T_FLOATLIT";
        case TokenType::T_STRINGLIT: return "T_STRINGLIT";
        case TokenType::T_BOOLLIT: return "T_BOOLLIT";
        case TokenType::T_ASSIGNOP: return "T_ASSIGNOP";
        case TokenType::T_EQUALSOP: return "T_EQUALSOP";
        case TokenType::T_PLUS: return "T_PLUS";
        case TokenType::T_MINUS: return "T_MINUS";
        case TokenType::T_MULT: return "T_MULT";
        case TokenType::T_DIV: return "T_DIV";
        case TokenType::T_MOD: return "T_MOD";
        case TokenType::T_LT: return "T_LT";
        case TokenType::T_GT: return "T_GT";
        case TokenType::T_LTE: return "T_LTE";
        case TokenType::T_GTE: return "T_GTE";
        case TokenType::T_NEQ: return "T_NEQ";
        case TokenType::T_AND: return "T_AND";
        case TokenType::T_OR: return "T_OR";
        case TokenType::T_NOT: return "T_NOT";
        case TokenType::T_BITAND: return "T_BITAND";
        case TokenType::T_BITOR: return "T_BITOR";
        case TokenType::T_BITXOR: return "T_BITXOR";
        case TokenType::T_BITNOT: return "T_BITNOT";
        case TokenType::T_LEFTSHIFT: return "T_LEFTSHIFT";
        case TokenType::T_RIGHTSHIFT: return "T_RIGHTSHIFT";
        case TokenType::T_PARENL: return "T_PARENL";
        case TokenType::T_PARENR: return "T_PARENR";
        case TokenType::T_BRACEL: return "T_BRACEL";
        case TokenType::T_BRACER: return "T_BRACER";
        case TokenType::T_BRACKL: return "T_BRACKL";
        case TokenType::T_BRACKR: return "T_BRACKR";
        case TokenType::T_COMMA: return "T_COMMA";
        case TokenType::T_SEMICOLON: return "T_SEMICOLON";
        case TokenType::T_COLON: return "T_COLON";
        case TokenType::T_QUESTION: return "T_QUESTION";
        case TokenType::T_DOT: return "T_DOT";
        case TokenType::T_COMMENT: return "T_COMMENT";
        case TokenType::T_UNKNOWN: return "T_UNKNOWN";
        case TokenType::T_EOF: return "T_EOF";
        case TokenType::T_INVALID_IDENTIFIER: return "T_INVALID_IDENTIFIER";
        case TokenType::T_INCREMENT: return "T_INCREMENT";
        case TokenType::T_PLUS_ASSIGN: return "T_PLUS_ASSIGN";
        case TokenType::T_DECREMENT: return "T_DECREMENT";
        case TokenType::T_MINUS_ASSIGN: return "T_MINUS_ASSIGN";
        default: return "UNKNOWN";
    }
}