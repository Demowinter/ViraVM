#include <core/types.hpp>
#include <core/execontext.hpp>
#include <core/runtime.hpp>
#include <core/isa.hpp>
#include <core/runner.hpp>

void executeInstruction(ExecutionContext& context, Instruction& instr) {
    switch (instr.opcode) {
        case OPCode::NOP: { nop(); break; }
        case OPCode::LOADC: { loadc(context, instr.operand1); break; }
        case OPCode::LOADL: { loadl(context, instr.operand1); break; }
        case OPCode::LOADG: { loadg(context, instr.operand1); break; }
        case OPCode::LOADA: { loada(context); break; }
        case OPCode::LOADM: { loadm(context, instr.operand1); break; }
        case OPCode::MKREF: { mkref(context); break; }
        case OPCode::INITREF: { initref(context); break; }
        case OPCode::STOREL: { storel(context, instr.operand1); break; }
        case OPCode::STOREG: { storeg(context, instr.operand1); break; }
        case OPCode::STOREA: { storea(context); break; }
        case OPCode::STOREM: { storem(context, instr.operand1); break; }
        case OPCode::JUMP: { jump(context, instr.operand1); break; }
        case OPCode::CALLF: { callf(context, instr.operand1, instr.operand2); break; }
        case OPCode::DEBUG_PRINT_STACK: { debug_print_stack(context); break; }

        // case OPCode::LOADG: {
        //     loadg(context, instr.operand1);

        //     break;
        // }
    }
}

void execute(ExecutionContext& context, InstructionList& instructions) {
    while (context.instructionPointer < instructions.size()) {
        executeInstruction(context, instructions[context.instructionPointer]);

        if (checkException(context)) catchException(context);

        updateInstructionPointer(context);
    }
}