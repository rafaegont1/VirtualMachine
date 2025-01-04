#include <iostream>
#include <cctype>
#include <unordered_map>
#include "HW/ISA/Encoding.hpp"

namespace HW::ISA {

Encoding::Opcode Encoding::get_opcode(const std::string& mnemonic)
{
    static const std::unordered_map<std::string, Opcode> opcode_map = {
        {"nop",   Opcode::NOP  },
        {"add",   Opcode::ADD  },
        {"sub",   Opcode::SUB  },
        {"mult",  Opcode::MUL  },
        {"div",   Opcode::DIV  },
        {"li",    Opcode::LI   },
        {"inc",   Opcode::INC  },
        {"dec",   Opcode::DEC  },
        {"jump",  Opcode::JUMP },
        {"bgez",  Opcode::BGEZ },
        {"bgtz",  Opcode::BGTZ },
        {"blez",  Opcode::BLEZ },
        {"bltz",  Opcode::BLTZ },
        {"beq",   Opcode::BEQ  },
        {"bne",   Opcode::BNE  },
        {"load",  Opcode::LOAD },
        {"store", Opcode::STORE},
        {"halt",  Opcode::HALT }
    };

    auto iter = opcode_map.find(mnemonic);
    if (iter == opcode_map.end()) {
        throw std::runtime_error(
            "opcode for mnemonic '" + mnemonic + "' not found"
        );
    }

    return iter->second;
}

bool Encoding::is_branch() const
{
    return (opcode == JUMP || opcode == BEQ || opcode == BNE || opcode == BGEZ
        || opcode == BGTZ || opcode == BLEZ || opcode == BLTZ);
}

} // namespace HW::ISA
