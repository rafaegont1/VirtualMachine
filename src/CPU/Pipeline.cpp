#include "CPU/Pipeline.hpp"

#include "CPU/Instruction.hpp"
#include "CPU/RegFile.hpp"

namespace CPU {

const std::vector<std::string> Pipeline::NOP_INSTR = {"nop"};

Pipeline::Pipeline(uint32_t & clk, uint32_t & pc, RegFile & rf, RAM & mem, ALU & alu) :
    clk(clk), pc(pc), rf(rf), mem(mem), alu(alu)
{
}

void Pipeline::run(Process * proc, std::ofstream & log)
{
    uint32_t pid = proc->get_pid();
    bool quantum_expired = false;

    do {
        log << "CLOCK: " << ++clk << '\n';

        instr_fetch(proc);
        instr_decode(proc);
        execute();
        mem_access(pid);
        write_back();

        rf.print_log(log);
        print_log(log);
        mem.print_log(pid, log);
        log << "==============================================================\n";

        update();

        quantum_expired = (proc->get_cpu_time() > proc->get_quantum());

    } while (proc->get_state() != Process::State::TERMINATED
        && not quantum_expired);
}

void Pipeline::instr_fetch(Process * proc)
{
    instr_if.text = proc->get_instr(pc++);
    instr_if.pid = proc->get_pid();
}

void Pipeline::instr_decode(Process * proc)
{
    const std::string & opcode_text = (*instr_id.text)[0];
    instr_id.opcode = Instruction::get_opcode(opcode_text);

    switch (instr_id.opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
        case Instruction::Opcode::SUB:
        case Instruction::Opcode::MUL:
        case Instruction::Opcode::DIV:
            instr_id.rd = RegFile::get_reg_num((*instr_id.text)[1]);
            instr_id.rs = RegFile::get_reg_num((*instr_id.text)[2]);
            instr_id.rt = RegFile::get_reg_num((*instr_id.text)[3]);
            break;

        case Instruction::Opcode::LI:
            instr_id.rd = RegFile::get_reg_num((*instr_id.text)[1]);
            instr_id.imm = std::stoi((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::INC:
        case Instruction::Opcode::DEC:
            instr_id.rd = RegFile::get_reg_num((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::JUMP:
            instr_id.imm = std::stoi((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::BEQ:
        case Instruction::Opcode::BNE:
            instr_id.rs = RegFile::get_reg_num((*instr_id.text)[1]);
            instr_id.rt = RegFile::get_reg_num((*instr_id.text)[2]);
            instr_id.imm = std::stoi((*instr_id.text)[3]);
            break;

        case Instruction::Opcode::BGEZ:
        case Instruction::Opcode::BGTZ:
        case Instruction::Opcode::BLEZ:
        case Instruction::Opcode::BLTZ:
            instr_id.rs = RegFile::get_reg_num((*instr_id.text)[1]);
            instr_id.imm = std::stoi((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::LOAD:
            instr_id.rd = RegFile::get_reg_num((*instr_id.text)[1]);
            break;

        case Instruction::Opcode::STORE:
            instr_id.rs = RegFile::get_reg_num((*instr_id.text)[2]);
            break;

        case Instruction::Opcode::HALT:
            proc->finish();
            break;
    }
}

void Pipeline::execute()
{
    ALUResult alu_res;
    int32_t rd_data, rs_data, rt_data;

    rd_data = rf.get_data(instr_ex.rd);
    rs_data = rf.get_data(instr_ex.rs);
    rt_data = rf.get_data(instr_ex.rt);

    switch (instr_ex.opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
            alu_res.number = alu.add(rs_data, rt_data);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::SUB:
            alu_res.number = alu.sub(rs_data, rt_data);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::MUL:
            alu_res.number = alu.mul(rs_data, rt_data);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::DIV:
            alu_res.number = alu.div(rs_data, rt_data);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::LI:
            rf.set_data(instr_ex.rd, instr_ex.imm);
            break;

        case Instruction::Opcode::INC:
            alu_res.number = alu.add(rd_data, 1);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::DEC:
            alu_res.number = alu.sub(rd_data, 1);
            rf.set_data(instr_ex.rd, alu_res.number);
            break;

        case Instruction::Opcode::JUMP:
            pc = alu.sub(instr_ex.imm, 1);
            break;

        case Instruction::Opcode::BEQ:
            alu_res.boolean = alu.eq(rs_data, rt_data);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BNE:
            alu_res.boolean = alu.ne(rs_data, rt_data);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BGEZ:
            alu_res.boolean = alu.ge(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BGTZ:
            alu_res.boolean = alu.gt(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BLEZ:
            alu_res.boolean = alu.le(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
            }
            break;

        case Instruction::Opcode::BLTZ:
            alu_res.boolean = alu.lt(rs_data, 0);
            if (alu_res.boolean == true) {
                pc = alu.sub(instr_ex.imm, 1);
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

void Pipeline::mem_access(uint32_t pid)
{
    RAM::Variable var;

    if (instr_mem.opcode == Instruction::Opcode::LOAD) {
        var.name = (*instr_mem.text)[2];
        var.data = mem.read_data(pid, var.name);

        rf.set_data(instr_mem.rd, var.data);

    } else if (instr_mem.opcode == Instruction::Opcode::STORE) {
        var.name = (*instr_wb.text)[1];
        var.data = rf.get_data(instr_mem.rs);

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

void Pipeline::print_log(std::ofstream & log)
{
    log << "PIPELINE\n";

    auto write_instr = [&log](const std::string & label, const auto & instr) {
        log << label;
        for (const auto & word : *instr.text) {
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

} // namespace CPU
