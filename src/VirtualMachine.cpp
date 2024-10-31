#include "VirtualMachine.hpp"
#include "Instruction.hpp"

VirtualMachine::VirtualMachine() : cpu(&mem) {}

void VirtualMachine::create_proc(const std::string& code_file)
{
    std::vector<std::vector<std::string>> code;
    Process new_proc;

    code = Instruction::get_instructions(code_file);
    new_proc.set_code(code);
    mem.new_process(new_proc);
}

void VirtualMachine::run()
{
    proc.push_back(mem.get_next_process());
    cpu.run(proc.back());
}
