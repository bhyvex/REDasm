#ifndef PRINTER_H
#define PRINTER_H

#include <memory>
#include <capstone.h>
#include "../../redasm.h"
#include "../../disassembler/types/symboltable.h"
#include "../../disassembler/disassemblerapi.h"

namespace REDasm {

class Printer
{
    public:
        typedef std::function<void(const Operand&, const std::string&, const std::string&)> OpCallback;
        typedef std::function<void(const SymbolPtr&, const std::string&)> SymbolCallback;
        typedef std::function<void(const std::string&, const std::string&, const std::string&)> HeaderCallback;
        typedef std::function<void(const std::string&)> LineCallback;

    public:
        Printer(DisassemblerAPI* disassembler, SymbolTable* symboltable);
        void symbols(const InstructionPtr& instruction, SymbolCallback symbolfunc);
        std::string symbol(const SymbolPtr& symbol) const;
        std::string out(const InstructionPtr& instruction) const;

    public:
        virtual void header(const SymbolPtr& symbol, HeaderCallback headerfunc);
        virtual void prologue(const SymbolPtr& symbol, LineCallback prologuefunc);
        virtual void symbol(const SymbolPtr& symbol, SymbolCallback symbolfunc) const;
        virtual void info(const InstructionPtr& instruction, LineCallback infofunc);
        virtual std::string out(const InstructionPtr& instruction, OpCallback opfunc) const;

    public: // Operand privitives
        virtual std::string reg(const RegisterOperand& regop) const;
        virtual std::string disp(const DisplacementOperand& dispop) const;
        virtual std::string loc(const Operand& operand) const;
        virtual std::string imm(const Operand& operand) const;

    protected:
        DisassemblerAPI* _disassembler;
        SymbolTable* _symboltable;
};

class CapstonePrinter: public Printer
{
    public:
        CapstonePrinter(csh cshandle, DisassemblerAPI* disassembler, SymbolTable* symboltable);

    protected:
        virtual std::string reg(const RegisterOperand &regop) const;

    private:
        csh _cshandle;
};

typedef std::shared_ptr<Printer> PrinterPtr;

}

#endif // PRINTER_H
