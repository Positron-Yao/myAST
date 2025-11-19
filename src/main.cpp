#include <iostream>
#include "parser.h"
using namespace std;

int main() {
    std::string input;
    while (true) {
        std::cout << "> ";
        getline(std::cin, input);
        Parser parser(input);
        std::cout << "= " << parser.parse() << "\n";
    }
    return 0;
}
