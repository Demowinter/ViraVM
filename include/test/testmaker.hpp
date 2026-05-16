#pragma once
#include <tuple>
#include <string>
#include <core/types.hpp>
#include <core/isa.hpp>
#include <core/config.hpp>

class TestMaker {
public:
    void addInstruction(OPCode opcode, Index op1 = invalidIndex, Index op2 = invalidIndex, uint16 stusage = 0);

    void addType(std::string name, ValueType type, Index subtype = invalidIndex, Index etype = invalidIndex);

    void addConstant(ValueContainer value, Index type);

    void addVariable(std::string name, Index type);

    void addFunction(std::string name, Index codeStart, Index codeEnd, Index returnType);
    void addFunctionTryBlock(Index codeStart, Index codeEnd);
    void addFunctionCatchBlock(Index codeStart, Index codeEnd, Index type);
    void addFunctionVariable(std::string name, Index type);

    void addStructure();
    void addStructureMember(std::string name, Index type);

    void init();

    ExecutionContext getContext();
    InstructionList getInstructions();
private:
    ExecutionContext context{};
    InstructionList ilist;

    Index currentFunction;
    Index currentTryBlock;
    Index currentStructure;
};