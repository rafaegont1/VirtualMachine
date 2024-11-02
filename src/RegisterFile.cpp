#include "RegisterFile.hpp"

#include <iostream>
#include <unordered_map>
#include <stdexcept>

RegisterFile::RegisterFile(std::ofstream *log) : log(log) {}

void RegisterFile::set_data(RegEnum reg_num, int32_t data)
{
    r[reg_num].data = data;
    r[reg_num].modified = true;
}

RegisterFile::RegEnum RegisterFile::get_enum(const std::string& regName)
{
    static const std::unordered_map<std::string, RegEnum> register_map = {
        {"$zero", RegEnum::ZERO},
        {"$at",   RegEnum::AT  },
        {"$v0",   RegEnum::V0  },
        {"$v1",   RegEnum::V1  },
        {"$a0",   RegEnum::A0  },
        {"$a1",   RegEnum::A1  },
        {"$a2",   RegEnum::A2  },
        {"$a3",   RegEnum::A3  },
        {"$t0",   RegEnum::T0  },
        {"$t1",   RegEnum::T1  },
        {"$t2",   RegEnum::T2  },
        {"$t3",   RegEnum::T3  },
        {"$t4",   RegEnum::T4  },
        {"$t5",   RegEnum::T5  },
        {"$t6",   RegEnum::T6  },
        {"$t7",   RegEnum::T7  },
        {"$s0",   RegEnum::S0  },
        {"$s1",   RegEnum::S1  },
        {"$s2",   RegEnum::S2  },
        {"$s3",   RegEnum::S3  },
        {"$s4",   RegEnum::S4  },
        {"$s5",   RegEnum::S5  },
        {"$s6",   RegEnum::S6  },
        {"$s7",   RegEnum::S7  },
        {"$t8",   RegEnum::T8  },
        {"$t9",   RegEnum::T9  },
        {"$k0",   RegEnum::K0  },
        {"$k1",   RegEnum::K1  },
        {"$gp",   RegEnum::GP  },
        {"$sp",   RegEnum::SP  },
        {"$fp",   RegEnum::FP  },
        {"$ra",   RegEnum::RA  }
    };

    auto it = register_map.find(regName);
    if (it == register_map.end()) {
        throw std::runtime_error("Error: Register '" + regName + "' not found.");
    }

    return it->second;
}

int32_t RegisterFile::get_data(uint8_t regNum) const
{
    if (regNum >= NUM_REGISTERS) {
        throw std::runtime_error("Error: Register number '"
            + std::to_string(regNum) + "' is out of bounds.");
    }

    return r[regNum].data; // Retorna o valor do registrador
}

void RegisterFile::update_regs(const int32_t* r)
{
    for (size_t i = 0; i < RegisterFile::NUM_REGISTERS; i++) {
        this->r[i].data = r[i];
    }
}

const RegisterFile::Register* RegisterFile::get_regs_data()
{
    return r.data();
}

void RegisterFile::print()
{
    *log << "\nGENERAL-PURPOSE REGISTERS\n"
         << "$zero: " << r[ZERO].data << '\t'
         << "$at: "   << r[AT].data   << '\t'
         << "$v0: "   << r[V0].data   << '\t'
         << "$v1: "   << r[V1].data   << '\n'
         << "$a0: "   << r[A0].data   << '\t'
         << "$a1: "   << r[A1].data   << '\t'
         << "$a2: "   << r[A2].data   << '\t'
         << "$a3: "   << r[A3].data   << '\n'
         << "$t0: "   << r[T0].data   << '\t'
         << "$t1: "   << r[T1].data   << '\t'
         << "$t2: "   << r[T2].data   << '\t'
         << "$t3: "   << r[T3].data   << '\n'
         << "$t4: "   << r[T4].data   << '\t'
         << "$t5: "   << r[T5].data   << '\t'
         << "$t6: "   << r[T6].data   << '\t'
         << "$t7: "   << r[T7].data   << '\n'
         << "$s0: "   << r[S0].data   << '\t'
         << "$s1: "   << r[S1].data   << '\t'
         << "$s2: "   << r[S2].data   << '\t'
         << "$s3: "   << r[S3].data   << '\n'
         << "$s4: "   << r[S4].data   << '\t'
         << "$s5: "   << r[S5].data   << '\t'
         << "$s6: "   << r[S6].data   << '\t'
         << "$s7: "   << r[S7].data   << '\n'
         << "$t8: "   << r[T8].data   << '\t'
         << "$t9: "   << r[T9].data   << '\t'
         << "$k0: "   << r[K0].data   << '\t'
         << "$k1: "   << r[K1].data   << '\n'
         << "$gp: "   << r[GP].data   << '\t'
         << "$sp: "   << r[SP].data   << '\t'
         << "$fp: "   << r[FP].data   << '\t'
         << "$ra: "   << r[RA].data   << '\n'
         << std::endl;
}
