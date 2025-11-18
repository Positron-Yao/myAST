#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/*
 * E -> T { (+|-) T }
 * T -> F { (*|/) F }
 * F -> ( E ) | number
 */

class Parser {
    public:
        Parser(string const &input): input(input), pos(0) {}

        double parse() {
            skip_whitespace();
            return parse_E();
        }

    private:
        string input;
        size_t pos;

        void skip_whitespace() {
            stringstream ss;
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

        double parse_E() {
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

        double parse_T() {
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

        double parse_F() {
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

        double parse_number() {
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
};

int main() {
    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        Parser parser(input);
        cout << "= " << parser.parse() << "\n";
    }
    return 0;
}
