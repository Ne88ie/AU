#ifndef Lexer_H_
#define Lexer_H_

#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo> 
#include <memory>

using std::ifstream;
using std::string;
using std::cout;

#include "Lexeme.h"
#include "Lexing_Automation.h"

typedef std::shared_ptr<Lexing_Automation> automation_ptr;


class Lexer {
public:
    Lexer(string const &file_path);

    vector<Lexeme> get_result();
    
private:
    Lexer(Lexer const&);
    Lexer& operator=(Lexer const&);

    void tokenize(ifstream &m_stream);

    automation_ptr m_automation;
};

#endif
