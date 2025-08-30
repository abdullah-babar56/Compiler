#include <bits/stdc++.h>
using namespace std;

enum TokenType {
    T_FUNCTION, T_INT, T_FLOAT, T_STRING, T_BOOL, T_RETURN,
    T_IDENTIFIER, T_INTLIT, T_FLOATLIT, T_STRINGLIT,
    T_ASSIGNOP, T_EQUALSOP,
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER,
    T_COMMA, T_SEMICOLON,
    T_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

bool isKeyword(const string &word, TokenType &type) {
    if (word == "fn") { type = T_FUNCTION; return true; }
    if (word == "int") { type = T_INT; return true; }
    if (word == "float") { type = T_FLOAT; return true; }
    if (word == "string") { type = T_STRING; return true; }
    if (word == "bool") { type = T_BOOL; return true; }
    if (word == "return") { type = T_RETURN; return true; }
    return false;
}

vector<Token> tokenize(const string &src) {
    vector<Token> tokens;
    int i = 0, n = src.size();

    while (i < n) {
        if (isspace(src[i])) { i++; continue; }

        // Identifier or keyword
        if (isalpha(src[i]) || src[i] == '_') {
            string word;
            while (i < n && (isalnum(src[i]) || src[i] == '_')) word.push_back(src[i++]);

            TokenType type;
            if (isKeyword(word, type)) tokens.push_back({type, word});
            else tokens.push_back({T_IDENTIFIER, word});
            continue;
        }

        // Numbers
        if (isdigit(src[i])) {
            string num;
            bool isFloat = false;
            while (i < n && (isdigit(src[i]) || src[i] == '.')) {
                if (src[i] == '.') isFloat = true;
                num.push_back(src[i++]);
            }
            tokens.push_back({isFloat ? T_FLOATLIT : T_INTLIT, num});
            continue;
        }

        // String literal
        if (src[i] == '"') {
            string lit; lit.push_back(src[i++]);
            while (i < n && src[i] != '"') {
                if (src[i] == '\\' && i+1 < n) { // handle escapes
                    lit.push_back(src[i++]);
                }
                lit.push_back(src[i++]);
            }
            if (i < n) lit.push_back(src[i++]); // closing "
            tokens.push_back({T_STRINGLIT, lit});
            continue;
        }

        // Operators and punctuation
        if (src[i] == '=') {
            if (i+1 < n && src[i+1] == '=') {
                tokens.push_back({T_EQUALSOP, "=="}); i+=2;
            } else {
                tokens.push_back({T_ASSIGNOP, "="}); i++;
            }
            continue;
        }
        if (src[i] == '(') { tokens.push_back({T_PARENL, "("}); i++; continue; }
        if (src[i] == ')') { tokens.push_back({T_PARENR, ")"}); i++; continue; }
        if (src[i] == '{') { tokens.push_back({T_BRACEL, "{"}); i++; continue; }
        if (src[i] == '}') { tokens.push_back({T_BRACER, "}"}); i++; continue; }
        if (src[i] == ',') { tokens.push_back({T_COMMA, ","}); i++; continue; }
        if (src[i] == ';') { tokens.push_back({T_SEMICOLON, ";"}); i++; continue; }

        // Unknown token
        tokens.push_back({T_UNKNOWN, string(1, src[i])});
        i++;
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
