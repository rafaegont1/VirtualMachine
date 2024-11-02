#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace Instruction {
    enum Opcode : uint8_t {
        NOP = 0x00, // no operation
        ADD,        // add
        SUB,        // subtract
        MULT,       // multiply
        DIV,        // divide
        LI,         // load immeadiate
        INC,        // increment
        DEC,        // decrement
        JUMP,       // jump
        BEQ,        // branch on equal
        BNE,        // branch on not equal
        BGEZ,       // branch on greatear or equal to zero
        BGTZ,       // branch on greatear than zero
        BLEZ,       // branch on lesser or equal to zero
        BLTZ,       // branch on lesser than zero
        LOAD,       // load variable from memory
        STORE       // store variable in memory
    };

    Opcode get_opcode(const std::string& opcode_name);
    std::vector<std::vector<std::string>> load_program(const std::string& file_name);
    bool is_branch(Instruction::Opcode opcode);
}

#endif // INSTRUCTION_HPP
