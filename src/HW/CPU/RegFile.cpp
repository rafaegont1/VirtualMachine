#include "HW/CPU/RegFile.hpp"

#include <unordered_map>
#include <stdexcept>

namespace HW::CPU {

RegFile::RegFile()
{
    gpr_[HW::CPU::RegFile::ZERO] = 0;

    for (int n = 1; n < HW::CPU::RegFile::NUM_REGISTERS; n++) {
        gpr_[n] = n + 100;
    }
}

RegFile::Index RegFile::reg_index(const std::string& reg_alias)
{
    static const std::unordered_map<std::string, Index> reg_map = {
        {"$zero", ZERO},
        {"$at",   AT  },
        {"$v0",   V0  },
        {"$v1",   V1  },
        {"$a0",   A0  },
        {"$a1",   A1  },
        {"$a2",   A2  },
        {"$a3",   A3  },
        {"$t0",   T0  },
        {"$t1",   T1  },
        {"$t2",   T2  },
        {"$t3",   T3  },
        {"$t4",   T4  },
        {"$t5",   T5  },
        {"$t6",   T6  },
        {"$t7",   T7  },
        {"$s0",   S0  },
        {"$s1",   S1  },
        {"$s2",   S2  },
        {"$s3",   S3  },
        {"$s4",   S4  },
        {"$s5",   S5  },
        {"$s6",   S6  },
        {"$s7",   S7  },
        {"$t8",   T8  },
        {"$t9",   T9  },
        {"$k0",   K0  },
        {"$k1",   K1  },
        {"$gp",   GP  },
        {"$sp",   SP  },
        {"$fp",   FP  },
        {"$ra",   RA  }
    };

    auto iter = reg_map.find(reg_alias);
    if (iter == reg_map.end()) {
        throw std::out_of_range("Register '" + reg_alias + "' not found.");
    }

    return iter->second;
}

void RegFile::reg(RegFile::Index index, int32_t data) { gpr_[index] = data; }

int32_t RegFile::reg(uint8_t index) const
{
    if (index < Index::ZERO || index > Index::RA) {
        throw std::out_of_range(
            "register of index '" + std::to_string(index) + "' is out of bounds."
        );
    }

    return gpr_[index];
}

void RegFile::gpr(RegFile::Registers& reg) { gpr_ = reg; }

const RegFile::Registers& RegFile::gpr() const { return gpr_; }

void RegFile::write_log(std::ofstream& log)
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

} // namespace HW::CPU
