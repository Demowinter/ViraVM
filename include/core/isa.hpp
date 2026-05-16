#pragma once
#include <vector>
#include <core/types.hpp>
#include <core/execontext.hpp>

enum class OPCode : uint16 {
    NOP,
    LOADC,
    LOADL,
    LOADG,
    LOADA,
    LOADM,
    STOREL,
    STOREG,
    STOREA,
    STOREM,
    MKREF,
    INITREF,
    JUMP,
    JUMPIF,
    COMPARE,
    CALLF,
    CALLL,
    RET,
    RETV,
    DEBUG_PRINT_STACK
};

struct Instruction {
    OPCode opcode;

    int32 operand1;
    int32 operand2;
};

using InstructionList = std::vector<Instruction>;


// Just nop, do absolutely nothing
void nop();

// Load value from constant pool (index) to valueStack of top call-stack frame.
//
// Pushes lvalue index to stack.
void loadc(ExecutionContext& context, Index constIndex);

// Load value from local variable (index) in top call-stack frame to valueStack of top call-stack frame.
//
// Pushes lvalue index to stack.
void loadl(ExecutionContext& context, Index varIndex);

// Load value from global valriable (index) to valueStack of top call-stack frame.
//
// Pushes lvalue index to stack.
void loadg(ExecutionContext& context, Index varIndex);

// Load value from array by index to valueStack of top call-stack frame.
//
// Pushes lvalue index to stack.
void loada(ExecutionContext& context);

// Load value from structure member by index to valueStack of top call-stack frame.
//
// Pushes lvalue index to stack.
void loadm(ExecutionContext& context, Index memberIndex);

// Store value from top call-stack frame to local variable in top call-satck frame.
void storel(ExecutionContext& context, Index varIndex);

// Store value from top call-stack frame to global variable.
void storeg(ExecutionContext& context, Index varIndex);

// Store value from top call-stack frame to array element by index from stack.
void storea(ExecutionContext& context);

// Store value from top call-stack frame to struct member by index.
void storem(ExecutionContext& context, Index memberIndex);

// Create reference entry in referenceTable (index) of value from valueStack in top call-stack frame and load reference id to valueStack of top call-stack frame.
void mkref(ExecutionContext& context);

void initref(ExecutionContext& context);

// Jump to position in code
void jump(ExecutionContext& context, int32 codePosition);

// Jump to position in code if value on stack can be casted to bool and represents true
void jumpif(ExecutionContext& context, int32 codePosition);

// Compares two values on stack with compareOperator and pushes result to stack
void compare(ExecutionContext& context, Index compareOperator);

// Calls function
void callf(ExecutionContext& context, Index functionIndex, Size argCount);

// Calls lambda function
void calll(ExecutionContext& context, Size argCount);

// Returns from the function
void ret(ExecutionContext& context);

// Returns from the function
void retv(ExecutionContext& context);

void debug_print_stack(ExecutionContext& context);