#ifndef SCHEDULER_BASE_HPP
#define SCHEDULER_BASE_HPP

#include <bitset>
#include <mutex>

namespace OS {

class SchedulerBase {
public:
    virtual void push(std::bitset<32> proc) = 0;
    virtual std::bitset<32> pop() = 0;
    virtual bool empty() = 0;

protected:
    std::mutex mtx_;
};

} // namespace OS

#endif // SCHEDULER_BASE_HPP
