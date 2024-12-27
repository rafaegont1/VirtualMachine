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

void RegFile::reg(RegFile::RegNum index, int32_t data) { reg_[index] = data; }

int32_t RegFile::reg(uint8_t index) const
{
    if (index >= NUM_REGISTERS) {
        std::string num = std::to_string(index);
        throw std::runtime_error(
            "Error: Register number '" + num + "' is out of bounds."
        );
    }

    return reg_[index];
}

void RegFile::reg(RegFile::Registers& reg) { reg_ = reg; }

const RegFile::Registers& RegFile::reg() const { return reg_; }

void RegFile::print_log(std::ofstream& log)
{
    log << "\nGENERAL-PURPOSE REGISTERS\n"
        << "$zero: " << reg_[ZERO] << '\t'
        << "$at: "   << reg_[AT]   << '\t'
        << "$v0: "   << reg_[V0]   << '\t'
        << "$v1: "   << reg_[V1]   << '\n'
        << "$a0: "   << reg_[A0]   << '\t'
        << "$a1: "   << reg_[A1]   << '\t'
        << "$a2: "   << reg_[A2]   << '\t'
        << "$a3: "   << reg_[A3]   << '\n'
        << "$t0: "   << reg_[T0]   << '\t'
        << "$t1: "   << reg_[T1]   << '\t'
        << "$t2: "   << reg_[T2]   << '\t'
        << "$t3: "   << reg_[T3]   << '\n'
        << "$t4: "   << reg_[T4]   << '\t'
        << "$t5: "   << reg_[T5]   << '\t'
        << "$t6: "   << reg_[T6]   << '\t'
        << "$t7: "   << reg_[T7]   << '\n'
        << "$s0: "   << reg_[S0]   << '\t'
        << "$s1: "   << reg_[S1]   << '\t'
        << "$s2: "   << reg_[S2]   << '\t'
        << "$s3: "   << reg_[S3]   << '\n'
        << "$s4: "   << reg_[S4]   << '\t'
        << "$s5: "   << reg_[S5]   << '\t'
        << "$s6: "   << reg_[S6]   << '\t'
        << "$s7: "   << reg_[S7]   << '\n'
        << "$t8: "   << reg_[T8]   << '\t'
        << "$t9: "   << reg_[T9]   << '\t'
        << "$k0: "   << reg_[K0]   << '\t'
        << "$k1: "   << reg_[K1]   << '\n'
        << "$gp: "   << reg_[GP]   << '\t'
        << "$sp: "   << reg_[SP]   << '\t'
        << "$fp: "   << reg_[FP]   << '\t'
        << "$ra: "   << reg_[RA]   << '\n'
        << std::endl;
}

} // namespace Hardware::CPU
