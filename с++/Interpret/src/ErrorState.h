#ifndef ErrorState_H_
#define ErrorState_H_

#include "LexerState.h"

class ErrorState: public LexerState {
public:
    void next_state(LexingAutomation& automation, SymbolTypes symbol_type) { }
};

#endif