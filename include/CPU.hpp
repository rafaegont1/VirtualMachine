#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <cstdint>
#include "Memory.hpp"
#include "Process.hpp"

class CPU
{
public:
    CPU(Memory *mem);
    virtual ~CPU() = default;

    void run(Process& proc);

private:
    void instr_fetch(Process& process);
    void instr_decode();
    void execute(Process &process);
    void mem_access();
    void write_back();

    int32_t alu(int32_t opnd1, int32_t opnd2, char op);
    void update();
    void print_pipeline();
    bool finished();

    static const std::vector<std::string> NOP_INSTR;

    uint32_t clk = 0;

    RegisterFile rf;

    Memory *mem;

    const std::vector<std::string> *instr_if  = &NOP_INSTR;
    const std::vector<std::string> *instr_id  = &NOP_INSTR;
    const std::vector<std::string> *instr_ex  = &NOP_INSTR;
    const std::vector<std::string> *instr_mem = &NOP_INSTR;
    const std::vector<std::string> *instr_wb  = &NOP_INSTR;

    RegisterFile::RegEnum id_rd, id_rs, id_rt, ex_rd, ex_rs, ex_rt;
    uint32_t id_imm, ex_imm;
};

#endif // PIPELINE_HPP
