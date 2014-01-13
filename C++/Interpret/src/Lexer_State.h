#ifndef Lexer_State_H_
#define Lexer_State_H_

#include "Lexeme.h"
#include "Lexing_Automation.h"


class Lexing_Automation;

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

class Lexer_State {
public:
    virtual void next_state(Lexing_Automation& automation, char symbol) = 0;
    virtual ~Lexer_State() { }
protected:
    Symbol_Types determine_symbol_type(char symbol);
};

class Empty_State: public Lexer_State {
public:
    void next_state(Lexing_Automation& automation, char symbol);
};

class Alphanumeric_State: public Lexer_State {
public:
    void next_state(Lexing_Automation& automation, char symbol);
};

class Numeric_State: public Lexer_State {
public:
    void next_state(Lexing_Automation& automation, char symbol);
};

class Operation_State: public Lexer_State {
public:
    void next_state(Lexing_Automation& automation, char symbol);
};

#endif
