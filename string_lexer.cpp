#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <optional>

using namespace std;

enum TokenType {
    T_FUNCTION, T_INT, T_FLOAT, T_STRING, T_BOOL, T_RETURN,
    T_IF, T_ELSE, T_FOR, T_WHILE, T_BREAK, T_CONTINUE,
    T_IDENTIFIER, T_INTLIT, T_FLOATLIT, T_STRINGLIT, T_BOOLLIT,
    T_ASSIGNOP, T_EQUALSOP, T_PLUS, T_MINUS, T_MULT, T_DIV, T_MOD,
    T_LT, T_GT, T_LTE, T_GTE, T_NEQ,
    T_AND, T_OR, T_NOT,
    T_BITAND, T_BITOR, T_BITXOR, T_BITNOT, T_LEFTSHIFT, T_RIGHTSHIFT,
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER, T_BRACKL, T_BRACKR,
    T_COMMA, T_SEMICOLON, T_COLON, T_QUESTION, T_DOT,
    T_COMMENT, T_UNKNOWN, T_EOF
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};

class Lexer {
private:
    string source;
    size_t position;
    int line;
    int column;
    char current_char;
    
    unordered_map<string, TokenType> keywords = {
        {"fn", T_FUNCTION},
        {"int", T_INT},
        {"float", T_FLOAT},
        {"string", T_STRING},
        {"bool", T_BOOL},
        {"return", T_RETURN},
        {"if", T_IF},
        {"else", T_ELSE},
        {"for", T_FOR},
        {"while", T_WHILE},
        {"break", T_BREAK},
        {"continue", T_CONTINUE},
        {"true", T_BOOLLIT},
        {"false", T_BOOLLIT}
    };
    
    void advance() {
        if (position < source.length()) {
            if (current_char == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            current_char = source[position++];
        } else {
            current_char = '\0';
        }
    }
    
    char peek() const {
        if (position < source.length()) {
            return source[position];
        }
        return '\0';
    }
    
    void skip_whitespace() {
        while (current_char != '\0' && isspace(current_char)) {
            advance();
        }
    }
    
    Token make_token(TokenType type, const string& value) {
        return {type, value, line, (column - value.length())};
    }
    
    Token parse_identifier() {
        string result;
        int start_col = column;
        
        while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
            result += current_char;
            advance();
        }
        
        auto it = keywords.find(result);
        if (it != keywords.end()) {
            return make_token(it->second, result);
        }
        return make_token(T_IDENTIFIER, result);
    }
    
    Token parse_number() {
        string result;
        int start_col = column;
        bool is_float = false;
        
        // Parse integer part
        while (current_char != '\0' && isdigit(current_char)) {
            result += current_char;
            advance();
        }
        
        // Check for decimal point
        if (current_char == '.') {
            is_float = true;
            result += current_char;
            advance();
            
            // Parse fractional part
            while (current_char != '\0' && isdigit(current_char)) {
                result += current_char;
                advance();
            }
        }
        
        // Check for exponent
        if (current_char == 'e' || current_char == 'E') {
            is_float = true;
            result += current_char;
            advance();
            
            // Check for sign
            if (current_char == '+' || current_char == '-') {
                result += current_char;
                advance();
            }
            
            // Parse exponent digits
            while (current_char != '\0' && isdigit(current_char)) {
                result += current_char;
                advance();
            }
        }
        
        return make_token(is_float ? T_FLOATLIT : T_INTLIT, result);
    }
    
    Token parse_string() {
        string result;
        int start_col = column;
        advance(); // Skip opening quote
        
        while (current_char != '\0' && current_char != '"') {
            if (current_char == '\\') {
                advance(); // Skip backslash
                if (current_char == '\0') {
                    break;
                }
                // Handle escape sequences
                switch (current_char) {
                    case 'n': result += '\n'; break;
                    case 't': result += '\t'; break;
                    case 'r': result += '\r'; break;
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    default: result += current_char; break;
                }
                advance();
            } else {
                result += current_char;
                advance();
            }
        }
        
        if (current_char == '"') {
            advance(); // Skip closing quote
            return make_token(T_STRINGLIT, result);
        } else {
            return make_token(T_UNKNOWN, "Unclosed string");
        }
    }
    
    Token parse_comment() {
        string result;
        int start_col = column;
        
        if (peek() == '/') { // Single line comment
            advance(); // Skip first '/'
            advance(); // Skip second '/'
            
            while (current_char != '\0' && current_char != '\n') {
                result += current_char;
                advance();
            }
            
            return make_token(T_COMMENT, result);
        } else if (peek() == '*') { // Multi-line comment
            advance(); // Skip '/'
            advance(); // Skip '*'
            
            while (current_char != '\0') {
                if (current_char == '*' && peek() == '/') {
                    advance(); // Skip '*'
                    advance(); // Skip '/'
                    return make_token(T_COMMENT, result);
                }
                result += current_char;
                advance();
            }
            
            return make_token(T_UNKNOWN, "Unclosed comment");
        } else {
            return make_token(T_DIV, "/");
        }
    }
    
    Token parse_operator() {
        int start_col = column;
        char first_char = current_char;
        advance();
        
        switch (first_char) {
            case '=':
                if (current_char == '=') {
                    advance();
                    return make_token(T_EQUALSOP, "==");
                }
                return make_token(T_ASSIGNOP, "=");
                
            case '+':
                return make_token(T_PLUS, "+");
                
            case '-':
                return make_token(T_MINUS, "-");
                
            case '*':
                return make_token(T_MULT, "*");
                
            case '/':
                return parse_comment(); // Handles comments and division
                
            case '%':
                return make_token(T_MOD, "%");
                
            case '<':
                if (current_char == '=') {
                    advance();
                    return make_token(T_LTE, "<=");
                } else if (current_char == '<') {
                    advance();
                    return make_token(T_LEFTSHIFT, "<<");
                }
                return make_token(T_LT, "<");
                
            case '>':
                if (current_char == '=') {
                    advance();
                    return make_token(T_GTE, ">=");
                } else if (current_char == '>') {
                    advance();
                    return make_token(T_RIGHTSHIFT, ">>");
                }
                return make_token(T_GT, ">");
                
            case '!':
                if (current_char == '=') {
                    advance();
                    return make_token(T_NEQ, "!=");
                }
                return make_token(T_NOT, "!");
                
            case '&':
                if (current_char == '&') {
                    advance();
                    return make_token(T_AND, "&&");
                }
                return make_token(T_BITAND, "&");
                
            case '|':
                if (current_char == '|') {
                    advance();
                    return make_token(T_OR, "||");
                }
                return make_token(T_BITOR, "|");
                
            case '^':
                return make_token(T_BITXOR, "^");
                
            case '~':
                return make_token(T_BITNOT, "~");
                
            default:
                return make_token(T_UNKNOWN, string(1, first_char));
        }
    }
    
    Token parse_punctuation() {
        char c = current_char;
        advance();
        
        switch (c) {
            case '(': return make_token(T_PARENL, "(");
            case ')': return make_token(T_PARENR, ")");
            case '{': return make_token(T_BRACEL, "{");
            case '}': return make_token(T_BRACER, "}");
            case '[': return make_token(T_BRACKL, "[");
            case ']': return make_token(T_BRACKR, "]");
            case ',': return make_token(T_COMMA, ",");
            case ';': return make_token(T_SEMICOLON, ";");
            case ':': return make_token(T_COLON, ":");
            case '?': return make_token(T_QUESTION, "?");
            case '.': return make_token(T_DOT, ".");
            default: return make_token(T_UNKNOWN, string(1, c));
        }
    }

public:
    Lexer(const string& src) : source(src), position(0), line(1), column(1), current_char('\0') {
        if (!source.empty()) {
            current_char = source[0];
            position = 1;
        }
    }
    
    vector<Token> tokenize() {
        vector<Token> tokens;
        
        while (current_char != '\0') {
            skip_whitespace();
            
            if (current_char == '\0') break;
            
            if (isalpha(current_char) || current_char == '_') {
                tokens.push_back(parse_identifier());
            } else if (isdigit(current_char)) {
                tokens.push_back(parse_number());
            } else if (current_char == '"') {
                tokens.push_back(parse_string());
            } else if (ispunct(current_char)) {
                if (current_char == '/' && (peek() == '/' || peek() == '*')) {
                    Token comment = parse_comment();
                    if (comment.type != T_COMMENT) {
                        tokens.push_back(comment);
                    }
                } else {
                    Token op_token = parse_operator();
                    if (op_token.type == T_DIV) {
                        tokens.push_back(op_token);
                    } else if (op_token.type != T_COMMENT) {
                        tokens.push_back(op_token);
                    }
                }
            } else {
                tokens.push_back(parse_punctuation());
            }
        }
        
        tokens.push_back({T_EOF, "", line, column});
        return tokens;
    }
};

string tokenTypeToString(TokenType type) {
    switch(type) {
        case T_FUNCTION: return "T_FUNCTION";
        case T_INT: return "T_INT";
        case T_FLOAT: return "T_FLOAT";
        case T_STRING: return "T_STRING";
        case T_BOOL: return "T_BOOL";
        case T_RETURN: return "T_RETURN";
        case T_IF: return "T_IF";
        case T_ELSE: return "T_ELSE";
        case T_FOR: return "T_FOR";
        case T_WHILE: return "T_WHILE";
        case T_BREAK: return "T_BREAK";
        case T_CONTINUE: return "T_CONTINUE";
        case T_IDENTIFIER: return "T_IDENTIFIER";
        case T_INTLIT: return "T_INTLIT";
        case T_FLOATLIT: return "T_FLOATLIT";
        case T_STRINGLIT: return "T_STRINGLIT";
        case T_BOOLLIT: return "T_BOOLLIT";
        case T_ASSIGNOP: return "T_ASSIGNOP";
        case T_EQUALSOP: return "T_EQUALSOP";
        case T_PLUS: return "T_PLUS";
        case T_MINUS: return "T_MINUS";
        case T_MULT: return "T_MULT";
        case T_DIV: return "T_DIV";
        case T_MOD: return "T_MOD";
        case T_LT: return "T_LT";
        case T_GT: return "T_GT";
        case T_LTE: return "T_LTE";
        case T_GTE: return "T_GTE";
        case T_NEQ: return "T_NEQ";
        case T_AND: return "T_AND";
        case T_OR: return "T_OR";
        case T_NOT: return "T_NOT";
        case T_BITAND: return "T_BITAND";
        case T_BITOR: return "T_BITOR";
        case T_BITXOR: return "T_BITXOR";
        case T_BITNOT: return "T_BITNOT";
        case T_LEFTSHIFT: return "T_LEFTSHIFT";
        case T_RIGHTSHIFT: return "T_RIGHTSHIFT";
        case T_PARENL: return "T_PARENL";
        case T_PARENR: return "T_PARENR";
        case T_BRACEL: return "T_BRACEL";
        case T_BRACER: return "T_BRACER";
        case T_BRACKL: return "T_BRACKL";
        case T_BRACKR: return "T_BRACKR";
        case T_COMMA: return "T_COMMA";
        case T_SEMICOLON: return "T_SEMICOLON";
        case T_COLON: return "T_COLON";
        case T_QUESTION: return "T_QUESTION";
        case T_DOT: return "T_DOT";
        case T_COMMENT: return "T_COMMENT";
        case T_UNKNOWN: return "T_UNKNOWN";
        case T_EOF: return "T_EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    string program = R"(fn int my_fn(int x, float y) {
        // This is a comment
        string my_str = "Hello \"world\"!\n\tEscaped";
        bool my_bool = true;
        int result = x * y + 10;
        
        if (x <= 40 && y != 0) {
            for (int i = 0; i < 10; i++) {
                result += i & 0xFF;
            }
        }
        
        /* Multi-line
           comment */
        return result << 2;
    })";

    Lexer lexer(program);
    vector<Token> tokens = lexer.tokenize();
    
    for (auto &t : tokens) {
        if (t.type != T_COMMENT && t.type != T_EOF) {
            cout << "Token(" << tokenTypeToString(t.type) << ", \"" << t.value 
                 << "\") at line " << t.line << ", column " << t.column << endl;
        }
    }
    
    // Print EOF separately
    Token eof = tokens.back();
    cout << "Token(" << tokenTypeToString(eof.type) << ", \"" << eof.value 
         << "\") at line " << eof.line << ", column " << eof.column << endl;
    
    return 0;
}