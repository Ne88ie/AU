#include <iostream>

#include "Evaluator.h"
#include "Exception.h"

using std::cin;
using std::cout;
using std::endl;


Evaluator::Evaluator(program_ptr program): m_program(program) { }

void Evaluator::execute_program() {
    visit(*m_program);
}

int Evaluator::visit(Constant const& instruction) {
    return instruction.value();
}

int Evaluator::visit(Variable const& instruction) {
    variable_scope::iterator variable = m_variable_scopes.top().find(instruction.name());
    if (variable == m_variable_scopes.top().end()) {
        throw Undefined_variable_error(instruction.line(), instruction.name());
        return 0;
    }
    return variable->second;
}

int Evaluator::visit(Program const& instruction) {
    variable_scope program_scope;
    m_variable_scopes.push(program_scope);

    for (instructions::const_iterator it = instruction.functions().begin(); it != instruction.functions().end(); ++it)
        (*it)->accept_visit(this);

    instruction.InstructionBlock::accept_visit(this);
    return m_result;
}

int Evaluator::visit(Function const& instruction) {
    m_function_scope[instruction.name()] = &instruction;
    return 0;
}

int Evaluator::visit(Assignment const& instruction) {
    int value = instruction.expression()->accept_visit(this);
    m_variable_scopes.top()[instruction.name()] = value;
    return m_result;
}

int Evaluator::visit(FunctionCall const& instruction) {
    function_scope::const_iterator func = m_function_scope.find(instruction.name());
    if (func == m_function_scope.end()) {
        throw Undefined_function_error(instruction.line(), instruction.name());
        return 0;
    }
    
    Function const* function_to_call = func->second;
    
    if (function_to_call->arguments().size() != instruction.arguments().size()) {
        throw Arguments_number_mismatch_error(instruction.line(), instruction.name());
        return 0;
    }
    
    variable_scope function_to_call_scope;
    
    for (size_t i = 0; i != instruction.arguments().size(); ++i)
        function_to_call_scope[function_to_call->arguments()[i]] = instruction.arguments()[i]->accept_visit(this);
    
    m_variable_scopes.push(function_to_call_scope);
    function_to_call->InstructionBlock::accept_visit(this);
    m_variable_scopes.pop();
    int result = m_result;
    m_result = 0;
    m_has_returned = false;
    return result;
}

int Evaluator::visit(ArithmeticOperation const& instruction) {
    int left = instruction.left_operand()->accept_visit(this);
    int right = instruction.right_operand()->accept_visit(this);
    
    switch (instruction.operation()) {
        case ArithmeticOperation::kAdd:
            return left + right;   
        case ArithmeticOperation::kSub:
            return left - right;
        case ArithmeticOperation::kMul:
            return left * right;
        case ArithmeticOperation::kDiv:
            if (right == 0) {
                throw Division_by_zero_error(instruction.line());
                return 0;
            }
            return left / right;
        default:
            return 0;
    }
}

int Evaluator::visit(Read const& instruction) {
    cin >> m_variable_scopes.top()[instruction.variable()];
    return m_result;
}

int Evaluator::visit(Print const& instruction) {
    int result = instruction.expression()->accept_visit(this);
    cout << result << endl;
    return m_result;
}

int Evaluator::visit(Conditional const& instruction) {
    int left = instruction.left_operand()->accept_visit(this);
    int right = instruction.right_operand()->accept_visit(this);
    
    switch (instruction.operation()) {
        case Conditional::kEqual:
            return left == right;
        case Conditional::kNotEqual:
            return left != right;
        case Conditional::kLess:
            return left < right;
        case Conditional::kGreater:
            return left > right;
        case Conditional::kLessEqual:
            return left <= right;
        case Conditional::kGreaterEqual:
            return left >= right;
        default:
            return 0;
    }
}

int Evaluator::visit(IfBlock const& instruction) {
    if (instruction.condition()->accept_visit(this))
        instruction.InstructionBlock::accept_visit(this);

    return m_result;
}

int Evaluator::visit(WhileBlock const& instruction) {
    while (instruction.condition()->accept_visit(this))
        instruction.InstructionBlock::accept_visit(this);

    return m_result;
}

int Evaluator::visit(Return const& instruction) {
    m_result = instruction.expression()->accept_visit(this);
    m_has_returned = true;
    return m_result;
}

int Evaluator::visit(InstructionBlock const& instruction) {
    for (instructions::const_iterator it = instruction.block().begin(); it != instruction.block().end() && !m_has_returned; ++it)
        (*it)->accept_visit(this);

    return m_result;
}
