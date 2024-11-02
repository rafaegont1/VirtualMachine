#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <array>
#include <vector>
#include <cstdint>
#include "RegisterFile.hpp"

class Process
{
public:
    Process(const std::string& code_file);
    virtual ~Process() = default;

    // void set_code(const std::vector<std::vector<std::string>>& code);
    void write_regs(const RegisterFile::Register* r);
    const int32_t* read_regs();

    uint32_t pc = 0;
    std::array<int32_t, RegisterFile::NUM_REGISTERS> r;
    std::vector<std::vector<std::string>> code;

private:
    static uint32_t count;

    uint32_t pid;
};

#endif // PROCESS_HPP
