#include "Process.hpp"
#include "RegisterFile.hpp"

uint32_t Process::count = 0;

Process::Process()
{
    pid = ++count;

    r[RegisterFile::ZERO] = 0;

    for (int n = 1; n < RegisterFile::NUM_REGISTERS; n++) {
        r[n] = n + 100;
    }
}

Process::~Process() {}

void Process::set_code(const std::vector<std::vector<std::string>>& code) {
    this->code = code;
}

void Process::write_regs(const RegisterFile::Register* r)
{
    for (size_t i = 0; i < RegisterFile::NUM_REGISTERS; i++) {
        if (r[i].modified == true) {
            this->r[i] = r[i].data;
        }
    }
}

const int32_t* Process::read_regs()
{
    return r.data();
}
