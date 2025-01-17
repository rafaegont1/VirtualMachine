#ifndef SCHEDULER_BASE_HPP
#define SCHEDULER_BASE_HPP

#include <memory>
#include <mutex>
#include "OS/PCB.hpp"

namespace OS {

class SchedulerBase {
public:
    virtual void push(std::shared_ptr<PCB> proc) = 0;
    virtual std::shared_ptr<PCB> pop() = 0;
    virtual bool empty() = 0;

protected:
    std::mutex mtx_;
};

} // namespace OS

#endif // SCHEDULER_BASE_HPP
