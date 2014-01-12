#include "ErrorHandler.h"

using std::cout;


bool ErrorHandler::m_ok = true;

void ErrorHandler::report_syntax_error(size_t const line_number) {
    m_ok = false;
    cout << "line " << line_number << ": syntax error.\n";
}

void ErrorHandler::report_division_by_zero(size_t const line_number) {
    m_ok = false;
    cout << "line " << line_number << ": division by zero.\n";
}

void ErrorHandler::report_undefined_variable(size_t const line_number, string variable_name) {
    m_ok = false;
    cout << "line " << line_number << ": undefined variable " << variable_name << ".\n";
}

void ErrorHandler::report_undefined_function(size_t const line_number, string function_name) {
    m_ok = false;
    cout << "line " << line_number << ": undefined function " << function_name << ".\n";
}

void ErrorHandler::report_arguments_number_mismatch(size_t const line_number, string function_name) {
    m_ok = false;
    cout << "line " << line_number << ": arguments number mismatch " << function_name << ".\n";
}
