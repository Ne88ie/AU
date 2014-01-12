#ifndef LexerState_H_
#define LexerState_H_

#include "LexingAutomation.h"
#include "Lexeme.h"

class LexingAutomation;


class LexerState {
public:
    virtual void next_state(LexingAutomation& automation, SymbolTypes lexeme_type) = 0;
    virtual ~LexerState() { }
};

#endif
