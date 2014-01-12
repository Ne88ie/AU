#include "Lexeme.h"

#include "EmptyState.h"
#include "AlphanumericState.h"
#include "OperationState.h"
#include "ErrorState.h"
#include "Exception.h"


void AlphanumericState::next_state(LexingAutomation &automation, SymbolTypes symbol_type) {
    switch (symbol_type) {
        case kUnderscore:
        case kLetter:
        case kDigit: break;
            
        case kOperationSymbol:
            automation.set_result(handle_alphanumeric(automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new OperationState()));
            break;
            
        case kSeparator:
            automation.set_result(handle_alphanumeric(automation.get_buffer_prefix(), automation.line()));
            automation.set_result(handle_separator(automation.buffer().back(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
        
        case kLinefeed:
            automation.set_result(handle_alphanumeric(automation.get_buffer_prefix(), automation.line()));
            automation.set_result(end_of_line(automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(handle_alphanumeric(automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
                        
        default:
            throw Syntax_error(automation.line());
            automation.set_state(state_ptr(new ErrorState()));
    }
}
