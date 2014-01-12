#include "Lexeme.h"
#include "EmptyState.h"
#include "AlphanumericState.h"
#include "NumericState.h"
#include "OperationState.h"
#include "ErrorState.h"
#include "ErrorHandler.h"


void OperationState::next_state(LexingAutomation& automation, SymbolTypes symbol_type) {
    switch (symbol_type) {
        case kLetter:
            automation.set_result(handle_operation(automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new AlphanumericState()));
            break;

        case kDigit:
            automation.set_result(handle_operation(automation.get_buffer_prefix(), automation.line()));
            automation.set_buffer_to_last_char();
            automation.set_state(state_ptr(new NumericState()));
            break;
            
        case kOperationSymbol:
            if (mapTypesOp.count(automation.buffer()) == 0) {
                automation.set_result(handle_operation(automation.get_buffer_prefix(), automation.line()));
                string lexeme_symbol(1, automation.buffer().back());
                automation.set_result(handle_operation(lexeme_symbol, automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new EmptyState()));
            } else {
                automation.set_result(handle_operation(automation.buffer(), automation.line()));
                automation.clear_buffer();
                automation.set_state(state_ptr(new EmptyState()));
            }
            break;
            
        case kSeparator:
            automation.set_result(handle_operation(automation.get_buffer_prefix(), automation.line()));
            automation.set_result(handle_separator(automation.buffer().back(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        case kWhitespace:
            automation.set_result(handle_operation(automation.get_buffer_prefix(), automation.line()));
            automation.clear_buffer();
            automation.set_state(state_ptr(new EmptyState()));
            break;
            
        default:
            ErrorHandler::report_syntax_error(automation.line());
            automation.set_state(state_ptr(new ErrorState()));
    }
}
