#ifndef REGISTER_FILE
#define REGISTER_FILE

#include <fstream>
#include <string>
#include <array>
#include <cstdint>

class RegisterFile {
public:
    enum RegEnum : uint8_t {
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

    struct Register {
        int32_t data;
        bool modified = false;
    };

    RegisterFile(std::ofstream *log);
    virtual ~RegisterFile() = default;

    static RegEnum get_enum(const std::string& regName);

    void set_data(RegEnum reg_num, int32_t data);
    int32_t get_data(uint8_t regNum) const;
    void update_regs(const int32_t* r);
    const Register* get_regs_data();
    void print();

private:
    std::array<Register, NUM_REGISTERS> r;

    std::ofstream *log;
};

#endif // REGISTER_FILE
