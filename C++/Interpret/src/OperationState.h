#ifndef OperationState_H_
#define OperationState_H_

#include "LexerState.h"


class OperationState: public LexerState {
public:
    void next_state(LexingAutomation& automation, SymbolTypes symbol_type);
};

#endif
