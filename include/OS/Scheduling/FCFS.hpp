#ifndef FCFS_HPP_
#define FCFS_HPP_

#include <queue>
#include "OS/PCB.hpp"

namespace OS::Scheduling {

class FCFS {
public:
    void push(PCB& proc);
    PCB& pop();
    bool empty() const;

private:
    std::queue<PCB*> queue;
};

} // namespace OS::Scheduling

#endif // FCFS_HPP_
