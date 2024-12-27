#ifndef FCFS_HPP_
#define FCFS_HPP_

#include <queue>
#include "OS/Process.hpp"

namespace OS::Scheduling {

class FCFS {
public:
    void push(Process& proc);
    Process& pop();
    bool empty() const;

private:
    std::queue<Process*> queue;
};

} // namespace OS::Scheduling

#endif // FCFS_HPP_
