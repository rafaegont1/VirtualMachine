#include <iostream>
#include <cctype>
// #include <fstream>
// #include <sstream>
#include <unordered_map>
#include "CPU/Instruction.hpp"

namespace CPU {

const std::vector<std::string> Instruction::NOP_INSTR = {"nop"};

Instruction::Opcode Instruction::get_opcode(const std::string & opcode_text)
{
    static const std::unordered_map<std::string, Instruction::Opcode>
    opcode_map = {
        {"nop",   Instruction::Opcode::NOP  },
        {"add",   Instruction::Opcode::ADD  },
        {"sub",   Instruction::Opcode::SUB  },
        {"mult",  Instruction::Opcode::MUL },
        {"div",   Instruction::Opcode::DIV  },
        {"li",    Instruction::Opcode::LI   },
        {"inc",   Instruction::Opcode::INC  },
        {"dec",   Instruction::Opcode::DEC  },
        {"jump",  Instruction::Opcode::JUMP },
        {"bgez",  Instruction::Opcode::BGEZ },
        {"bgtz",  Instruction::Opcode::BGTZ },
        {"blez",  Instruction::Opcode::BLEZ },
        {"bltz",  Instruction::Opcode::BLTZ },
        {"beq",   Instruction::Opcode::BEQ  },
        {"bne",   Instruction::Opcode::BNE  },
        {"load",  Instruction::Opcode::LOAD },
        {"store", Instruction::Opcode::STORE},
        {"halt",  Instruction::Opcode::HALT }
    };

    auto iter = opcode_map.find(opcode_text);
    if (iter == opcode_map.end()) {
        throw std::runtime_error("Error: opcode '" + opcode_text + "' not found");
    }

    return iter->second;
}

bool Instruction::is_branch()
{
    return (opcode == BEQ || opcode == BNE || opcode == BGEZ
        || opcode == BGTZ || opcode == BLEZ || opcode == BLTZ);
}

} // namespace CPU
