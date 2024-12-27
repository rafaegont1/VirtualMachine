#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include <vector>
#include "Hardware/CPU/ALU.hpp"
#include "Hardware/CPU/RegFile.hpp"
#include "Hardware/RAM.hpp"
#include "OS/Process.hpp"
#include "Hardware/CPU/Instruction.hpp"

namespace Hardware::CPU {

class Pipeline {
public:
    void instr_fetch(uint32_t& pc, const OS::Process& proc);
    void instr_decode(OS::Process& proc);
    void execute(RegFile& rf, uint32_t& pc);
    void mem_access(uint32_t pid, RegFile& rf, RAM& mem);
    void write_back();

    void update();

    void print_log(std::ofstream& log);

private:
    union ALUResult {
        int32_t number;
        bool boolean;
    };

    static const std::vector<std::string> NOP_INSTR;

    ALU alu_;

    // current instructions in pipeline
    Instruction instr_if;
    Instruction instr_id;
    Instruction instr_ex;
    Instruction instr_mem;
    Instruction instr_wb;
};

} // namespace Hardware::CPU

#endif // PIPELINE_HPP_
