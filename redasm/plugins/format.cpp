#include "format.h"

namespace REDasm {

FormatPlugin::FormatPlugin(): Plugin()
{

}

const SegmentList &FormatPlugin::segments() const
{
    return _segments;
}

SymbolTable *FormatPlugin::symbols()
{
    return &this->_symbol;
}

Segment *FormatPlugin::segment(address_t address)
{
    for(auto it = this->_segments.begin(); it != this->_segments.end(); it++)
    {
        if(it->contains(address))
            return &(*it);
    }

    return NULL;
}

Segment *FormatPlugin::segmentAt(u64 index)
{
    return &this->_segments[index];
}

Segment *FormatPlugin::segmentByName(const std::string &name)
{
    for(auto it = this->_segments.begin(); it != this->_segments.end(); it++)
    {
        Segment& segment = *it;

        if(segment.name == name)
            return &segment;
    }

    return NULL;
}

Segment *FormatPlugin::entryPointSegment()
{
    SymbolPtr symbol = this->_symbol.entryPoint();

    if(symbol)
        return this->segment(symbol->address);

    return NULL;
}

const SignatureFiles &FormatPlugin::signatures() const
{
    return this->_signatures;
}

u64 FormatPlugin::addressWidth() const
{
    return this->bits() / 8;
}

offset_t FormatPlugin::offset(address_t address) const
{
    for(auto it = this->_segments.begin(); it != this->_segments.end(); it++)
    {
        const Segment& segment = *it;

        if(segment.contains(address))
            return (address - segment.address) + segment.offset;
    }

    return address;
}

Analyzer* FormatPlugin::createAnalyzer(DisassemblerAPI *disassembler, const SignatureFiles& signatures) const
{
    return new Analyzer(disassembler, signatures);
}

u32 FormatPlugin::flags() const
{
    return FormatFlags::None;
}

endianness_t FormatPlugin::endianness() const
{
    return Endianness::LittleEndian; // Use LE by default
}

bool FormatPlugin::isBinary() const
{
    return this->flags() & FormatFlags::Binary;
}

bool FormatPlugin::load(u8 *format)
{
    RE_UNUSED(format);

    std::sort(this->_segments.begin(), this->_segments.end(), [](const Segment& s1, const Segment& s2) -> bool {
        return s1.address < s2.address;
    });

    return false;
}

void FormatPlugin::addSignature(const std::string &signaturefile)
{
    this->_signatures.push_back(signaturefile);
}

void FormatPlugin::defineSegment(const std::string &name, offset_t offset, address_t address, u64 size, u32 flags)
{
    this->_segments.push_back(Segment(name, offset, address, size, flags));
}

void FormatPlugin::defineSymbol(address_t address, const std::string &name, u32 type, u32 extratype)
{
    this->_symbol.create(address, name, type | SymbolTypes::Locked, extratype);
}

void FormatPlugin::defineFunction(address_t address, const std::string& name, u32 extratype)
{
    this->defineSymbol(address, name, SymbolTypes::Function, extratype);
}

void FormatPlugin::defineEntryPoint(address_t address, u32 extratype)
{
    this->defineSymbol(address, ENTRYPOINT_FUNCTION, SymbolTypes::EntryPoint, extratype);
}

}
