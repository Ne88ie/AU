#ifndef Exception_H
#define Exception_H

#include <stdexcept>
#include <string>

using std::string;


class Exception : public std::exception {
    string message;
    size_t line_number;
public:
    Exception(size_t const line, string const &msg) : message(msg), line_number(line) {}
    ~Exception() throw () { }

    const char* what() const throw() {
        return message.c_str();
    }
    size_t get_line() const {
        return line_number;
    }
};

class EvaluatorException : public Exception {
public:
    explicit EvaluatorException(size_t const line_number, string const &message):
        Exception(line_number, message)  { }
};


class Syntax_error : public Exception {
public:
    explicit Syntax_error(size_t const line_number):
        Exception(line_number, "syntax error") { }
};


class Division_by_zero_error : public EvaluatorException {
public:
    explicit Division_by_zero_error(size_t const line_number):
        EvaluatorException(line_number, "division by zero") { }
};

class Undefined_variable_error : public EvaluatorException {
public:
    explicit Undefined_variable_error(size_t const line_number, string const &variable_name):
        EvaluatorException(line_number, "undefined variable " + variable_name) { }
};

class Undefined_function_error : public EvaluatorException {
public:
    explicit Undefined_function_error(size_t const line_number, string const &function_name):
        EvaluatorException(line_number, "undefined function " + function_name) { }
};

class Arguments_number_mismatch_error : public EvaluatorException {
public:
    explicit Arguments_number_mismatch_error(size_t const line_number, string const &function_name):
        EvaluatorException(line_number, "arguments number mismatch " + function_name) { }
};

#endif
