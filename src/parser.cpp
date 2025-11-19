#include <cmath>
#include <stdexcept>
#include "parser.h"

/*
 * E -> T { (+|-) T }
 * T -> P { (*|/) P }
 * P -> F { ** P }
 * F -> ( E ) | number
 */

namespace ast {

// class Parser

double Parser::parse() {
    return parse_E();
}

double Parser::parse_E() {
    double left = parse_T();
    while (pos < input.size()) {
        switch (input[pos].type) {
            case ast::TokenType::PLUS:
                pos++;
                left += parse_T();
                break;
            case ast::TokenType::MINUS:
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
    double left = parse_P();
    while (pos < input.size()) {
        switch (input[pos].type) {
            case ast::TokenType::MUL:
                pos++;
                left *= parse_P();
                break;
            case ast::TokenType::DIV:
                pos++;
                {
                    double divisor = parse_P();
                    if (divisor == 0) {
                        throw std::runtime_error("Division by zero");
                    }
                    left /= divisor;
                }
                break;
            default:
                return left;
        }
    }
    return left;
}

double Parser::parse_P() {
    double left = parse_F();
    while (pos < input.size() && input[pos].type == ast::TokenType::POW) {
        pos++;
        double exponent = parse_P();
        left = std::pow(left, exponent);
    }
    return left;
}

double Parser::parse_F() {
    if (pos < input.size() && input[pos].type == ast::TokenType::LPAREN) {
        pos++;
        double result = parse_E();
        if (pos < input.size() && input[pos].type == ast::TokenType::RPAREN) {
            pos++;
            return result;
        } else {
            throw std::runtime_error("Expected )");
        }
    } else {
        return parse_number();
    }
}

double Parser::parse_number() {
    double result = 0;
    if (input[pos].type == ast::TokenType::NUMBER) {
        result = input[pos].get_num();
        pos++;
    }
    return result;
}

} // ast
