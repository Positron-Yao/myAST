#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

namespace ast {

enum class TokenType {
    // 字面量
    NUMBER,
    IDENT,

    // 运算符
    PLUS,   // +
    MINUS,  // -
    MUL,    // *
    DIV,    // /
    POW,    // **
    LPAREN, // (
    RPAREN, // )

    // 其他
    END,    // 结束
    ERROR,  // 错误
};

class Token {
    public:
        TokenType type;
        std::variant<double, std::string> value;
        size_t pos;

        Token(TokenType t, std::string const &lex, size_t pos=0): type(t), value(lex), pos(pos) {}
        Token(TokenType t, double val, size_t pos=0): type(t), value(val), pos(pos) {}

    private:
        struct Visitor {
            std::string operator()(double v) {return std::to_string(v);}
            std::string operator()(std::string v) {return v;}
        };

        struct NumVisitor {
            double operator()(double v) {return v;}
            double operator()(std::string v) {
                std::cerr << "Not a number: " << v << std::endl;
                throw std::runtime_error("Not a number");
            }
        };

        std::unordered_map<TokenType, std::string> token_names = {
            { TokenType::NUMBER, "NUMBER" },
            { TokenType::IDENT, "IDENT" },
            { TokenType::PLUS, "PLUS" },
            { TokenType::MINUS, "MINUS" },
            { TokenType::MUL, "MUL" },
            { TokenType::DIV, "DIV" },
            { TokenType::POW, "POW" },
            { TokenType::LPAREN, "LPAREN" },
            { TokenType::RPAREN, "RPAREN" },
            { TokenType::END, "END" },
            { TokenType::ERROR, "ERROR" },
        };

    public:
        std::string show();
        std::string get_str();
        double get_num();
};

} // ast
