#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    STRING,
    OPERATOR,
    PUNCTUATION,
    COMMENT,
    WHITESPACE,
    INVALID_IDENTIFIER,
    INCLUDE_DIRECTIVE
};

struct Token {
    string value;
    TokenType type;
};

bool isKeyword(const string& token) {
    string keywords[] = {"int", "float", "if", "else", "while", "for", "return"};
    return find(begin(keywords), end(keywords), token) != end(keywords);
}

bool isIdentifier(char c) {
    return isalpha(c) || c == '_' || c == '#';
}

bool isNumber(const string& token) {
    if (token.empty()) {
        return false;
    }

    size_t start = (token[0] == '-') ? 1 : 0;
    return all_of(token.begin() + start, token.end(), [](char c) { return isdigit(c); });
}

bool isOperator(char c) {
    string operators = "+-*/=%<>&|!";
    return operators.find(c) != string::npos;
}

bool isPunctuation(char c) {
    string punctuations = "{}[],;()";
    return punctuations.find(c) != string::npos;
}

bool isWhitespace(char c) {
    return isspace(c);
}

bool isComment(const string& token) {
    return token.size() >= 2 && token.substr(0, 2) == "//";
}

Token getTokenType(const string& token) {
    Token t;
    if (isKeyword(token)) {
        t.value = token;
        t.type = KEYWORD;
    } else if (isNumber(token)) {
        t.value = token;
        if (token.find('.') != string::npos) {
            t.type = FLOAT;
        } else {
            t.type = INTEGER;
        }
    } else if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
        t.value = token;
        t.type = STRING;
    } else if (isOperator(token[0])) {
        t.value = token;
        t.type = OPERATOR;
    } else if (isPunctuation(token[0])) {
        t.value = token;
        t.type = PUNCTUATION;
    } else if (isComment(token)) {
        t.value = token;
        t.type = COMMENT;
    } else if (all_of(token.begin(), token.end(), isWhitespace)) {
        t.value = token;
        t.type = WHITESPACE;
    } else if (token == "#include") {
        t.value = token;
        t.type = INCLUDE_DIRECTIVE;
    } else if (token[0] == '#' && token.size() > 1 && isalpha(token[1])) {
        t.value = token;
        t.type = INVALID_IDENTIFIER;
    } else if (isIdentifier(token[0])) {
        t.value = token;
        t.type = IDENTIFIER; // Treat unknown sequences as identifiers
    } else {
        t.value = token;
        t.type = INVALID_IDENTIFIER;
    }
    return t;
}

string getTokenTypeString(TokenType type) {
    switch (type) {
        case KEYWORD:
            return "Keyword";
        case IDENTIFIER:
            return "Identifier";
        case INTEGER:
            return "Integer";
        case FLOAT:
            return "Float";
        case STRING:
            return "String";
        case OPERATOR:
            return "Operator";
        case PUNCTUATION:
            return "Punctuation";
        case COMMENT:
            return "Comment";
        case WHITESPACE:
            return "Whitespace";
        case INVALID_IDENTIFIER:
            return "Invalid Identifier";
        case INCLUDE_DIRECTIVE:
            return "Include Directive";
    }
}

int main() {
    ifstream file("input.txt");
    if (!file) {
        cerr << "Error opening file.\n";
        return 1;
    }

    char ch;
    string token;

    while (file.get(ch)) {
        if (isWhitespace(ch) || isPunctuation(ch) || isOperator(ch)) {
            if (!token.empty()) {
                Token t = getTokenType(token);
                cout << "Token: " << t.value << " - Type: " << getTokenTypeString(t.type) << endl;
                token.clear();
            }
            if (isPunctuation(ch) || isOperator(ch)) {
                token.push_back(ch);
                Token t = getTokenType(token);
                cout << "Token: " << t.value << " - Type: " << getTokenTypeString(t.type) << endl;
                token.clear();
            }
        } else {
            token.push_back(ch);
        }
    }

    return 0;
}
