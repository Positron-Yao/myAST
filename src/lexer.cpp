#include <iostream>
#include <sstream>
#include <stdexcept>
#include "lexer.h"
#include "token.h"

namespace ast {

// class Lexer

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = next_token();
    while (token.type != TokenType::END) {
        if (token.type == TokenType::ERROR) {
            std::cerr << "Error: " << token.show() << "\n  at line " << line << ", column " << column << std::endl;
            throw std::runtime_error("Syntax error");
            return tokens;
        }
        tokens.push_back(token);
        token = next_token();
    }
    tokens.push_back(token);
    return tokens;
}

char Lexer::peek() const {
    return (pos < input.size() ? input[pos] : 0);
}

char Lexer::peek_next() const {
    return (pos + 1 < input.size() ? input[pos + 1] : 0);
}

void Lexer::advance() {
    if (pos < input.size()) {
        if (input[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

void Lexer::skip_whitespace() {
    while (pos < input.size() && isspace(input[pos])) {
        advance();
    }
}

Token Lexer::next_token() {
    skip_whitespace();

    if (pos >= input.size()) {
        return Token(TokenType::END, "", pos);
    }

    char current = peek();

    // 数字字面值
    if (isdigit(current)) {
        return read_number();
    }

    // 标识符 / 关键字
    if (isalpha(current) || current == '_') {
        return read_identifier();
    }

    // 运算符
    if (is_operator_start(current)) {
        return read_operator();
    }

    return Token(TokenType::ERROR, "Invalid character", pos);
}

Token Lexer::read_number() {
    size_t start = pos;
    // 允许数字字面值中出现'_'
    while (pos < input.size() && (isdigit(peek()) || peek() == '_')) {
        advance();
    }

    // 浮点数
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (pos < input.size() && (isdigit(peek()) || peek() == '_')) {
            advance();
        }
    }

    std::string num_str = input.substr(start, pos - start);
    // 去除'_'
    std::stringstream ss;
    for (char c: num_str) {
        if (c != '_') {
            ss << c;
        }
    }
    double value = stod(ss.str());

    return Token(TokenType::NUMBER, value, start);
}

Token Lexer::read_identifier() {
    size_t start = pos;
    while (pos < input.size() && (isalpha(peek()) || peek() == '_' || isdigit(peek()))) {
        advance();
    }

    std::string id_str = input.substr(start, pos - start);
    return Token(TokenType::IDENT, id_str, start);
}

Token Lexer::read_operator() {
    size_t start = pos;
    char current;
    switch(current = peek()) {
        case '*':
            advance();
            switch (current = peek()) {
                case '*':
                    // power
                    advance();
            }
            break;
        case '+':
        case '-':
        case '/':
        case '(':
        case ')':
            advance();
            break;
    }
    std::string op_str = input.substr(start, pos - start);
    return Token(operators[op_str], op_str, start);
}

bool Lexer::is_operator_start(char c) {
    return c == '+' || c == '-' || c == '/' || c == '*' || c == '(' || c == ')';
}

} // ast
