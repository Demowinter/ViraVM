#include <algorithm>
#include <test/testmaker.hpp>

Index addName(ExecutionContext& context, std::string name) {
    context.namePool.push_back({name});

    return context.namePool.size() - 1;
}

void TestMaker::addInstruction(OPCode opcode, Index op1, Index op2, uint16 stusage) {
    ilist.push_back({opcode, op1, op2});
}

void TestMaker::addType(std::string name, ValueType type, Index subtype, Index etype) {
    Index nameIndex = addName(context, name);

    MetaTypeEntry typeEntry;
    typeEntry.typeName = nameIndex;
    typeEntry.typeBinding = type;
    typeEntry.subtypeBinding = subtype;
    typeEntry.externalTypeBinding = etype;
    
    context.typePool.push_back(typeEntry);
}

void TestMaker::addConstant(ValueContainer value, Index type) {
    MetaConstantEntry constEntry;
    constEntry.value.type = type;
    constEntry.value.value = value;

    context.constPool.push_back(constEntry);
}

void TestMaker::addVariable(std::string name, Index type) {
    Index nameIndex = addName(context, name);

    MetaVariableEntry varEntry;
    varEntry.variableName = nameIndex;
    varEntry.variableType = type;

    context.globalVarPool.push_back(varEntry);
}

void TestMaker::addFunction(std::string name, Index codeStart, Index codeEnd, Index returnType) {
    Index nameIndex = addName(context, name);

    MetaFunctionEntry funcEntry;
    funcEntry.functionName = nameIndex;
    funcEntry.codeStart = codeStart;
    funcEntry.codeEnd = codeEnd;
    funcEntry.returnType = returnType;

    context.functionPool.push_back(funcEntry);

    currentFunction = context.functionPool.size() - 1;
}

void TestMaker::addFunctionTryBlock(Index codeStart, Index codeEnd) {
    MetaFunctionEntry& funcEntry = context.functionPool[currentFunction];

    MetaTryBlockEntry tryEntry;
    tryEntry.codeStart = codeStart;
    tryEntry.codeEnd = codeEnd;

    funcEntry.tryBlockPool.push_back(tryEntry);

    currentTryBlock = funcEntry.tryBlockPool.size() - 1;
}

void TestMaker::addFunctionCatchBlock(Index codeStart, Index codeEnd, Index type) {
    MetaFunctionEntry& funcEntry = context.functionPool[currentFunction];
    MetaTryBlockEntry& tryEntry = funcEntry.tryBlockPool[currentTryBlock];

    MetaCatchBlockEntry catchEntry;
    catchEntry.codeStart = codeStart;
    catchEntry.codeEnd = codeEnd;
    catchEntry.catchArg = type;

    tryEntry.catchBlockPool.push_back(catchEntry);
    
}

void TestMaker::addFunctionVariable(std::string name, Index type) {
    Index nameIndex = addName(context, name);

    MetaFunctionEntry& funcEntry = context.functionPool[currentFunction];

    MetaVariableEntry varEntry;
    varEntry.variableName = nameIndex;
    varEntry.variableType = type;

    funcEntry.localVarPool.push_back(varEntry);
}

void TestMaker::addStructure() {
    MetaStructEntry structEntry;

    context.structPool.push_back(structEntry);

    currentStructure = context.structPool.size() - 1;
}

void TestMaker::addStructureMember(std::string name, Index type) {
    Index nameIndex = addName(context, name);

    MetaStructEntry& structEntry = context.structPool[currentStructure];

    MetaStructMemberEntry memberEntry;
    memberEntry.memberName = nameIndex;
    memberEntry.memberType = type;

    structEntry.memberPool.push_back(memberEntry);
}

void TestMaker::init() {
    addFunction("viraEntryPoint", 0, ilist.size(), 0);

    // TODO: remove once CALL instruction exists
    context.callStack.push_back({});
}

ExecutionContext TestMaker::getContext() {
    return context;
}

InstructionList TestMaker::getInstructions() {
    return ilist;
}
