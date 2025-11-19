#include <iostream>
#include <string>
#include "parser.h"
using namespace std;

void repl() {
    std::string input;
    while (true) {
        std::cout << "> ";
        getline(std::cin, input);
        ast::Parser parser(input);
        std::cout << "= " << parser.parse() << "\n";
    }
}

int main() {
    ast::Lexer lexer("191_9810 * a + 1.14514 * shit + __inner_variable_1__ / 5 + 2 ** ( 350 + 234 )");
    vector<ast::Token> ts = lexer.tokenize();
    for (auto t: ts) {
        cout << t.show() << endl;
    }
    return 0;
}
