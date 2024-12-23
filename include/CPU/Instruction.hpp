#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "CPU/RegFile.hpp"

namespace CPU {

struct Instruction {
    enum Opcode : uint8_t {
        NOP = 0x00, // no operation
        ADD,        // add
        SUB,        // subtract
        MUL,       // multiply
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
        STORE,      // store variable in memory
        HALT        // signal to end progrma
    };

    static const std::vector<std::string> NOP_INSTR;

    static Opcode get_opcode(const std::string & opcode_text);

    bool is_branch();

    const std::vector<std::string> * text;
    Opcode opcode;
    RegFile::RegNum rd, rs, rt;
    int32_t imm;
    uint32_t pid;
};

} // namespace CPU

#endif // INSTRUCTION_HPP
