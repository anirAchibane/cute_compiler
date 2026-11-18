#include <string>
#include <iostream>
#include <utility>
#include "lexer.hpp"
#include <cctype>
#include <cstdlib>
#include <vector>
#include <cstdint>
using namespace std;


Token::Token(TokenType type, string attribute, pair<size_t,size_t> pos): type(type), attribute(attribute), pos(pos){};

TokenType Token::get_type(){
    return type;
};

string& Token::get_attribute(){
    return attribute;
};

pair<size_t,size_t> Token::get_pos(){
    return pos;
};


Lexer::Lexer(string source): source(source), cur_char(source.empty() ? '\0' : source[0]), cur_pos({0,0}), index(0){
    if (!source.empty() && cur_char == '\0'){
        abort("Unexpected null byte in source");
    }
};

char Lexer::next_char(){
    if (this->index >= source.size()){
        this->cur_char = '\0';
        return this->cur_char;
    }

    this->index++;
    // update pos   
    if (this->cur_char == '\n'){
        this->cur_pos.first = 0;
        this->cur_pos.second++;
    }
    else this->cur_pos.first++;

    // update char
    if (this->index >= source.size()){
        this->cur_char = '\0';
    }
    else {
        this->cur_char = this->source[index];
        if (this->cur_char == '\0'){
            abort("Unexpected null byte in source");
        }
    }
    
    return this->cur_char;
};

char Lexer::peek(){
    if (this->index + 1 >= source.size()){
        return '\0';
    }
    return source[this->index + 1];
};

void Lexer::abort(const string& message){
    cerr << "Lexical error at source index " << index << ": " << message << "\n" <<
        "line: " << cur_pos.second << ", col: " << cur_pos.first << endl;
    std::exit(EXIT_FAILURE);
};

void Lexer::ignore_char(){
    while(cur_char == ' ' || cur_char == '\t' || cur_char == '\r'){
        next_char();
    }
};

void Lexer::ignore_comment(){
    if (this->cur_char == '#'){
        while(this->cur_char != '\n' && this->cur_char != '\0') next_char();
    }
}

Token Lexer::get_token(){
    TokenType type;
    string attribute = "";
    pair<size_t, size_t> pos;

    ignore_char();
    ignore_comment();
    pos = cur_pos;
    if (cur_char == '-'){
        type = MINUS;
        next_char();
    }
    else if (cur_char == '+'){
        type = PLUS;
        next_char();
    }
    else if (cur_char == '*'){
        type = ASTERISK;
        next_char();
    }
    else if (cur_char == '/'){
        type = SLASH;
        next_char();
    }
    else if (cur_char == '\n'){
        type = NEWLINE;
        next_char();
    }
    else if (index >= source.size()){
        type = ENDOF;
    }
    else if (cur_char == '='){
        if (peek() == '='){
            next_char();
            next_char();
            type = EQEQ;
        }
        else {
            type = EQ;
            next_char();
        }
    }
    else if (cur_char == '!'){
        if (peek() == '='){
            next_char();
            next_char();
            type = NOTEQ;
        }
        else abort("Expected '=' after '!'");
    }
    else if (cur_char == '>'){
        if (peek() == '='){
            next_char();
            next_char();
            type = GTEQ;
        }
        else { 
            type = GT;
            next_char();
        }
    }
    else if (cur_char == '<'){
        if (peek() == '='){
            next_char();
            next_char();
            type = LTEQ;
        }
        else { 
            type = LT;
            next_char();
        }
    }
    else if (cur_char == '\"'){
        next_char();
        size_t start_ind = this->index;
        
        while(this->cur_char != '\"'){
            if (this->cur_char == '\0'){
                abort("Unterminated string literal");
            }
            if (this->cur_char == '\r' || this->cur_char == '\n'){
                abort("Newline in string literal");
            }
            if (this->cur_char == '\\'){
                abort("Backslash is not allowed in string literals");
            }
            next_char();
        }
        attribute = source.substr(start_ind, index - start_ind);
        next_char();
        type = STRING;
        
    }
    else if(isdigit(static_cast<unsigned char>(this->cur_char))){
        size_t start_ind = this->index;
        // The parser must allow this magnitude only for the minimum signed value.
        constexpr std::uint32_t max_magnitude = std::uint32_t{1} << 31;
        std::uint32_t magnitude = 0;

        while(isdigit(static_cast<unsigned char>(this->cur_char))){
            const auto digit = static_cast<std::uint32_t>(this->cur_char - '0');
            if (magnitude > (max_magnitude - digit) / 10){
                abort("Integer literal magnitude exceeds 2147483648");
            }
            magnitude = magnitude * 10 + digit;
            this->next_char();
        }
        if (this->cur_char == '.'){
            abort("Fractional literals are not supported");
        }
        attribute = source.substr(start_ind, index - start_ind);
        type = NUMBER;
    }
    else if (source.compare(index, 5, "INPUT") == 0 &&
             (index + 5 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 5])) && source[index + 5] != '_'))){
        for (int i = 0; i < 5; i++){
            next_char();
        }
        type = INPUT;
    }
    else if (cur_char == 'I' && peek() == 'F' &&
             (index + 2 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 2])) && source[index + 2] != '_'))){
        next_char();
        next_char();

        type = IF;
    }
    else if (cur_char == 'L' && peek() == 'E' &&
             index + 2 < source.size() && source[index + 2] == 'T' &&
             (index + 3 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 3])) && source[index + 3] != '_'))){
        next_char();
        next_char();
        next_char();

        type = LET;
    }
    else if (source.compare(index, 5, "LABEL") == 0 &&
             (index + 5 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 5])) && source[index + 5] != '_'))){
        for (int i = 0; i < 5; i++){
            next_char();
        }
        type = LABEL;
    }
    else if (source.compare(index, 4, "GOTO") == 0 &&
             (index + 4 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 4])) && source[index + 4] != '_'))){
        for (int i = 0; i < 4; i++){
            next_char();
        }
        type = GOTO;
    }
    else if (source.compare(index, 5, "PRINT") == 0 &&
             (index + 5 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 5])) && source[index + 5] != '_'))){
        for (int i = 0; i < 5; i++){
            next_char();
        }
        type = PRINT;
    }
    else if (source.compare(index, 4, "THEN") == 0 &&
             (index + 4 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 4])) && source[index + 4] != '_'))){
        for (int i = 0; i < 4; i++){
            next_char();
        }
        type = THEN;
    }
    else if (source.compare(index, 5, "ENDIF") == 0 &&
             (index + 5 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 5])) && source[index + 5] != '_'))){
        for (int i = 0; i < 5; i++){
            next_char();
        }
        type = ENDIF;
    }
    else if (source.compare(index, 5, "WHILE") == 0 &&
             (index + 5 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 5])) && source[index + 5] != '_'))){
        for (int i = 0; i < 5; i++){
            next_char();
        }
        type = WHILE;
    }
    else if (source.compare(index, 6, "REPEAT") == 0 &&
             (index + 6 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 6])) && source[index + 6] != '_'))){
        for (int i = 0; i < 6; i++){
            next_char();
        }
        type = REPEAT;
    }
    else if (source.compare(index, 8, "ENDWHILE") == 0 &&
             (index + 8 >= source.size() ||
              (!isalnum(static_cast<unsigned char>(source[index + 8])) && source[index + 8] != '_'))){
        for (int i = 0; i < 8; i++){
            next_char();
        }
        type = ENDWHILE;
    }

    else if(isalpha(static_cast<unsigned char>(this->cur_char))){
        size_t start_ind = this->index;
        next_char();

        while(isalnum(static_cast<unsigned char>(this->cur_char)) || this->cur_char == '_'){
            this->next_char();
        }
        attribute = source.substr(start_ind, index - start_ind);
        type = IDENT;
    }
    else abort("Unexpected character (byte " + to_string(static_cast<unsigned char>(cur_char)) + ")");

    return Token(type,attribute,pos);
};

vector<Token> Lexer::tokenize(){
    vector<Token> source_tokenized = {};

    do {
        source_tokenized.push_back(get_token());
    } while (source_tokenized.back().get_type() != ENDOF);

    return source_tokenized;
}
