#include <cctype>
#include <sstream>
#include "parser.h"

/*
 * E -> T { (+|-) T }
 * T -> F { (*|/) F } | F ** F
 * F -> ( E ) | number
 */

namespace ast {

// class Token

std::string Token::show() {
    return Token::token_names[type] + " \t(" + std::to_string(pos) + "): \t" + std::visit(Token::Visitor{}, value);
}

// class Lexer

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = next_token();
    while (token.type != TokenType::END) {
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
        case '+':
        case '-':
        case '/':
        case '(':
        case ')':
        case '*':
            advance();
            switch (current = peek()) {
                case '*':
                    // power
                    advance();
            }
    }
    std::string op_str = input.substr(start, pos - start);
    return Token(operators[op_str], op_str, start);
}

bool Lexer::is_operator_start(char c) {
    return c == '+' || c == '-' || c == '/' || c == '*' || c == '(' || c == ')';
}

// class Parser

double Parser::parse() {
    skip_whitespace();
    return parse_E();
}

void Parser::skip_whitespace() {
    std::stringstream ss;
    while (pos < input.size()) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        ss << input[pos];
        pos++;
    }
    input = ss.str();
    pos = 0;
}

double Parser::parse_E() {
    double left = parse_T();
    while (pos < input.size()) {
        switch (input[pos]) {
            case '+':
                pos++;
                left += parse_T();
                break;
            case '-':
                pos++;
                left -= parse_T();
                break;
            default:
                return left;
        }
    }
    return left;
}

double Parser::parse_T() {
    double left = parse_F();
    while (pos < input.size()) {
        switch (input[pos]) {
            case '*':
                pos++;
                left *= parse_F();
                break;
            case '/':
                pos++;
                left /= parse_F();
                break;
            default:
                return left;
        }
    }
    return left;
}

double Parser::parse_F() {
    if (pos < input.size() && input[pos] == '(') {
        pos++;
        double result = parse_E();
        if (pos < input.size() && input[pos] == ')') {
            pos++;
            return result;
        } else {
            throw "Expected )";
        }
    } else {
        return parse_number();
    }
}

double Parser::parse_number() {
    double result = 0;
    while (pos < input.size()) {
        if (isdigit(input[pos])) {
            result = result * 10 + (input[pos] - '0');
            pos++;
        } else if (input[pos] == '_') {
            pos++;
        } else if (input[pos] == '.') {
            pos++;
            double decimal = 1;
            while (pos < input.size() && isdigit(input[pos])) {
                decimal *= 0.1;
                result += (input[pos] - '0') * decimal;
                pos++;
            }
            return result;
        } else {
            return result;
        }
    }
    return result;
}

} // ast
