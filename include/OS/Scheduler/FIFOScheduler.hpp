#ifndef FCFS_HPP
#define FCFS_HPP

#include <queue>
#include <memory>
#include "OS/Scheduler/SchedulerBase.hpp"
#include "OS/PCB.hpp"

namespace OS {

class FIFOScheduler : SchedulerBase {
public:
    void push(std::shared_ptr<OS::PCB> proc) override;
    std::shared_ptr<OS::PCB> pop() override;
    bool empty() override;

private:
    std::queue<std::shared_ptr<OS::PCB>> queue_;
};

} // namespace OS

#endif // FCFS_HPP
