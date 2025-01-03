#include "OS/Scheduler.hpp"

#include <stdexcept>

namespace OS {

void Scheduler::push(std::shared_ptr<OS::PCB> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    proc->set_state(PCB::State::READY);
    fcfs.push(proc);
}

std::shared_ptr<OS::PCB> Scheduler::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (fcfs.empty()) {
        throw std::out_of_range("Can't restore context; FCFS queue is empty");
    }

    std::shared_ptr<OS::PCB> proc = fcfs.front();
    fcfs.pop();

    return proc;
}

bool Scheduler::empty() const
{
    return fcfs.empty();
}

} // namespace OS
