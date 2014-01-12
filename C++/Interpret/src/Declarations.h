#ifndef Declarations_H_
#define Declarations_H_

#include <vector>
#include <iostream>
#include <memory>
#include <cstddef>

class Instruction;
class Visitor;
class Program;
class LexerState;
class LexingAutomation;
class Error;

typedef std::shared_ptr<Instruction> instruction_ptr;
typedef std::shared_ptr<Program> program_ptr;
typedef std::shared_ptr<Visitor> visitor_ptr;
typedef std::vector<instruction_ptr> instructions;

#endif
