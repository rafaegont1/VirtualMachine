#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include "OS/PCB.hpp"

namespace HW::CPU {

class Pipeline {
public:
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

    // current process
    std::shared_ptr<OS::PCB> proc_;
};

} // namespace HW::CPU

#endif // PIPELINE_HPP_
