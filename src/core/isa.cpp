#include <iostream>
#include <core/types.hpp>
#include <core/execontext.hpp>
#include <core/runtime.hpp>
#include <core/isa.hpp>
#include <cxxabi.h>

void nop() {}

void loadc(ExecutionContext& context, Index constIndex) {
    Index valIndex = getConstantValue(context, constIndex);

    pushValue(context, valIndex);
}

void loadl(ExecutionContext& context, Index varIndex) {
    Index valIndex = getLocalVariableValue(context, varIndex);

    pushValue(context, valIndex);
}

void loadg(ExecutionContext& context, Index varIndex) {
    Index valIndex = getGlobalVariableValue(context, varIndex);

    pushValue(context, valIndex);
}

void loada(ExecutionContext& context) {
    Index elemIndex = popValue(context);
    Index arrayIndex = popValue(context);

    Index valIndex = getArrayElementValue(context, arrayIndex, elemIndex);

    pushValue(context, valIndex);

    checkClearTemporary(context, elemIndex);
    checkClearTemporary(context, arrayIndex);
}

void loadm(ExecutionContext& context, Index memberIndex) {
    Index structIndex = popValue(context);

    Index valIndex = getStructMemberValue(context, structIndex, memberIndex);

    pushValue(context, valIndex);

    checkClearTemporary(context, structIndex);
}

void storel(ExecutionContext& context, Index varIndex) {
    Index source = popValue(context);
    Index destination = getLocalVariableValue(context, varIndex);

    storeValue(context, destination, source);

    checkClearTemporary(context, source);
}

void storeg(ExecutionContext& context, Index varIndex) {
    Index source = popValue(context);
    Index destination = getGlobalVariableValue(context, varIndex);

    storeValue(context, destination, source);

    checkClearTemporary(context, source);
}

void storea(ExecutionContext& context) {
    Index source = popValue(context);
    Index elemIndex = popValue(context);
    Index arrayIndex = popValue(context);

    Index destination = getArrayElementValue(context, arrayIndex, elemIndex);

    storeValue(context, destination, source);

    checkClearTemporary(context, source);
    checkClearTemporary(context, elemIndex);
    checkClearTemporary(context, arrayIndex);
}

void storem(ExecutionContext& context, Index memberIndex) {
    Index source = popValue(context);
    Index structIndex = popValue(context);

    Index destination = getStructMemberValue(context, structIndex, memberIndex);

    storeValue(context, destination, source);

    checkClearTemporary(context, source);
    checkClearTemporary(context, structIndex);
}

void mkref(ExecutionContext& context) {
    Index valueIndex = popValue(context);
    Index valueIndexNew = makeReference(context, valueIndex);
    
    pushValue(context, valueIndexNew);
}

void initref(ExecutionContext& context) {
    Index refValIndex = popValue(context);
    Index initValIndex = popValue(context); // cannot be unbound, so checkClearTemporary not used

    initReference(context, initValIndex, refValIndex);

    checkClearTemporary(context, refValIndex);
}

void jump(ExecutionContext& context, Index codePosition) {
    setInstructionPointer(context, codePosition);
}

void jumpif(ExecutionContext& context, Index codePosition) {
    Index valueIndex = popValue(context);

    if (checkCondition(context, valueIndex)) setInstructionPointer(context, codePosition);

    checkClearTemporary(context, valueIndex);
}

void compare(ExecutionContext& context, Index compareOperator) {
    Index operand1 = popValue(context);
    Index operand2 = popValue(context);

    Index resultIndex = compareValues(context, operand1, operand2, compareOperator);

    pushValue(context, resultIndex);

    checkClearTemporary(context, operand1);
    checkClearTemporary(context, operand2);
}

void callf(ExecutionContext& context, Index functionIndex, Size argCount) {
    callFunction(context, functionIndex, argCount);
}

void calll(ExecutionContext& context, Size argCount) {
    Index valueIndex = popValue(context);

    callLambdaFunction(context, valueIndex, argCount);

    checkClearTemporary(context, valueIndex);
}

void ret(ExecutionContext& context) {
    returnFunction(context);
}

void retv(ExecutionContext& context) {
    Index valueIndex = popValue(context);

    returnFunction(context);

    pushValue(context, valueIndex);
}

void debug_print_stack(ExecutionContext& context) {
    Index index = popValue(context);

    ValueEntry& valEntry = getValueEntry(context, index);

    std::visit([](auto&& value) -> void {
        std::cout << value << "\t(" << abi::__cxa_demangle(typeid(value).name(), nullptr, nullptr, nullptr) << ")" << std::endl;
    }, valEntry.value.value);
}