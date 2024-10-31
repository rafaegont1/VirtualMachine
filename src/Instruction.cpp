#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Instruction.hpp"

Instruction::Opcode Instruction::get_opcode(const std::string& opcode_name)
{
    static const std::unordered_map<std::string, Instruction::Opcode>
    opcode_map = {
        {"nop",   Instruction::Opcode::NOP  },
        {"add",   Instruction::Opcode::ADD  },
        {"sub",   Instruction::Opcode::SUB  },
        {"mult",  Instruction::Opcode::MULT },
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
        {"store", Instruction::Opcode::STORE}
    };

    auto it = opcode_map.find(opcode_name);

    if (it == opcode_map.end()) {
        throw std::runtime_error("Error: opcode '" + opcode_name + "' not found");
    }

    return it->second;
}

std::vector<std::vector<std::string>>
Instruction::get_instructions(const std::string& file_name)
{
    std::vector<std::vector<std::string>> code;
    std::ifstream file(file_name);
    std::string line;

    if (!file) {
        throw std::runtime_error("Error: failled to open file " + file_name);
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> instruction;
        std::string word;

        while (iss >> word) {
            instruction.push_back(word);
        }

        code.push_back(instruction);
    }

    file.close();
    return code;
}

bool Instruction::is_branch(Instruction::Opcode opcode)
{
    return (opcode == BEQ || opcode == BNE || opcode == BGEZ
        || opcode == BGTZ || opcode == BLEZ || opcode == BLTZ);
}
