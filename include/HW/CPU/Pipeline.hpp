#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include <memory>
#include "OS/PCB.hpp"
#include "HW/CPU/Cache.hpp"

namespace HW::CPU {

class Pipeline {
public:
    // pipeline stages
    void instr_fetch(Cache& cache);
    void instr_decode();
    void execute(Cache& cache);
    void mem_access(Cache& cache);
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
    bool cache_used_ = false;
};

} // namespace HW::CPU

#endif // PIPELINE_HPP_
