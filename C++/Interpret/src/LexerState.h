#ifndef LexerState_H_
#define LexerState_H_

#include "Lexeme.h"
#include "LexingAutomation.h"


class LexingAutomation;

enum SymbolTypes {
    kUnknownType = -1,
    kLetter,
    kUnderscore,
    kDigit,
    kOperationSymbol,
    kSeparator,
    kLinefeed,
    kWhitespace
};

class LexerState {
public:
    virtual void next_state(LexingAutomation& automation, char symbol) = 0;
    virtual ~LexerState() { }
protected:
    SymbolTypes determine_symbol_type(char symbol);
};

class EmptyState: public LexerState {
public:
    void next_state(LexingAutomation& automation, char symbol);
};

class AlphanumericState: public LexerState {
public:
    void next_state(LexingAutomation& automation, char symbol);
};

class NumericState: public LexerState {
public:
    void next_state(LexingAutomation& automation, char symbol);
};

class OperationState: public LexerState {
public:
    void next_state(LexingAutomation& automation, char symbol);
};

// class ErrorState: public LexerState {
// public:
//     void next_state(LexingAutomation&, char) { }
// };

#endif
