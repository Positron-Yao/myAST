#include "parser.h"

/*
 * E -> T { (+|-) T }
 * T -> F { (*|/) F } | F ** F
 * F -> ( E ) | number
 */

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
    while (pos > input.size() && isspace(input[pos])) {
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
    // 暂时用不上

    // 运算符
    if (is_operator_start(current)) {
        return read_operator();
    }

    return Token(TokenType::ERROR, "Invalid character", pos);
}

/*
 * 读取数字字面值
 *
 * 他这个弄的更聪明一点，直接截取字符串然后转换数字
 */
Token Lexer::read_number() {
    size_t start = pos;
    while (pos < input.size() && isdigit(peek())) {
        advance();
    }

    // 浮点数
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (pos < input.size() && isdigit(peek())) {
            advance();
        }
    }

    std::string num_str = input.substr(start, pos - start);
    double value = stod(num_str);

    return Token(TokenType::NUMBER, value, start);
}

// [TODO]: 剩几个没写完
Token Lexer::read_operator() {return Token(TokenType::ERROR, "Not implemented", pos);}
bool Lexer::is_operator_start(char c) {return true;}

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
