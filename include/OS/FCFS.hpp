#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <memory>
#include <queue>
#include <mutex>
#include "OS/PCB.hpp"

namespace OS {

class FCFS {
public:
    void push(std::shared_ptr<OS::PCB> proc);
    std::shared_ptr<OS::PCB> pop();
    bool empty() const;

private:
    std::queue<std::shared_ptr<OS::PCB>> queue_;
    std::mutex mtx_;
};

} // namespace OS

#endif // SCHEDULER_HPP
