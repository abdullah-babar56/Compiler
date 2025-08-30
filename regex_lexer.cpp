#include <bits/stdc++.h>
#include <regex>
using namespace std;

enum TokenType {
    T_FUNCTION, T_INT, T_FLOAT, T_STRING, T_BOOL, T_RETURN,
    T_IF, T_ELSE, T_FOR, T_WHILE,
    T_IDENTIFIER, T_INTLIT, T_FLOATLIT, T_STRINGLIT,
    T_ASSIGNOP, T_EQUALSOP, T_PLUS, T_MINUS, T_MULT, T_DIV,
    T_LT, T_GT, T_LTE, T_GTE, T_NEQ,
    T_AND, T_OR, T_NOT,
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER, T_BRACKL, T_BRACKR,
    T_COMMA, T_SEMICOLON, T_QUOTES,
    T_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

vector<pair<regex, TokenType>> tokenPatterns = {
    {regex("^fn"), T_FUNCTION},
    {regex("^int"), T_INT},
    {regex("^float"), T_FLOAT},
    {regex("^string"), T_STRING},
    {regex("^bool"), T_BOOL},
    {regex("^return"), T_RETURN},
    {regex("^if"), T_IF},
    {regex("^else"), T_ELSE},
    {regex("^for"), T_FOR},
    {regex("^while"), T_WHILE},
    {regex("^[a-zA-Z_][a-zA-Z0-9_]*"), T_IDENTIFIER},
    {regex("^[0-9]+"), T_INTLIT},
    {regex("^[0-9]+\\.[0-9]+"), T_FLOATLIT},
    {regex("^\"(\\\\.|[^\"])*\""), T_STRINGLIT}, // handles escapes
    {regex("^=="), T_EQUALSOP},
    {regex("^="), T_ASSIGNOP},
    {regex("^\\+"), T_PLUS},
    {regex("^-"), T_MINUS},
    {regex("^\\*"), T_MULT},
    {regex("^/"), T_DIV},
    {regex("^<="), T_LTE},
    {regex("^>="), T_GTE},
    {regex("^<"), T_LT},
    {regex("^>"), T_GT},
    {regex("^!="), T_NEQ},
    {regex("^&&"), T_AND},
    {regex("^\\|\\|"), T_OR},
    {regex("^!"), T_NOT},
    {regex("^\\("), T_PARENL},
    {regex("^\\)"), T_PARENR},
    {regex("^\\{"), T_BRACEL},
    {regex("^\\}"), T_BRACER},
    {regex("^\\["), T_BRACKL},
    {regex("^\\]"), T_BRACKR},
    {regex("^,"), T_COMMA},
    {regex("^;"), T_SEMICOLON},
    {regex("^\""), T_QUOTES}
};

vector<Token> tokenize(const string &src) {
    vector<Token> tokens;
    string code = src;
    smatch match;

    while (!code.empty()) {
        bool matched = false;
        code = regex_replace(code, regex("^\\s+"), ""); // skip whitespace

        for (auto &tp : tokenPatterns) {
            if (regex_search(code, match, tp.first)) {
                tokens.push_back({tp.second, match.str(0)});
                code = match.suffix().str();
                matched = true;
                break;
            }
        }

        if (!matched) {
            cerr << "Error: Unknown token starting at -> " << code << endl;
            tokens.push_back({T_UNKNOWN, string(1, code[0])});
            code.erase(0, 1);
        }
    }
    return tokens;
}

int main() {
    string program = R"(fn int my_fn(int x, float y) {
        string my_str = "hmm";
        bool my_bool = x == 40;
        return x;
    })";

    vector<Token> tokens = tokenize(program);
    for (auto &t : tokens) {
        cout << "Token(" << t.type << ", \"" << t.value << "\")" << endl;
    }
}
