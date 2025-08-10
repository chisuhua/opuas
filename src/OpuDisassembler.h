// opuas/src/OpuDisassembler.h

#ifndef OPU_DISASSEMBLER_H
#define OPU_DISASSEMBLER_H

#include <string>

class OpuDisassembler {
private:
    std::string inputFile;
    std::string outputFile;

public:
    OpuDisassembler(const std::string& input, const std::string& output);
    bool disassemble(); // Main disassembly function
};

#endif // OPU_DISASSEMBLER_H
