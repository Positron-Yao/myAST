#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

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
};

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

class Parser {
    public:
        Parser(std::string const &input): input(input), pos(0) {}
        double parse();

    private:
        std::string input;
        size_t pos;

        void skip_whitespace();
        double parse_E();
        double parse_T();
        double parse_F();
        double parse_number();
};

} // ast
