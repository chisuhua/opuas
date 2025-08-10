// opuas/src/main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "OpuAssembler.h"
#include "OpuDisassembler.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <mode> <input_file> [output_file]\n";
        std::cerr << "Modes:\n";
        std::cerr << "  assemble    - Assemble .coasm to .o/.cubin\n";
        std::cerr << "  disassemble - Disassemble .o/.cubin to .coasm\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file = (argc > 3) ? argv[3] : "output.bin"; // Default output

    std::ifstream stream(input_file);
    if (!stream.good()) {
        std::cerr << "Error: Could not open input file " << input_file << std::endl;
        return 1;
    }

    if (mode == "assemble") {
        std::cout << "Assembling '" << input_file << "'...\n";
        // For now, just validate using coasm_infra's parser
        OpuAssembler assembler(input_file, output_file);
        if (assembler.assemble()) {
            std::cout << "Assembly successful. Output written to '" << output_file << "'.\n";
        } else {
            std::cerr << "Assembly failed.\n";
            return 1;
        }
    } else if (mode == "disassemble") {
        std::cout << "Disassembling '" << input_file << "'...\n";
        OpuDisassembler disassembler(input_file, output_file);
        if (disassembler.disassemble()) {
            std::cout << "Disassembly successful. Output written to '" << output_file << "'.\n";
        } else {
            std::cerr << "Disassembly failed.\n";
            return 1;
        }
    } else {
        std::cerr << "Error: Unknown mode '" << mode << "'. Use 'assemble' or 'disassemble'.\n";
        return 1;
    }

    stream.close();
    return 0;
}
