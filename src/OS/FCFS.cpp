#include "OS/FCFS.hpp"

#include <stdexcept>

namespace OS {

void FCFS::push(std::shared_ptr<OS::PCB> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    proc->set_state(PCB::State::READY);
    queue_.push(proc);
}

std::shared_ptr<OS::PCB> FCFS::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (queue_.empty()) {
        return nullptr;
        // throw std::out_of_range("Can't pop FCFS queue, it's empty");
    }

    std::shared_ptr<OS::PCB> proc = queue_.front();
    queue_.pop();

    return proc;
}

bool FCFS::empty() const
{
    return queue_.empty();
}

} // namespace OS
