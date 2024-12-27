#include "OS/Scheduling/FCFS.hpp"

namespace OS::Scheduling {

void FCFS::push(Process& proc)
{
    queue.push(&proc);
}

Process& FCFS::pop()
{
    if (queue.empty()) {
        throw std::runtime_error("Can't fetch process; FCFS queue is empty");
    }

    Process& proc = *queue.front();

    queue.pop();

    return proc;
}

bool FCFS::empty() const
{
    return queue.empty();
}

} // namespace OS::Scheduling
