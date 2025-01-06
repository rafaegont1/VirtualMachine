#ifndef REGISTER_FILE_HPP_
#define REGISTER_FILE_HPP_

#include <fstream>
#include <string>
#include <array>
#include <cstdint>

namespace HW::CPU {

class RegFile {
public:
    enum Index : uint8_t {
        ZERO = 0, // $zero
        AT,       // $at
        V0,       // $v0
        V1,       // $v1
        A0,       // $a0
        A1,       // $a1
        A2,       // $a2
        A3,       // $a3
        T0,       // $t0
        T1,       // $t1
        T2,       // $t2
        T3,       // $t3
        T4,       // $t4
        T5,       // $t5
        T6,       // $t6
        T7,       // $t7
        S0,       // $s0
        S1,       // $s1
        S2,       // $s2
        S3,       // $s3
        S4,       // $s4
        S5,       // $s5
        S6,       // $s6
        S7,       // $s7
        T8,       // $t8
        T9,       // $t9
        K0,       // $k0
        K1,       // $k1
        GP,       // $gp
        SP,       // $sp
        FP,       // $fp
        RA        // $ra
    };

    static const uint8_t NUM_REGISTERS = 32;

    using Registers = std::array<int32_t, RegFile::NUM_REGISTERS>;

    RegFile();
    static Index reg_index(const std::string& reg_alias);

    // setters and getters
    void reg(Index index, int32_t data);
    int32_t reg(uint8_t index) const;
    void gpr(Registers& reg);
    const Registers& gpr() const;

    void print_log(std::ofstream& log) const;

private:
    Registers gpr_; // General-Purpose Registers
};

} // namespace HW::CPU

#endif // REGISTER_FILE_HPP_
