#ifndef ENCODING_HPP
#define ENCODING_HPP

#include <cstdint>
#include <functional>
#include <string>
#include "HW/ISA/Code.hpp"
#include "HW/CPU/RegFile.hpp"

namespace HW::ISA {

struct Encoding {
    enum Opcode : uint8_t {
        NOP = 0x00, // no operation
        ADD,        // add from registers
        SUB,        // subtract from registers
        MUL,        // multiply from registers
        DIV,        // divide from registers
        ADDI,       // add from immediate values
        SUBI,       // subtract from immediate values
        MULI,       // multiply from immediate values
        DIVI,       // divide from immediate values
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
        LW,         // load variable from memory
        SW,         // store variable in memory
        HALT        // signal to end program
    };

    static Opcode get_opcode(const std::string& mnemonic);

    bool is_jump_or_branch() const;

    std::reference_wrapper<const Code::Line> code_line = Code::NOP_LINE;
    Opcode opcode = Opcode::NOP;
    CPU::RegFile::Index rd = CPU::RegFile::Index::ZERO;
    CPU::RegFile::Index rs = CPU::RegFile::Index::ZERO;
    CPU::RegFile::Index rt = CPU::RegFile::Index::ZERO;
    int32_t imm1 = 0;
    int32_t imm2 = 0;
};

} // namespace HW::ISA

#endif // ENCODING_HPP
