#ifndef FCFS_HPP
#define FCFS_HPP

#include <queue>
#include "OS/Scheduler/SchedulerBase.hpp"

namespace OS {

class FIFOScheduler : SchedulerBase {
public:
    void push(std::bitset<32> proc) override;
    std::bitset<32> pop() override;
    bool empty() override;

private:
    std::queue<std::bitset<32>> queue_;
};

} // namespace OS

#endif // FCFS_HPP
