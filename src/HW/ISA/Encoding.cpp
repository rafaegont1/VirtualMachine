#include <cctype>
#include <unordered_map>
#include "HW/ISA/Encoding.hpp"

namespace HW::ISA {

Encoding::Opcode Encoding::get_opcode(const std::string& mnemonic)
{
    static const std::unordered_map<std::string, Opcode> opcode_map = {
        { "nop",  Opcode::NOP  },
        { "add",  Opcode::ADD  },
        { "sub",  Opcode::SUB  },
        { "mul",  Opcode::MUL  },
        { "div",  Opcode::DIV  },
        { "addi", Opcode::ADDI },
        { "subi", Opcode::SUBI },
        { "muli", Opcode::MULI },
        { "divi", Opcode::DIVI },
        { "li",   Opcode::LI   },
        { "inc",  Opcode::INC  },
        { "dec",  Opcode::DEC  },
        { "jump", Opcode::JUMP },
        { "bgez", Opcode::BGEZ },
        { "bgtz", Opcode::BGTZ },
        { "blez", Opcode::BLEZ },
        { "bltz", Opcode::BLTZ },
        { "beq",  Opcode::BEQ  },
        { "bne",  Opcode::BNE  },
        { "lw",   Opcode::LW   },
        { "sw",   Opcode::SW   },
        { "halt", Opcode::HALT }
    };

    return opcode_map.at(mnemonic);
}

bool Encoding::is_jump_or_branch() const
{
    return (opcode == JUMP || opcode == BEQ || opcode == BNE || opcode == BGEZ
        || opcode == BGTZ || opcode == BLEZ || opcode == BLTZ);
}

} // namespace HW::ISA
