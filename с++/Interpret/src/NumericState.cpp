#include "NumericState.h"
#include "Lexeme.h"
#include "EmptyState.h"
#include "NumericState.h"
#include "OperationState.h"
#include "ErrorState.h"
#include "ErrorHandler.h"


void NumericState::next_state(LexingAutomation &automation, SymbolTypes symbol_type) {
    switch (symbol_type) {
        case kDigit:
            break;
            
        case kOperationSymbol:
            automation.set_result(handle_number(automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new OperationState()));
            break;
            
        case kSeparator:
            automation.set_result(handle_number(automation.get_buffer_prefix(), automation.line()));
            automation.set_result(handle_separator(automation.buffer().back(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
        
        case kLinefeed:
            automation.set_result(handle_number(automation.get_buffer_prefix(), automation.line()));
            automation.set_result(end_of_line(automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(handle_number(automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        default:
            ErrorHandler::report_syntax_error(automation.line());
            automation.set_state(state_ptr(new ErrorState()));
    }
}
