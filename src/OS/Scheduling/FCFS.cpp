#include "OS/Scheduling/FCFS.hpp"

namespace OS::Scheduling {

void FCFS::push(PCB& proc)
{
    queue.push(&proc);
}

PCB& FCFS::pop()
{
    if (queue.empty()) {
        throw std::runtime_error("Can't fetch process; FCFS queue is empty");
    }

    PCB& proc = *queue.front();

    queue.pop();

    return proc;
}

bool FCFS::empty() const
{
    return queue.empty();
}

} // namespace OS::Scheduling
