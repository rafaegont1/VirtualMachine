#include "OS/FCFS.hpp"

namespace OS {

FCFS::FCFS()
{
}

void FCFS::push(Process & proc)
{
    queue.push(&proc);
}

Process & FCFS::pop()
{
    if (queue.empty()) {
        throw std::runtime_error("Can't fetch process; FCFS queue is empty");
    }

    Process & proc = *queue.front();

    queue.pop();

    return proc;
}

} // namespace OS
