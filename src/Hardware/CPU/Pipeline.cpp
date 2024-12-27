#include "Hardware/CPU/Pipeline.hpp"

#include "Hardware/CPU/Instruction.hpp"
#include "Hardware/CPU/RegFile.hpp"

namespace Hardware::CPU {

void Pipeline::instr_fetch(uint32_t& pc, const OS::Process& proc)
{
    instr_if.text = proc.get_instr(pc++);
    instr_if.pid = proc.pid();
}

void Pipeline::instr_decode(OS::Process& proc)
{
    instr_id.opcode = Instruction::get_opcode((*instr_id.text)[0]);

    switch (instr_id.opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
        case Instruction::Opcode::SUB:
        case Instruction::Opcode::MUL:
        case Instruction::Opcode::DIV:
            instr_id.rd = RegFile::reg_index((*instr_id.text)[1]);
            instr_id.rs = RegFile::reg_index((*instr_id.text)[2]);
            instr_id.rt = RegFile::reg_index((*instr_id.text)[3]);
            break;

        case Instruction::Opcode::LI:
            instr_id.rd = RegFile::reg_index((*instr_id.text)[1]);
            instr_id.imm = std::stoi((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::INC:
        case Instruction::Opcode::DEC:
            instr_id.rd = RegFile::reg_index((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::JUMP:
            instr_id.imm = std::stoi((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::BEQ:
        case Instruction::Opcode::BNE:
            instr_id.rs = RegFile::reg_index((*instr_id.text)[1]);
            instr_id.rt = RegFile::reg_index((*instr_id.text)[2]);
            instr_id.imm = std::stoi((*instr_id.text)[3]);
            break;

        case Instruction::Opcode::BGEZ:
        case Instruction::Opcode::BGTZ:
        case Instruction::Opcode::BLEZ:
        case Instruction::Opcode::BLTZ:
            instr_id.rs = RegFile::reg_index((*instr_id.text)[1]);
            instr_id.imm = std::stoi((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::LOAD:
            instr_id.rd = RegFile::reg_index((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::STORE:
            instr_id.rs = RegFile::reg_index((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::HALT:
            proc.terminate();
            break;
    }
}

void Pipeline::execute(RegFile& rf, uint32_t& pc)
{
    if (instr_ex.opcode == Instruction::Opcode::NOP) return;

    ALUResult alu_res;
    int32_t rd_data, rs_data, rt_data;

    rd_data = rf.reg(instr_ex.rd);
    rs_data = rf.reg(instr_ex.rs);
    rt_data = rf.reg(instr_ex.rt);

    switch (instr_ex.opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
            alu_res.number = alu_.add(rs_data, rt_data);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::SUB:
            alu_res.number = alu_.sub(rs_data, rt_data);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::MUL:
            alu_res.number = alu_.mul(rs_data, rt_data);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::DIV:
            alu_res.number = alu_.div(rs_data, rt_data);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::LI:
            rf.reg(instr_ex.rd, instr_ex.imm);
            break;

        case Instruction::Opcode::INC:
            alu_res.number = alu_.add(rd_data, 1);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::DEC:
            alu_res.number = alu_.sub(rd_data, 1);
            rf.reg(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::JUMP:
            pc = alu_.sub(instr_ex.imm, 1);
            break;

        case Instruction::Opcode::BEQ:
            alu_res.boolean = alu_.eq(rs_data, rt_data);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BNE:
            alu_res.boolean = alu_.ne(rs_data, rt_data);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BGEZ:
            alu_res.boolean = alu_.ge(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BGTZ:
            alu_res.boolean = alu_.gt(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BLEZ:
            alu_res.boolean = alu_.le(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BLTZ:
            alu_res.boolean = alu_.lt(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu_.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::LOAD:
            break;

        case Instruction::Opcode::STORE:
            break;

        case Instruction::Opcode::HALT:
            break;
    }
}

void Pipeline::mem_access(uint32_t pid, RegFile& rf, RAM& mem)
{
    RAM::Variable var;

    if (instr_mem.opcode == Instruction::Opcode::LOAD) {
        var.name = (*instr_mem.text)[2];
        var.data = mem.read_data(pid, var.name);

        rf.reg(instr_mem.rd, var.data);

    } else if (instr_mem.opcode == Instruction::Opcode::STORE) {
        var.name = (*instr_wb.text)[1];
        var.data = rf.reg(instr_mem.rs);

        mem.write_data(pid, var);
    }
}

void Pipeline::write_back()
{
}

void Pipeline::update()
{
    instr_wb  = instr_mem;
    instr_mem = instr_ex;
    instr_ex  = instr_id;
    instr_id  = instr_if;
    instr_if.text  = &Instruction::NOP_INSTR;
}

void Pipeline::print_log(std::ofstream& log)
{
    log << "PIPELINE\n";

    auto write_instr = [&log](const std::string& label, const auto& instr) {
        log << label;
        for (const auto& word : *instr.text) {
            log << '\t' << word;
        }
        log << "\n";
    };

    write_instr("IF:", instr_if);
    write_instr("ID:", instr_id);
    write_instr("EX:", instr_ex);
    write_instr("MEM:", instr_mem);
    write_instr("WB:", instr_wb);
}

} // namespace Hardware::CPU
