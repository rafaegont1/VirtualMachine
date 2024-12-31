#include "Hardware/CPU/RegFile.hpp"

#include <iostream>
#include <unordered_map>
#include <stdexcept>

namespace Hardware::CPU {

RegFile::RegNum RegFile::reg_index(const std::string& reg_name)
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

void RegFile::reg(RegFile::RegNum index, int32_t data) { gpr_[index] = data; }

int32_t RegFile::reg(uint8_t index) const
{
    if (index >= NUM_REGISTERS) {
        std::string num = std::to_string(index);
        throw std::runtime_error(
            "Error: Register number '" + num + "' is out of bounds."
        );
    }

    return gpr_[index];
}

void RegFile::gpr(RegFile::Registers& reg) { gpr_ = reg; }

const RegFile::Registers& RegFile::gpr() const { return gpr_; }

void RegFile::print_log(std::ofstream& log)
{
    log << "\nGENERAL-PURPOSE REGISTERS\n"
        << "$zero: " << gpr_[ZERO] << '\t'
        << "$at: "   << gpr_[AT]   << '\t'
        << "$v0: "   << gpr_[V0]   << '\t'
        << "$v1: "   << gpr_[V1]   << '\n'
        << "$a0: "   << gpr_[A0]   << '\t'
        << "$a1: "   << gpr_[A1]   << '\t'
        << "$a2: "   << gpr_[A2]   << '\t'
        << "$a3: "   << gpr_[A3]   << '\n'
        << "$t0: "   << gpr_[T0]   << '\t'
        << "$t1: "   << gpr_[T1]   << '\t'
        << "$t2: "   << gpr_[T2]   << '\t'
        << "$t3: "   << gpr_[T3]   << '\n'
        << "$t4: "   << gpr_[T4]   << '\t'
        << "$t5: "   << gpr_[T5]   << '\t'
        << "$t6: "   << gpr_[T6]   << '\t'
        << "$t7: "   << gpr_[T7]   << '\n'
        << "$s0: "   << gpr_[S0]   << '\t'
        << "$s1: "   << gpr_[S1]   << '\t'
        << "$s2: "   << gpr_[S2]   << '\t'
        << "$s3: "   << gpr_[S3]   << '\n'
        << "$s4: "   << gpr_[S4]   << '\t'
        << "$s5: "   << gpr_[S5]   << '\t'
        << "$s6: "   << gpr_[S6]   << '\t'
        << "$s7: "   << gpr_[S7]   << '\n'
        << "$t8: "   << gpr_[T8]   << '\t'
        << "$t9: "   << gpr_[T9]   << '\t'
        << "$k0: "   << gpr_[K0]   << '\t'
        << "$k1: "   << gpr_[K1]   << '\n'
        << "$gp: "   << gpr_[GP]   << '\t'
        << "$sp: "   << gpr_[SP]   << '\t'
        << "$fp: "   << gpr_[FP]   << '\t'
        << "$ra: "   << gpr_[RA]   << '\n'
        << std::endl;
}

} // namespace Hardware::CPU
