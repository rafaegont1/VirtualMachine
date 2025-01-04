#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include <vector>
#include "HW/CPU/ALU.hpp"
#include "HW/CPU/RegFile.hpp"
#include "OS/PCB.hpp"
#include "HW/ISA/Encoding.hpp"

namespace HW::CPU {

class Pipeline {
public:
    // void run(std::shared_ptr<OS::PCB> proc);
    // pipeline stages
    void instr_fetch();
    void instr_decode();
    void execute();
    void mem_access();
    void write_back();

    // update pipeline instructions
    void update_stages();

    void set_proc(std::shared_ptr<OS::PCB> proc);
    void print_log(std::ofstream& log) const;

private:
    union ALUResult {
        int32_t as_num;
        bool as_bool;
    };

    uint32_t stall_count_ = 0;

    // current process
    std::shared_ptr<OS::PCB> proc_;

    // FIXME: arrumar uma maneira de salvar os estados dos estágios da pipeline
    // no PCB (acho que só estas variáveis instr_*_ mesmo, não tenho ctz)

    // current instructions in pipeline
    HW::ISA::Encoding instr_if_;
    HW::ISA::Encoding instr_id_;
    HW::ISA::Encoding instr_ex_;
    HW::ISA::Encoding instr_mem_;
    HW::ISA::Encoding instr_wb_;
};

} // namespace HW::CPU

#endif // PIPELINE_HPP_
