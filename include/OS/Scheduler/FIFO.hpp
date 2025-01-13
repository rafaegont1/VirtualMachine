#ifndef FCFS_HPP
#define FCFS_HPP

#include <memory>
#include <queue>
#include "OS/Scheduler/SchedulerBase.hpp"
#include "OS/PCB.hpp"

namespace OS {

class FIFO : SchedulerBase {
public:
    void push(std::shared_ptr<OS::PCB> proc);
    std::shared_ptr<OS::PCB> pop();
    bool empty();

private:
    std::queue<std::shared_ptr<OS::PCB>> queue_;
};

} // namespace OS

#endif // FCFS_HPP
