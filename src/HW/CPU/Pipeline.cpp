#include "HW/CPU/Pipeline.hpp"

#include "HW/CPU/ALU.hpp"
#include "HW/ISA/Encoding.hpp"
#include "HW/RAM/DataSpace.hpp"
#include <fstream>

namespace HW::CPU {

void Pipeline::instr_fetch()
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    if (cpu.pipeline.stall_count_ > 0) {
        cpu.pipeline.if_enc.code_line = HW::ISA::Code::NOP_LINE;
        cpu.pipeline.stall_count_--;
    } else {
        cpu.pipeline.if_enc.code_line = proc_->fetch_line(cpu.pc++);

        // HACK: it's preferable to get the opcode of the instruction in ID
        // stage, but we need to know it to put stalls so the branch or jump is
        // evaluated before any other instruction is fetched
        auto code_line = cpu.pipeline.if_enc.code_line.get();
        cpu.pipeline.if_enc.opcode = ISA::Encoding::get_opcode(code_line[0]);
        if (cpu.pipeline.if_enc.is_jump_or_branch()) {
            cpu.pipeline.stall_count_ += 2;
        }
    }
}

void Pipeline::instr_decode()
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    auto code_line = cpu.pipeline.id_enc.code_line.get();
    cpu.pipeline.id_enc.opcode = ISA::Encoding::get_opcode(code_line[0]);

    switch (cpu.pipeline.id_enc.opcode) {
        case ISA::Encoding::Opcode::NOP:
            break;

        case ISA::Encoding::Opcode::ADD:
        case ISA::Encoding::Opcode::SUB:
        case ISA::Encoding::Opcode::MUL:
        case ISA::Encoding::Opcode::DIV:
            cpu.pipeline.id_enc.rd = RegFile::reg_index(code_line[1]);
            cpu.pipeline.id_enc.rs = RegFile::reg_index(code_line[2]);
            cpu.pipeline.id_enc.rt = RegFile::reg_index(code_line[3]);
            break;

        case ISA::Encoding::Opcode::LI:
            cpu.pipeline.id_enc.rd = RegFile::reg_index(code_line[1]);
            cpu.pipeline.id_enc.imm = std::stoi(code_line[2]);
            break;

        case ISA::Encoding::Opcode::INC:
        case ISA::Encoding::Opcode::DEC:
            cpu.pipeline.id_enc.rd = RegFile::reg_index(code_line[1]);
            break;

        case ISA::Encoding::Opcode::JUMP:
            cpu.pipeline.id_enc.imm = std::stoi(code_line[1]);
            break;

        case ISA::Encoding::Opcode::BEQ:
        case ISA::Encoding::Opcode::BNE:
            cpu.pipeline.id_enc.rs = RegFile::reg_index(code_line[1]);
            cpu.pipeline.id_enc.rt = RegFile::reg_index(code_line[2]);
            cpu.pipeline.id_enc.imm = std::stoi(code_line[3]);
            break;

        case ISA::Encoding::Opcode::BGEZ:
        case ISA::Encoding::Opcode::BGTZ:
        case ISA::Encoding::Opcode::BLEZ:
        case ISA::Encoding::Opcode::BLTZ:
            cpu.pipeline.id_enc.rs = RegFile::reg_index(code_line[1]);
            cpu.pipeline.id_enc.imm = std::stoi(code_line[2]);
            break;

        case ISA::Encoding::Opcode::LOAD:
            cpu.pipeline.id_enc.rd = RegFile::reg_index(code_line[1]);
            break;

        case ISA::Encoding::Opcode::STORE:
            cpu.pipeline.id_enc.rs = RegFile::reg_index(code_line[2]);
            break;

        case ISA::Encoding::Opcode::HALT:
            break;
    }
}

void Pipeline::execute()
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    if (cpu.pipeline.ex_enc.opcode == ISA::Encoding::Opcode::NOP) return;

    ALUResult alu_res;
    int32_t rd_data = cpu.rf.reg(cpu.pipeline.ex_enc.rd);
    int32_t rs_data = cpu.rf.reg(cpu.pipeline.ex_enc.rs);
    int32_t rt_data = cpu.rf.reg(cpu.pipeline.ex_enc.rt);

    switch (cpu.pipeline.ex_enc.opcode) {
        case ISA::Encoding::Opcode::NOP:
            break;

        case ISA::Encoding::Opcode::ADD:
            alu_res.as_num = HW::CPU::ALU::add(rs_data, rt_data);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::SUB:
            alu_res.as_num = HW::CPU::ALU::sub(rs_data, rt_data);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::MUL:
            alu_res.as_num = HW::CPU::ALU::mul(rs_data, rt_data);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::DIV:
            alu_res.as_num = HW::CPU::ALU::div(rs_data, rt_data);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::LI:
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, cpu.pipeline.ex_enc.imm);
            break;

        case ISA::Encoding::Opcode::INC:
            alu_res.as_num = HW::CPU::ALU::add(rd_data, 1);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::DEC:
            alu_res.as_num = HW::CPU::ALU::sub(rd_data, 1);
            cpu.rf.reg(cpu.pipeline.ex_enc.rd, alu_res.as_num);
            break;

        case ISA::Encoding::Opcode::JUMP:
            cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            break;

        case ISA::Encoding::Opcode::BEQ:
            alu_res.as_bool = HW::CPU::ALU::eq(rs_data, rt_data);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BNE:
            alu_res.as_bool = HW::CPU::ALU::ne(rs_data, rt_data);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BGEZ:
            alu_res.as_bool = HW::CPU::ALU::ge(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BGTZ:
            alu_res.as_bool = HW::CPU::ALU::gt(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BLEZ:
            alu_res.as_bool = HW::CPU::ALU::le(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
            }
            break;

        case ISA::Encoding::Opcode::BLTZ:
            alu_res.as_bool = HW::CPU::ALU::lt(rs_data, 0);
            if (alu_res.as_bool == true) {
                cpu.pc = HW::CPU::ALU::sub(cpu.pipeline.ex_enc.imm, 1);
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
    HW::CPU::CPUState& cpu = proc_->cpu_state;
    auto code_line = cpu.pipeline.mem_enc.code_line.get();
    HW::RAM::DataSpace& mem = proc_->mem;
    HW::RAM::DataSpace::Variable var;

    if (cpu.pipeline.mem_enc.opcode == ISA::Encoding::Opcode::LOAD) {
        var.name = code_line[2];
        var.data = mem.read(var.name);

        cpu.rf.reg(cpu.pipeline.mem_enc.rd, var.data);
    } else if (cpu.pipeline.mem_enc.opcode == ISA::Encoding::Opcode::STORE) {
        var.name = code_line[1];
        var.data = cpu.rf.reg(cpu.pipeline.mem_enc.rs);

        mem.write(var);
    }
}

void Pipeline::write_back()
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    if (cpu.pipeline.wb_enc.opcode == ISA::Encoding::Opcode::HALT) {
        proc_->set_state(OS::PCB::State::TERMINATED);
    }
}

void Pipeline::update_stages()
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    cpu.pipeline.wb_enc  = std::move(cpu.pipeline.mem_enc);
    cpu.pipeline.mem_enc = std::move(cpu.pipeline.ex_enc);
    cpu.pipeline.ex_enc  = std::move(cpu.pipeline.id_enc);
    cpu.pipeline.id_enc  = std::move(cpu.pipeline.if_enc);
    cpu.pipeline.if_enc.code_line  = ISA::Code::NOP_LINE;
}

void Pipeline::set_proc(std::shared_ptr<OS::PCB> proc)
{
    proc_ = proc;
}

void Pipeline::print_log(std::ofstream& log) const
{
    HW::CPU::CPUState& cpu = proc_->cpu_state;

    log << "PIPELINE\n";

    auto write_instr = [&log](const std::string& label, const HW::ISA::Encoding& instr)
    {
        log << label;
        for (const auto& word : instr.code_line.get()) {
            log << '\t' << word;
        }
        log << "\n";
    };

    write_instr("IF:", cpu.pipeline.if_enc);
    write_instr("ID:", cpu.pipeline.id_enc);
    write_instr("EX:", cpu.pipeline.ex_enc);
    write_instr("MEM:", cpu.pipeline.mem_enc);
    write_instr("WB:", cpu.pipeline.wb_enc);
}

} // namespace HW::CPU
