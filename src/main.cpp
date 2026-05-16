#include <iostream>
#include <cmath>
#include <core/types.hpp>
#include <core/runner.hpp>
#include <core/runtime.hpp>
#include <bytecode/loader.hpp>
#include <test/testmaker.hpp>

int main(int argc, char** argv, char** env) {
    std::setlocale(LC_ALL, "");

    // auto [status, context, instructions] = loadBytecode("test.vic");

    // if (status == LoadStatus::OK) {
    //     runBytecode(context, instructions);

    //     return context.exitCode;
    // }

    // int8 intStatus = static_cast<int8>(status);

    // std::cout << intStatus << std::endl;

    // return intStatus;

    TestMaker maker;
    maker.addType("int32", ValueType::INT32);
    maker.addType("float64", ValueType::FLOAT64);
    maker.addType("int32&", ValueType::REFERECNE, 0);
    maker.addType("none", ValueType::NONE);

    maker.addConstant(10, 0);
    maker.addConstant(100, 0);
    maker.addConstant(M_PI, 1);

    maker.addFunction("test", 1, 10, 3);
    maker.addFunctionVariable("test", 2);
    maker.addFunctionVariable("answerToEveryting", 0);
    maker.addFunctionVariable("pi", 1);
    maker.addFunctionVariable("test2", 0);

    maker.addInstruction(OPCode::CALLF, 0, 0);

    // maker.addInstruction(OPCode::LOADC, 2);
    maker.addInstruction(OPCode::LOADC, 1);
    // maker.addInstruction(OPCode::LOADC, 0);

    // maker.addInstruction(OPCode::STOREG, 0);
    maker.addInstruction(OPCode::STOREL, 1);
    // maker.addInstruction(OPCode::STOREG, 2);

    maker.addInstruction(OPCode::LOADL, 0);
    maker.addInstruction(OPCode::LOADL, 1);
    // maker.addInstruction(OPCode::LOADG, 0);

    maker.addInstruction(OPCode::MKREF);
    maker.addInstruction(OPCode::INITREF);

    maker.addInstruction(OPCode::LOADL, 0);
    maker.addInstruction(OPCode::STOREL, 3);

    maker.addInstruction(OPCode::LOADL, 3);
    maker.addInstruction(OPCode::DEBUG_PRINT_STACK);
    // maker.addInstruction(OPCode::DEBUG_PRINT_STACK);
    // maker.addInstruction(OPCode::DEBUG_PRINT_STACK);
    
    // maker.addInstruction(OPCode::JUMP, 0);

    ExecutionContext context = maker.getContext();
    InstructionList ilist = maker.getInstructions();

    buildGlobals(context);

    execute(context, ilist);

    // Value testVal = getValueEntry(context, getGlobalVariableValue(context, 0)).value;
    // Value answer = getValueEntry(context, getGlobalVariableValue(context, 1)).value;
    // Value pi = getValueEntry(context, getGlobalVariableValue(context, 2)).value;

    // std::visit([](auto&& val) -> void {std::cout << "test: " << val << std::endl;}, testVal.value);
    // std::visit([](auto&& val) -> void {std::cout << "The answer to life the universe and everything: " << val << std::endl;}, answer.value);

    // std::cout.precision(15);

    // std::visit([](auto&& val) -> void {std::cout << "pi: " << val << std::endl;}, pi.value);

    return 0;
}