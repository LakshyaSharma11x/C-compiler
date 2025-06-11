#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>
#include <map>

using namespace std;

// === TOKENIZER ===

enum class TokenType {
    INT, RETURN, IF, WHILE, IDENTIFIER, NUMBER,
    ASSIGN, PLUS, MINUS, MUL, DIV,
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, SEMICOLON,
    LT, GT, EQ, NEQ,
    END
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
    string input;
    size_t pos = 0;

public:
    Lexer(const string& src) : input(src) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < input.length()) {
            char ch = input[pos];
            if (isspace(ch)) {
                ++pos;
            } else if (isalpha(ch)) {
                string word;
                while (isalnum(input[pos])) word += input[pos++];
                if (word == "int") tokens.push_back({TokenType::INT, word});
                else if (word == "return") tokens.push_back({TokenType::RETURN, word});
                else if (word == "if") tokens.push_back({TokenType::IF, word});
                else if (word == "while") tokens.push_back({TokenType::WHILE, word});
                else tokens.push_back({TokenType::IDENTIFIER, word});
            } else if (isdigit(ch)) {
                string number;
                while (isdigit(input[pos])) number += input[pos++];
                tokens.push_back({TokenType::NUMBER, number});
            } else {
                switch (ch) {
                    case '=':
                        if (input[pos + 1] == '=') {
                            tokens.push_back({TokenType::EQ, "=="});
                            pos += 2;
                        } else {
                            tokens.push_back({TokenType::ASSIGN, "="});
                            pos++;
                        }
                        break;
                    case '!':
                        if (input[pos + 1] == '=') {
                            tokens.push_back({TokenType::NEQ, "!="});
                            pos += 2;
                        } else {
                            throw runtime_error("Unknown token !");
                        }
                        break;
                    case '<': tokens.push_back({TokenType::LT, "<"}); ++pos; break;
                    case '>': tokens.push_back({TokenType::GT, ">"}); ++pos; break;
                    case '+': tokens.push_back({TokenType::PLUS, "+"}); ++pos; break;
                    case '-': tokens.push_back({TokenType::MINUS, "-"}); ++pos; break;
                    case '*': tokens.push_back({TokenType::MUL, "*"}); ++pos; break;
                    case '/': tokens.push_back({TokenType::DIV, "/"}); ++pos; break;
                    case '(': tokens.push_back({TokenType::L_PAREN, "("}); ++pos; break;
                    case ')': tokens.push_back({TokenType::R_PAREN, ")"}); ++pos; break;
                    case '{': tokens.push_back({TokenType::L_BRACE, "{"}); ++pos; break;
                    case '}': tokens.push_back({TokenType::R_BRACE, "}"}); ++pos; break;
                    case ';': tokens.push_back({TokenType::SEMICOLON, ";"}); ++pos; break;
                    default:
                        throw runtime_error(string("Unknown character: ") + ch);
                }
            }
        }

        tokens.push_back({TokenType::END, ""});
        return tokens;
    }
};

// === PARSER ===

class Parser {
    vector<Token> tokens;
    size_t index = 0;
    map<string, string> symbolTable;
    int tempId = 0;
    int labelId = 0;

    Token peek() { return tokens[index]; }
    Token consume(TokenType expected) {
        if (peek().type != expected)
            throw runtime_error("Expected different token, got: " + peek().value);
        return tokens[index++];
    }

    string newTemp() { return "t" + to_string(tempId++); }
    string newLabel() { return "L" + to_string(labelId++); }

    // === Expressions ===

    string parseExpression() { return parseRelational(); }

    string parseRelational() {
        string left = parseAddSub();
        TokenType opType = peek().type;

        if (opType == TokenType::LT || opType == TokenType::GT ||
            opType == TokenType::EQ || opType == TokenType::NEQ) {
            Token op = consume(opType);
            string right = parseAddSub();
            string temp = newTemp();
            cout << temp << " = " << left << " " << op.value << " " << right << endl;
            return temp;
        }
        return left;
    }

    string parseAddSub() {
        string left = parseMulDiv();
        while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
            Token op = consume(peek().type);
            string right = parseMulDiv();
            string temp = newTemp();
            cout << temp << " = " << left << " " << op.value << " " << right << endl;
            left = temp;
        }
        return left;
    }

    string parseMulDiv() {
        string left = parsePrimary();
        while (peek().type == TokenType::MUL || peek().type == TokenType::DIV) {
            Token op = consume(peek().type);
            string right = parsePrimary();
            string temp = newTemp();
            cout << temp << " = " << left << " " << op.value << " " << right << endl;
            left = temp;
        }
        return left;
    }

    string parsePrimary() {
        Token t = peek();
        if (t.type == TokenType::NUMBER) {
            consume(TokenType::NUMBER);
            return t.value;
        } else if (t.type == TokenType::IDENTIFIER) {
            consume(TokenType::IDENTIFIER);
            return t.value;
        } else if (t.type == TokenType::L_PAREN) {
            consume(TokenType::L_PAREN);
            string val = parseExpression();
            consume(TokenType::R_PAREN);
            return val;
        }
        throw runtime_error("Invalid primary");
    }

    // === Statements ===

    void parseStatement() {
        Token t = peek();

        if (t.type == TokenType::INT) {
            consume(TokenType::INT);
            string var = consume(TokenType::IDENTIFIER).value;
            consume(TokenType::ASSIGN);
            string val = parseExpression();
            symbolTable[var] = val;
            cout << var << " = " << val << endl;
            consume(TokenType::SEMICOLON);
        } else if (t.type == TokenType::IDENTIFIER) {
            string var = consume(TokenType::IDENTIFIER).value;
            consume(TokenType::ASSIGN);
            string val = parseExpression();
            cout << var << " = " << val << endl;
            consume(TokenType::SEMICOLON);
        } else if (t.type == TokenType::RETURN) {
            consume(TokenType::RETURN);
            string val = parseExpression();
            cout << "RET " << val << endl;
            consume(TokenType::SEMICOLON);
        } else if (t.type == TokenType::IF) {
            parseIfStatement();
        } else if (t.type == TokenType::WHILE) {
            parseWhileStatement();
        } else if (t.type == TokenType::L_BRACE) {
            parseBlock();
        } else {
            throw runtime_error("Invalid statement: " + t.value);
        }
    }

    void parseIfStatement() {
        consume(TokenType::IF);
        consume(TokenType::L_PAREN);
        string cond = parseExpression();
        consume(TokenType::R_PAREN);

        string labelElse = newLabel();
        string labelEnd = newLabel();
        cout << "IFZ " << cond << " GOTO " << labelElse << endl;

        parseStatement(); // block or single stmt

        cout << "GOTO " << labelEnd << endl;
        cout << labelElse << ":" << endl;
        // (else support can go here later)
        cout << labelEnd << ":" << endl;
    }

    void parseWhileStatement() {
        consume(TokenType::WHILE);
        string labelStart = newLabel();
        string labelEnd = newLabel();
        cout << labelStart << ":" << endl;

        consume(TokenType::L_PAREN);
        string cond = parseExpression();
        consume(TokenType::R_PAREN);
        cout << "IFZ " << cond << " GOTO " << labelEnd << endl;

        parseStatement(); // block or stmt
        cout << "GOTO " << labelStart << endl;
        cout << labelEnd << ":" << endl;
    }

    void parseBlock() {
        consume(TokenType::L_BRACE);
        while (peek().type != TokenType::R_BRACE) {
            parseStatement();
        }
        consume(TokenType::R_BRACE);
    }

public:
    Parser(const vector<Token>& toks) : tokens(toks) {}

    void parseProgram() {
        consume(TokenType::INT);
        Token func = consume(TokenType::IDENTIFIER);
        if (func.value != "main") throw runtime_error("Only main supported");
        consume(TokenType::L_PAREN);
        consume(TokenType::R_PAREN);
        parseBlock();
        cout << "Compilation finished.\n";
    }
};

// === MAIN ===

int main() {
    cout << "Enter your C code (end with `}`):\n";
    string code, line;
    while (getline(cin, line)) {
        code += line + "\n";
        if (line.find("}") != string::npos) break;
    }

    try {
        Lexer lexer(code);
        vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        parser.parseProgram();
    } catch (const exception& e) {
        cerr << "Compiler error: " << e.what() << endl;
    }

    return 0;
}