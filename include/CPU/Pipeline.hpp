#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <vector>
#include "CPU/ALU.hpp"
#include "CPU/RegFile.hpp"
#include "RAM.hpp"
#include "Process.hpp"
#include "CPU/Instruction.hpp"

namespace CPU {

class Pipeline {
public:
    Pipeline(uint32_t & clk, uint32_t & pc, RegFile & rf, RAM & mem, ALU & alu);
    virtual ~Pipeline() = default;

    void run(Process * proc, std::ofstream & log);

    void instr_fetch(Process * proc);
    void instr_decode(Process * proc);
    void execute();
    void mem_access(uint32_t pid);
    void write_back();

    void update();

    void print_log(std::ofstream & log);

private:
    // struct InstructionOperands {
    //     RegFile::RegNum rd, rs, rt;
    //     int32_t imm;
    // };

    union ALUResult {
        int32_t number;
        bool boolean;
    };

    static const std::vector<std::string> NOP_INSTR;

    uint32_t & clk;
    uint32_t & pc;
    RegFile & rf;
    RAM & mem;
    ALU & alu;

    // current instructions in pipeline
    Instruction instr_if;
    Instruction instr_id;
    Instruction instr_ex;
    Instruction instr_mem;
    Instruction instr_wb;
    // const std::vector<std::string> * instr_if = &NOP_INSTR;
    // const std::vector<std::string> * instr_id = &NOP_INSTR;
    // const std::vector<std::string> * instr_ex = &NOP_INSTR;
    // const std::vector<std::string> * instr_mem = &NOP_INSTR;
    // const std::vector<std::string> * instr_wb = &NOP_INSTR;

    // instruction operands in ID and EX stages
    // InstructionOperands id_op;
    // InstructionOperands ex_op;
};

} // namespace CPU

#endif // PIPELINE_HPP
