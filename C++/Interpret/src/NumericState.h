#ifndef NumericState_H_
#define NumericState_H_

#include "LexerState.h"


class NumericState: public LexerState {
public:
    void next_state(LexingAutomation& automation, SymbolTypes symbol_type);
};


#endif
