// opuas/src/OpuAssembler.cpp

#include "OpuAssembler.h"
#include "Validator.h" // Our validator using coasm_infra's parser
#include <iostream>
#include <fstream>
#include <sstream>

OpuAssembler::OpuAssembler(const std::string& input, const std::string& output)
    : inputFile(input), outputFile(output) {}

bool OpuAssembler::assemble() {
    std::cout << "Loading COASM file '" << inputFile << "' for assembly...\n";

    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file " << inputFile << std::endl;
        return false;
    }

    // Read the entire file content into a string
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string coasmCode = buffer.str();
    inFile.close();

    if (coasmCode.empty()) {
        std::cerr << "Error: Input COASM file is empty.\n";
        return false;
    }

    std::cout << "Validating COASM syntax using coasm_infra parser...\n";
    // --- Core Integration Point ---
    // Use the Validator class which wraps coasm_infra's generated ANTLR parser
    if (!validateCoasmSyntax(coasmCode)) {
        std::cerr << "Validation Error: COASM syntax is invalid according to coasm_infra parser.\n";
        return false; // Fail assembly if syntax is wrong
    } else {
        std::cout << "Validation Info: COASM syntax is valid.\n";
    }

    // --- Placeholder for subsequent steps ---
    // 1. Parse the COASM code into an AST (using our own Parser or coasm_infra's visitor/listener)
    // 2. Perform semantic analysis (resolve symbols, check types, etc.)
    // 3. Allocate registers (algorithms/RegisterAllocator)
    // 4. Analyze dependencies/stalls (algorithms/StallSetter)
    // 5. Generate machine code (CodeGenerator)
    // 6. Write ELF object file (elf/ElfObjectWriter)
    std::cout << "Info: Assembly process (validation only) completed successfully.\n";
    std::cout << "Info: Full assembly pipeline (codegen, ELF writing) is not yet implemented.\n";

    // For testing purposes, just write the validated code to the output file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFile << std::endl;
        return false;
    }
    outFile << coasmCode; // Echo input to output for now
    outFile.close();

    return true; // Indicate success (validation passed)
}
