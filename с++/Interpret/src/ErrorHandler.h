#ifndef ErrorHandler_H_
#define ErrorHandler_H_

#include <string>
#include <iostream>

using std::string;

class ErrorHandler {
public:
    static void report_syntax_error(size_t const line_number);
    static void report_division_by_zero(size_t const line_number);
    static void report_undefined_variable(size_t const line_number, string variable_name);
    static void report_undefined_function(size_t const line_number, string function_name);
    static void report_arguments_number_mismatch(size_t const line_number, string function_name);
    static bool is_ok() {
        return m_ok;
    }
    
private:
    static bool m_ok;
};

#endif