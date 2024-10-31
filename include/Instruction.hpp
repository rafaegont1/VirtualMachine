#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <string>
#include <vector>

struct Instruction {
    enum Opcode : uint8_t {
        NOP = 0x00,
        ADD,
        SUB,
        MULT,
        DIV,
        LI,
        JUMP,
        BEQ,
        BNE,
        BGEZ,
        BGTZ,
        BLEZ,
        BLTZ,
        LOAD,
        STORE
    };

    Opcode opcode;
    std::vector<uint32_t> args;
    std::string variable;

    static Opcode get_opcode(const std::string& opcode_name);
    static std::vector<std::vector<std::string>> get_instructions(const std::string& file_name);
};

#endif // INSTRUCTION_HPP
