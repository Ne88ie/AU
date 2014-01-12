#ifndef AlphanumericState_H_
#define AlphanumericState_H_

#include "LexerState.h"


class AlphanumericState: public LexerState {
public:
    void next_state(LexingAutomation& automation, SymbolTypes symbol_type);
};

#endif