#include "VirtualMachine.hpp"

#include <iostream>
#include "Instruction.hpp"

VirtualMachine::VirtualMachine(const std::string& log_file) :
    log(log_file), cpu(&log, &mem), mem(&log)
{
    std::cout << "O log da execução será gravado em '"
        << log_file << "'" << std::endl;
}

VirtualMachine::~VirtualMachine()
{
    log.close();
}

void VirtualMachine::create_proc(const std::string& code_file)
{
    mem.new_process(Process(code_file));
}

void VirtualMachine::run()
{
    proc.push_back(mem.get_next_proc());
    cpu.run(proc.back());
}
