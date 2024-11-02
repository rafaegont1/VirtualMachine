#include "CPU.hpp"

#include <iostream>
#include "Instruction.hpp"
#include "RegisterFile.hpp"

const std::vector<std::string> CPU::NOP_INSTR = {"nop"};

CPU::CPU(std::ofstream *log, Memory *mem) :
    rf(log), mem(mem), log(log) {}

void CPU::run(Process& p)
{
    rf.update_regs(p.read_regs());

    do {
        *log << "CLOCK: " << ++clk << '\n';

        instr_fetch(p);
        instr_decode();
        execute(p);
        mem_access();
        write_back();

        rf.print();
        print_pipeline();
        mem->print_data();
        *log <<
            "==============================================================\n";

        update();

    } while (!finished());

    p.write_regs(rf.get_regs_data());
}

bool CPU::stall_on_branch()
{
    Instruction::Opcode id_opcode = Instruction::get_opcode((*instr_id)[0]);
    Instruction::Opcode ex_opcode = Instruction::get_opcode((*instr_ex)[0]);

    return Instruction::is_branch(id_opcode)
        || Instruction::is_branch(ex_opcode);
}

void CPU::instr_fetch(Process& p)
{
    if (p.pc < p.code.size() && !stall_on_branch()) {
        instr_if = &p.code[p.pc++];
    } else {
        instr_if = &NOP_INSTR;
    }
}

void CPU::instr_decode()
{
    Instruction::Opcode opcode = Instruction::get_opcode((*instr_id)[0]);

    switch (opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
        case Instruction::Opcode::SUB:
        case Instruction::Opcode::MULT:
        case Instruction::Opcode::DIV:
            this->id_rd = RegisterFile::get_enum((*instr_id)[1]);
            this->id_rs = RegisterFile::get_enum((*instr_id)[2]);
            this->id_rt = RegisterFile::get_enum((*instr_id)[3]);
            break;

        case Instruction::Opcode::LI:
            this->id_rd = RegisterFile::get_enum((*instr_id)[1]);
            this->id_imm = std::stoi((*instr_id)[2]);
            break;

        case Instruction::Opcode::INC:
        case Instruction::Opcode::DEC:
            this->id_rd = RegisterFile::get_enum((*instr_id)[1]);
            break;

        case Instruction::Opcode::JUMP:
            this->id_imm = std::stoi((*instr_id)[1]);
            break;

        case Instruction::Opcode::BEQ:
        case Instruction::Opcode::BNE:
            this->id_rs = RegisterFile::get_enum((*instr_id)[1]);
            this->id_rt = RegisterFile::get_enum((*instr_id)[2]);
            this->id_imm = std::stoi((*instr_id)[3]);
            break;

        case Instruction::Opcode::BGEZ:
        case Instruction::Opcode::BGTZ:
        case Instruction::Opcode::BLEZ:
        case Instruction::Opcode::BLTZ:
            this->id_rs = RegisterFile::get_enum((*instr_id)[1]);
            this->id_imm = std::stoi((*instr_id)[2]);
            break;

        case Instruction::Opcode::LOAD:
            this->id_rd = RegisterFile::get_enum((*instr_id)[1]);
            break;

        case Instruction::Opcode::STORE:
            this->id_rs = RegisterFile::get_enum((*instr_id)[2]);
            break;
    }
}

void CPU::execute(Process &p)
{
    Instruction::Opcode opcode = Instruction::get_opcode((*instr_ex)[0]);
    int32_t alu_res;

    switch (opcode) {
        case Instruction::Opcode::NOP:
            break;

        case Instruction::Opcode::ADD:
            alu_res = alu(rf.get_data(ex_rs), rf.get_data(ex_rt), '+');
            rf.set_data(ex_rd, alu_res);
            break;

        case Instruction::Opcode::SUB:
            alu_res = alu(rf.get_data(ex_rs), rf.get_data(ex_rt), '-');
            rf.set_data(ex_rd, alu_res);
            break;

        case Instruction::Opcode::MULT:
            alu_res = alu(rf.get_data(ex_rs), rf.get_data(ex_rt), '*');
            rf.set_data(ex_rd, alu_res);
            break;

        case Instruction::Opcode::DIV:
            alu_res = alu(rf.get_data(ex_rs), rf.get_data(ex_rt), '/');
            rf.set_data(ex_rd, alu_res);
            break;

        case Instruction::Opcode::LI:
            rf.set_data(ex_rd, ex_imm);
            break;

        case Instruction::Opcode::INC:
            rf.set_data(ex_rd, rf.get_data(ex_rd) + 1);
            break;

        case Instruction::Opcode::DEC:
            rf.set_data(ex_rd, rf.get_data(ex_rd) - 1);
            break;

        case Instruction::Opcode::JUMP:
            p.pc = this->ex_imm - 1;
            break;

        case Instruction::Opcode::BEQ:
            if (rf.get_data(ex_rs) == rf.get_data(ex_rt)) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::BNE:
            if (rf.get_data(ex_rs) != rf.get_data(ex_rt)) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::BGEZ:
            if (rf.get_data(ex_rs) >= 0) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::BGTZ:
            if (rf.get_data(ex_rs) > 0) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::BLEZ:
            if (rf.get_data(ex_rs) <= 0) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::BLTZ:
            if (rf.get_data(ex_rs) < 0) {
                p.pc = this->ex_imm - 1;
            }
            break;

        case Instruction::Opcode::LOAD:
            break;

        case Instruction::Opcode::STORE:
            break;
    }
}

void CPU::mem_access()
{
    Instruction::Opcode opcode = Instruction::get_opcode((*instr_mem)[0]);

    if (opcode == Instruction::Opcode::LOAD) {
        std::string var = (*instr_mem)[2];
        rf.set_data(id_rd, mem->read_data(var));
    }
}

void CPU::write_back()
{
    Instruction::Opcode opcode = Instruction::get_opcode((*instr_wb)[0]);

    if (opcode == Instruction::Opcode::STORE) {
        std::string var = (*instr_wb)[1];
        mem->write_data(var, rf.get_data(id_rs));
    }
}

int32_t CPU::alu(int32_t opnd1, int32_t opnd2, char op)
{
    int32_t res;

    switch (op) {
        case '+': res = opnd1 + opnd2; break;
        case '-': res = opnd1 - opnd2; break;
        case '*': res = opnd1 * opnd2; break;
        case '/': res = opnd1 / opnd2; break;
    }

    return res;
}

void CPU::update()
{
    this->instr_wb  = this->instr_mem;
    this->instr_mem = this->instr_ex;
    this->instr_ex  = this->instr_id;
    this->instr_id  = this->instr_if;
    this->instr_if  = &NOP_INSTR;

    this->ex_rd  = this->id_rd;
    this->ex_rs  = this->id_rs;
    this->ex_rt  = this->id_rt;
    this->ex_imm = this->id_imm;
}

void CPU::print_pipeline()
{
    *log << "PIPELINE\n";
    *log << "IF:";
    for (const auto& word : *instr_if) {
        *log << '\t' << word;
    }
    *log << "\n";

    *log << "ID:";
    for (const auto& word : *instr_id) {
        *log << '\t' << word;
    }
    *log << "\n";

    *log << "EX:";
    for (const auto& word : *instr_ex) {
        *log << '\t' << word;
    }
    *log << "\n";

    *log << "MEM:";
    for (const auto& word : *instr_mem) {
        *log << '\t' << word;
    }
    *log << "\n";

    *log << "WB:";
    for (const auto& word : *instr_wb) {
        *log << '\t' << word;
    }
    *log << "\n";
}

bool CPU::finished()
{
    return instr_if->front() == "nop" && instr_id->front()  == "nop"
        && instr_ex->front() == "nop" && instr_mem->front() == "nop"
        && instr_wb->front() == "nop";
}
