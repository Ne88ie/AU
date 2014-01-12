#include "Lexeme.h"

#include "EmptyState.h"
#include "AlphanumericState.h"
#include "NumericState.h"
#include "OperationState.h"
#include "ErrorState.h"
#include "Exception.h"


void EmptyState::next_state(LexingAutomation& automation, SymbolTypes symbol_type) {
    switch (symbol_type) {
        case kLetter:
            automation.set_state(state_ptr(new AlphanumericState()));
            break;
            
        case kDigit:
            automation.set_state(state_ptr(new NumericState()));
            break;
            
        case kOperationSymbol:
            automation.set_state(state_ptr(new OperationState()));
            break;
            
        case kSeparator:
            automation.set_result(handle_separator(automation.buffer().back(), automation.line()));
            automation.clear_buffer();
            break;
            
        case kLinefeed:
            automation.set_result(end_of_line(automation.line()));
            automation.clear_buffer();
            break;
            
        case kWhitespace:
            if (automation.buffer().length() != 1)
                automation.set_state(state_ptr(new ErrorState()));
            else
                automation.clear_buffer();
            break;
     
        default:
            throw Syntax_error(automation.line());
            automation.set_state(state_ptr(new ErrorState()));
    }    
}
