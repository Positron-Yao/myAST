#include "token.h"

namespace ast {

// class Token

std::string Token::show() {
    return Token::token_names[type] + " \t(" + std::to_string(pos) + "): \t" + std::visit(Token::Visitor{}, value);
}

double Token::get_num() {
    return std::visit(Token::NumVisitor{}, value);
}

} // ast
