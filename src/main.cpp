#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
using namespace std;

void repl() {
    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        ast::Lexer lexer(input);
        vector<ast::Token> ts = lexer.tokenize();
        for (auto t: ts) {
            cout << t.show() << "\n";
        }
        ast::Parser parser(ts);
        double res = parser.parse();
        cout << "= " << res << "\n";
    }
}

void lexer_repl() {
    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        ast::Lexer lexer(input);
        vector<ast::Token> ts = lexer.tokenize();
        for (auto t: ts) {
            cout << t.show() << "\n";
        }
    }
}

int main() {
    repl();
    return 0;
}
