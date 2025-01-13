#include "OS/Scheduler/FIFO.hpp"

namespace OS {

void FIFO::push(std::shared_ptr<OS::PCB> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    queue_.push(proc);
}

std::shared_ptr<OS::PCB> FIFO::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (queue_.empty()) {
        return nullptr;
    }

    std::shared_ptr<OS::PCB> proc = queue_.front();
    queue_.pop();

    return proc;
}

bool FIFO::empty()
{
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.empty();
}

} // namespace OS
