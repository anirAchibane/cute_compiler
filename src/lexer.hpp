#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
using namespace std;

enum TokenType{
    ENDOF,
    NEWLINE,
    NUMBER,
    IDENT,
    STRING,
    // keywords
    INPUT,
    LABEL,
    GOTO,
    PRINT,
    LET,
    IF,
    THEN,
    ENDIF,
    WHILE,
    REPEAT,
    ENDWHILE,
    // operators
    EQ,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    EQEQ,
    NOTEQ,
    LT,
    LTEQ,
    GT,
    GTEQ
};

class Token{
    private:
        TokenType type;
        string attribute;
        pair<size_t,size_t> pos;  // column, line
    
    public:
        Token(TokenType type, string attribute, pair<size_t,size_t> pos);
        // getters
        TokenType get_type();
        string& get_attribute();
        pair<size_t,size_t> get_pos();
};

class Lexer{
    private:
        string source;                  // source code
        char cur_char;                  // current character
        pair<size_t,size_t> cur_pos;    // column, line
        size_t index;                   // index in source code string

    public:
        Lexer(string source);

        // helper methods
        char next_char();
        char peek();
        [[noreturn]] void abort(const string& message = "Invalid token");
        void ignore_char();
        void ignore_comment();

        // tokenization methods
        Token get_token();            // returns next token
        vector<Token> tokenize();     // tokenizes source code
};
