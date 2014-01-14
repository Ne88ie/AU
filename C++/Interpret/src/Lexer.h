#ifndef Lexer_H_
#define Lexer_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <typeinfo> 
#include <memory>
#include <cctype>
#include <unordered_map>

using std::ifstream;
using std::string;
using std::vector;

#include "Lexeme.h"


enum Symbol_Types {
    kUnknownType = -1,
    kLetter,
    kUnderscore,
    kDigit,
    kOperationSymbol,
    kSeparator,
    kLinefeed,
    kWhitespace
};


class Lexer {
public:
    Lexer(string const &file_path);
    
    vector<Lexeme> const get_result();
    
private:
	size_t line;
	string buffer;
	enum States {empty_state, alphanumeric_state, numeric_state, operation_state} current_state;
	vector<Lexeme> result;

	string get_buffer_prefix();
	void set_buffer_to_last_char();
	Symbol_Types determine_symbol_type(char symbol);
	Lexeme alphanumeric_lexeme();
	Lexeme separator_lexeme();
	Lexeme operation_lexeme(string operation);
	void empty_state_to_next_state(Symbol_Types type);
	void alphanumeric_state_to_next_state(Symbol_Types type);
	void numeric_state_to_next_state(Symbol_Types type);
	void operation_state_to_next_state(Symbol_Types type);
	void next_state(Symbol_Types type);

    Lexer(Lexer const&);
    Lexer& operator=(Lexer const&);

    void tokenize(ifstream &input_stream);
    
};

#endif
