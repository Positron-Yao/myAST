#include <cmath>
#include <iostream>
#include <stdexcept>
#include "parser.h"

/*
 * E: Expression
 * T: Term
 * P: Primary
 * U: Unary
 * F: Factor
 *
 * E -> T { (+|-) T }
 * T -> P { (*|/) P }
 * P -> U { ** P }
 * U -> ident ( F { , F } ) | F
 * F -> ( E ) | number | ident
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
    double left = parse_U();
    while (pos < input.size() && input[pos].type == ast::TokenType::POW) {
        pos++;
        double exponent = parse_P();
        left = std::pow(left, exponent);
    }
    return left;
}

double Parser::parse_U() {
    double left = 0;
    if (pos < input.size() && input[pos].type == ast::TokenType::IDENT) {
        if (pos + 1 < input.size() && input[pos + 1].type == ast::TokenType::LPAREN) {
            std::cout << "\tcall:\t" << input[pos].get_str() << "\n";
            pos += 2;
            // [TODO]: 实现函数调用解析
            // 设计函数参数栈
            // 解析逗号表达式的参数
            return left;
        }
    }
    left = parse_F();
    return left;
}

double Parser::parse_F() {
    if (pos < input.size()) { 
        switch (input[pos].type) {
            case ast::TokenType::LPAREN: {
                pos++;
                double result = parse_E();
                if (pos < input.size() && input[pos].type == ast::TokenType::RPAREN) {
                    pos++;
                    return result;
                } else {
                    throw std::runtime_error("Expected )");
                }
            }
            case ast::TokenType::IDENT:
                // [TODO]: 实现变量解析
                return 0;
            case ast::TokenType::NUMBER:
                return parse_number();
            default:
                return 0;
        }
    }
    return 0;
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
