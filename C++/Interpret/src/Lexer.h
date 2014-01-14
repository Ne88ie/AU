#ifndef Lexer_H_
#define Lexer_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <typeinfo> 
#include <memory>
#include <cctype>


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
	size_t line_number;
	Lexeme current_lexeme;

	vector<Lexeme> result;

	void process_line(string const &line); 
	void get_operation(string::const_iterator &it, Lexeme &current_lexeme);
	void get_identifier(string::const_iterator &it, Lexeme &current_lexeme);
	void get_number(string::const_iterator &it, Lexeme &current_lexeme);

    Lexer(Lexer const&);
    Lexer& operator=(Lexer const&);    
};

#endif
