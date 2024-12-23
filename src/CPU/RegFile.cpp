#include "CPU/RegFile.hpp"

#include <iostream>
#include <unordered_map>
#include <stdexcept>

namespace CPU {

RegFile::RegFile()
{
}

void RegFile::set_data(RegNum reg_num, int32_t data)
{
    reg[reg_num] = data;
}

RegFile::RegNum RegFile::get_reg_num(const std::string & reg_name)
{
    static const std::unordered_map<std::string, RegNum> register_map = {
        {"$zero", RegNum::ZERO},
        {"$at",   RegNum::AT  },
        {"$v0",   RegNum::V0  },
        {"$v1",   RegNum::V1  },
        {"$a0",   RegNum::A0  },
        {"$a1",   RegNum::A1  },
        {"$a2",   RegNum::A2  },
        {"$a3",   RegNum::A3  },
        {"$t0",   RegNum::T0  },
        {"$t1",   RegNum::T1  },
        {"$t2",   RegNum::T2  },
        {"$t3",   RegNum::T3  },
        {"$t4",   RegNum::T4  },
        {"$t5",   RegNum::T5  },
        {"$t6",   RegNum::T6  },
        {"$t7",   RegNum::T7  },
        {"$s0",   RegNum::S0  },
        {"$s1",   RegNum::S1  },
        {"$s2",   RegNum::S2  },
        {"$s3",   RegNum::S3  },
        {"$s4",   RegNum::S4  },
        {"$s5",   RegNum::S5  },
        {"$s6",   RegNum::S6  },
        {"$s7",   RegNum::S7  },
        {"$t8",   RegNum::T8  },
        {"$t9",   RegNum::T9  },
        {"$k0",   RegNum::K0  },
        {"$k1",   RegNum::K1  },
        {"$gp",   RegNum::GP  },
        {"$sp",   RegNum::SP  },
        {"$fp",   RegNum::FP  },
        {"$ra",   RegNum::RA  }
    };

    auto iter = register_map.find(reg_name);
    if (iter == register_map.end()) {
        throw std::runtime_error(
            "Error: Register '" + reg_name + "' not found."
        );
    }

    return iter->second;
}

int32_t RegFile::get_data(uint8_t regNum)
{
    if (regNum >= NUM_REGISTERS) {
        std::string num = std::to_string(regNum);
        throw std::runtime_error(
            "Error: Register number '" + num + "' is out of bounds."
        );
    }

    return reg[regNum];
}

void RegFile::set_reg(const RegFile::Registers & reg)
{
    this->reg = reg;
}

RegFile::Registers & RegFile::get_reg()
{
    return reg;
}

void RegFile::print_log(std::ofstream & log)
{
    log << "\nGENERAL-PURPOSE REGISTERS\n"
        << "$zero: " << reg[ZERO] << '\t'
        << "$at: "   << reg[AT]   << '\t'
        << "$v0: "   << reg[V0]   << '\t'
        << "$v1: "   << reg[V1]   << '\n'
        << "$a0: "   << reg[A0]   << '\t'
        << "$a1: "   << reg[A1]   << '\t'
        << "$a2: "   << reg[A2]   << '\t'
        << "$a3: "   << reg[A3]   << '\n'
        << "$t0: "   << reg[T0]   << '\t'
        << "$t1: "   << reg[T1]   << '\t'
        << "$t2: "   << reg[T2]   << '\t'
        << "$t3: "   << reg[T3]   << '\n'
        << "$t4: "   << reg[T4]   << '\t'
        << "$t5: "   << reg[T5]   << '\t'
        << "$t6: "   << reg[T6]   << '\t'
        << "$t7: "   << reg[T7]   << '\n'
        << "$s0: "   << reg[S0]   << '\t'
        << "$s1: "   << reg[S1]   << '\t'
        << "$s2: "   << reg[S2]   << '\t'
        << "$s3: "   << reg[S3]   << '\n'
        << "$s4: "   << reg[S4]   << '\t'
        << "$s5: "   << reg[S5]   << '\t'
        << "$s6: "   << reg[S6]   << '\t'
        << "$s7: "   << reg[S7]   << '\n'
        << "$t8: "   << reg[T8]   << '\t'
        << "$t9: "   << reg[T9]   << '\t'
        << "$k0: "   << reg[K0]   << '\t'
        << "$k1: "   << reg[K1]   << '\n'
        << "$gp: "   << reg[GP]   << '\t'
        << "$sp: "   << reg[SP]   << '\t'
        << "$fp: "   << reg[FP]   << '\t'
        << "$ra: "   << reg[RA]   << '\n'
        << std::endl;
}

} // namespace CPU
