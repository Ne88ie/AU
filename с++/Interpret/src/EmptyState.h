#ifndef EmptyState_H_
#define EmptyState_H_

#include "LexerState.h"

class LexingAutomation;

class EmptyState: public LexerState {
public:
    void next_state(LexingAutomation& automation, SymbolTypes symbol_type);
};

#endif