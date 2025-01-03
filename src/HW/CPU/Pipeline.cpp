#include "HW/CPU/Pipeline.hpp"

#include "HW/ISA/Code.hpp"
#include "HW/CPU/RegFile.hpp"
#include "HW/ISA/Encoding.hpp"
#include "HW/RAM/DataSpace.hpp"
#include <fstream>

namespace HW::CPU {

void Pipeline::set_proc(std::shared_ptr<OS::PCB> proc)
{
    proc_ = proc;
}

void Pipeline::run(std::shared_ptr<OS::PCB> proc)
{
    proc_ = proc;
    // do {
        // log << "CLOCK: " << ++clk       << '\n';
             // << "PID: "   << proc->pid() << '\n'
             // << "Process CPU time: " << proc->cpu_time().count() << '\n';

    instr_fetch();
    instr_decode();
    execute();
    mem_access();
    write_back();

    print_log(proc->log);
    proc_->mem.print_log(proc->pid(), log); // TODO
    // log << "==============================================================\n";

    update_stages();
    // } while ((proc.state() != OS::PCB::State::TERMINATED)
    //     && (proc.cpu_time() < proc.quantum()));
}

void Pipeline::instr_fetch()
{
    OS::PCB::CPUState& cpu = proc_->cpu_state;

    if (stall_count_ > 0) {
        instr_if_.code_line = HW::ISA::Code::NOP_LINE;
        // instr_if.proc = nullptr;
        stall_count_--;
    } else {
        instr_if_.code_line = proc_->fetch_line(cpu.pc++);
        // instr_if.proc = proc;
    }
}

void Pipeline::instr_decode()
{
    auto code_line = instr_id_.code_line.get();
    instr_id_.opcode = ISA::Encoding::get_opcode(code_line[0]);

    switch (instr_id_.opcode) {
        case ISA::Encoding::Opcode::NOP:
            break;

        case ISA::Encoding::Opcode::ADD:
        case ISA::Encoding::Opcode::SUB:
        case ISA::Encoding::Opcode::MUL:
        case ISA::Encoding::Opcode::DIV:
            instr_id_.rd = RegFile::reg_index(code_line[1]);
            instr_id_.rs = RegFile::reg_index(code_line[2]);
            instr_id_.rt = RegFile::reg_index(code_line[3]);
            break;

        case ISA::Encoding::Opcode::LI:
            instr_id_.rd = RegFile::reg_index(code_line[1]);
            instr_id_.imm = std::stoi(code_line[2]);
            break;

        case ISA::Encoding::Opcode::INC:
        case ISA::Encoding::Opcode::DEC:
            instr_id_.rd = RegFile::reg_index(code_line[1]);
            break;

        case ISA::Encoding::Opcode::JUMP:
            instr_id_.imm = std::stoi(code_line[1]);
            break;

        case ISA::Encoding::Opcode::BEQ:
        case ISA::Encoding::Opcode::BNE:
            instr_id_.rs = RegFile::reg_index(code_line[1]);
            instr_id_.rt = RegFile::reg_index(code_line[2]);
            instr_id_.imm = std::stoi(code_line[3]);
            break;

        case ISA::Encoding::Opcode::BGEZ:
        case ISA::Encoding::Opcode::BGTZ:
        case ISA::Encoding::Opcode::BLEZ:
        case ISA::Encoding::Opcode::BLTZ:
            instr_id_.rs = RegFile::reg_index(code_line[1]);
            instr_id_.imm = std::stoi(code_line[2]);
            break;

        case ISA::Encoding::Opcode::LOAD:
            instr_id_.rd = RegFile::reg_index(code_line[1]);
            break;

        case ISA::Encoding::Opcode::STORE:
            instr_id_.rs = RegFile::reg_index(code_line[2]);
            break;

        case ISA::Encoding::Opcode::HALT:
            break;
    }
}

void Pipeline::execute()
{
    if (instr_ex_.opcode == ISA::Encoding::Opcode::NOP) return;

    OS::PCB::CPUState& cpu = proc_->cpu_state;
    ALUResult alu_res;
    int32_t rd_data, rs_data, rt_data;

    rd_data = cpu.rf.reg(instr_ex_.rd);
    rs_data = cpu.rf.reg(instr_ex_.rs);
    rt_data = cpu.rf.reg(instr_ex_.rt);

    switch (instr_ex_.opcode) {
        case ISA::Encoding::Opcode::NOP:
            break;

        case ISA::Encoding::Opcode::ADD:
            alu_res.as_num = HW::CPU::ALU::add(rs_data, rt_data);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::SUB:
            alu_res.as_num = HW::CPU::ALU::sub(rs_data, rt_data);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::MUL:
            alu_res.as_num = HW::CPU::ALU::mul(rs_data, rt_data);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::DIV:
            alu_res.as_num = HW::CPU::ALU::div(rs_data, rt_data);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::LI:
            cpu.rf.reg(instr_ex_.rd, instr_ex_.imm);
            break;

        case ISA::Encoding::Opcode::INC:
            alu_res.as_num = HW::CPU::ALU::add(rd_data, 1);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::DEC:
            alu_res.as_num = HW::CPU::ALU::sub(rd_data, 1);
            cpu.rf.reg(instr_ex_.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::JUMP:
            cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            break;

        case ISA::Encoding::Opcode::BEQ:
            alu_res.as_bool = HW::CPU::ALU::eq(rs_data, rt_data);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BNE:
            alu_res.as_bool = HW::CPU::ALU::ne(rs_data, rt_data);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BGEZ:
            alu_res.as_bool = HW::CPU::ALU::ge(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BGTZ:
            alu_res.as_bool = HW::CPU::ALU::gt(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BLEZ:
            alu_res.as_bool = HW::CPU::ALU::le(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BLTZ:
            alu_res.as_bool = HW::CPU::ALU::lt(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(instr_ex_.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::LOAD:
            break;

        case ISA::Encoding::Opcode::STORE:
            break;

        case ISA::Encoding::Opcode::HALT:
            break;
    }
}

void Pipeline::mem_access()
{
    auto code_line = instr_id_.code_line.get();
    OS::PCB::CPUState cpu = proc_->cpu_state;
    HW::RAM::DataSpace& mem = proc_->mem;
    HW::RAM::DataSpace::Variable var;

    if (instr_mem_.opcode == ISA::Encoding::Opcode::LOAD) {
        // var.name = instr_mem.line.get()[2];
        // var.data = mem.read_data(pid, var.name);

        // rf_.reg(instr_mem.rd, var.data);

        var.name = code_line[2];
        var.data = mem.read(var.name);

        cpu.rf.reg(instr_mem_.rd, var.data);
    } else if (instr_mem_.opcode == ISA::Encoding::Opcode::STORE) {
        // var.name = (*instr_wb.line)[1];
        // var.data = rf_.reg(instr_mem.rs);

        // mem.write_data(pid, var);
        var.name = code_line[1];
        var.data = cpu.rf.reg(instr_mem_.rs);

        mem.write(var);
    }
}

void Pipeline::write_back()
{
    if (instr_mem_.opcode == ISA::Encoding::Opcode::HALT) {
        proc_->set_state(OS::PCB::State::TERMINATED);
    }
}

void Pipeline::update_stages()
{
    instr_wb_  = std::move(instr_mem_);
    instr_mem_ = std::move(instr_ex_);
    instr_ex_  = std::move(instr_id_);
    instr_id_  = std::move(instr_if_);
    instr_if_.code_line  = ISA::Code::NOP_LINE;
}

void Pipeline::print_log(std::ofstream& log)
{
    OS::PCB::CPUState cpu = proc_->cpu_state;
    cpu.rf.write_log(log);

    log << "PIPELINE\n";

    auto write_instr = [&log](const std::string& label, const HW::ISA::Encoding& instr)
    {
        log << label;
        for (const auto& word : instr.code_line.get()) {
            log << '\t' << word;
        }
        log << "\n";
    };

    write_instr("IF:", instr_if_);
    write_instr("ID:", instr_id_);
    write_instr("EX:", instr_ex_);
    write_instr("MEM:", instr_mem_);
    write_instr("WB:", instr_wb_);
}

} // namespace HW::CPU
