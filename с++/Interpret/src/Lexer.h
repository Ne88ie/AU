#ifndef Lexer_H_
#define Lexer_H_

#include <iostream>
#include <typeinfo> 

#include "Lexeme.h"
#include "LexingAutomation.h"

typedef std::shared_ptr<std::istream> istream_ptr;
typedef std::shared_ptr<LexingAutomation> automation_ptr;


class Lexer {
public:
    Lexer(istream_ptr input_stream);

    void tokenize();
    vector<Lexeme> get_result();
    
private:
    Lexer(Lexer const&);
    Lexer& operator=(Lexer const&);

    istream_ptr m_stream;
    automation_ptr m_automation;
};

#endif