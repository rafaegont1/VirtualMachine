#ifndef FCFS_HPP
#define FCFS_HPP

#include <queue>
#include "Process.hpp"

namespace OS {

class FCFS {
public:
    FCFS();
    virtual ~FCFS() = default;

    void push(Process & proc);
    Process & pop();

private:
    std::queue<Process *> queue;
};

} // namespace OS

#endif // FCFS_HPP
