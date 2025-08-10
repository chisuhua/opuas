// opuas/src/OpuAssembler.h

#ifndef OPU_ASSEMBLER_H
#define OPU_ASSEMBLER_H

#include <string>
#include <fstream>

class OpuAssembler {
private:
    std::string inputFile;
    std::string outputFile;

public:
    OpuAssembler(const std::string& input, const std::string& output);
    bool assemble(); // Main assembly function
};

#endif // OPU_ASSEMBLER_H
