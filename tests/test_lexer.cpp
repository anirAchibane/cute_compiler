#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "lexer.hpp"
using namespace std;

const unordered_map<TokenType, string> token_names = {
    {ENDOF, "ENDOF"},
    {NEWLINE, "NEWLINE"},
    {NUMBER, "NUMBER"},
    {IDENT, "IDENT"},
    {STRING, "STRING"},
    {INPUT, "INPUT"},
    {LABEL, "LABEL"},
    {GOTO, "GOTO"},
    {PRINT, "PRINT"},
    {LET, "LET"},
    {IF, "IF"},
    {THEN, "THEN"},
    {ENDIF, "ENDIF"},
    {WHILE, "WHILE"},
    {REPEAT, "REPEAT"},
    {ENDWHILE, "ENDWHILE"},
    {EQ, "EQ"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {ASTERISK, "ASTERISK"},
    {SLASH, "SLASH"},
    {EQEQ, "EQEQ"},
    {NOTEQ, "NOTEQ"},
    {LT, "LT"},
    {LTEQ, "LTEQ"},
    {GT, "GT"},
    {GTEQ, "GTEQ"}
};

std::string read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return "";

    streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    string content(size, '\0');
    file.read(&content[0], size);
    return content;
}

void print_result(Lexer lexer){
    vector<Token> token_list = lexer.tokenize();

    for (Token token: token_list){
        if (token.get_attribute() != ""){
            cout << token_names.at(token.get_type()) << "(" << token.get_attribute() << ")" << endl;
        }
        else{
            cout << token_names.at(token.get_type()) << endl;
        }
    }
}

int main(){
    // source code to test
    string source = read_file("../examples/first.cute");
    // lexer construction
    Lexer lexer = Lexer(source);
    print_result(lexer);
}
