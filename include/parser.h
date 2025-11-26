#pragma once
#include <vector>
#include "token.h"

namespace ast {

class Parser {
    public:
        Parser(std::vector<Token> input): input(input), pos(0) {}
        double parse();

    private:
        std::vector<Token> input;
        size_t pos;

        double parse_E();
        double parse_T();
        double parse_P();
        double parse_U();
        double parse_F();
        double parse_number();
};

} // ast
