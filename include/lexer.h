#pragma once
#include <vector>
#include "token.h"

namespace ast {

class Lexer {
    private:
        std::string input;
        size_t pos;
        size_t line;
        size_t column;

        std::unordered_map<std::string, TokenType> operators = {
            { "+", TokenType::PLUS },
            { "-", TokenType::MINUS },
            { "*", TokenType::MUL },
            { "/", TokenType::DIV },
            { "**", TokenType::POW },
            { "(", TokenType::LPAREN },
            { ")", TokenType::RPAREN },
        };

    public:
        Lexer(std::string const &input): input(input), pos(0), line(1), column(1) {}

        std::vector<Token> tokenize();

    private:
        char peek() const;
        char peek_next() const;
        void advance();
        void skip_whitespace();
        Token next_token();
        /*
         * 读取数字字面值
         *
         * 他这个弄的更聪明一点，直接截取字符串然后转换数字
         */
        Token read_number();
        Token read_identifier();
        Token read_operator();
        bool is_operator_start(char c);
};

} // ast
