// opuas/src/elf/ElfObjectWriter.cpp
#include "ElfObjectWriter.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> // For memcpy

// --- Simplified ELF Structures (Placeholder) ---
// In a real implementation, you'd use a library like libelf or write detailed structs.
// Or define them here based on ELF specification.
struct Elf64_Ehdr {
    unsigned char e_ident[16]; // ELF identification
    uint16_t e_type;          // Object file type
    uint16_t e_machine;       // Architecture
    uint32_t e_version;       // Object file version
    uint64_t e_entry;         // Entry point virtual address
    uint64_t e_phoff;         // Program header table file offset
    uint64_t e_shoff;         // Section header table file offset
    uint32_t e_flags;         // Processor-specific flags
    uint16_t e_ehsize;        // ELF header size in bytes
    uint16_t e_phentsize;     // Program header table entry size
    uint16_t e_phnum;         // Program header table entry count
    uint16_t e_shentsize;     // Section header table entry size
    uint16_t e_shnum;         // Section header table entry count
    uint16_t e_shstrndx;      // Section header string table index
};

struct Elf64_Shdr {
    uint32_t sh_name;      // Section name (string tbl index)
    uint32_t sh_type;      // Section type
    uint64_t sh_flags;     // Section flags
    uint64_t sh_addr;      // Section virtual addr at execution
    uint64_t sh_offset;    // Section file offset
    uint64_t sh_size;      // Section size in bytes
    uint32_t sh_link;      // Link to another section
    uint32_t sh_info;      // Additional section information
    uint64_t sh_addralign; // Section alignment
    uint64_t sh_entsize;   // Entry size if section holds table
};

// ELF constants (simplified)
#define ET_REL 1
#define EM_OPU 0xFEED // Placeholder machine type for OPU
#define EV_CURRENT 1
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHF_ALLOC 2
#define SHF_EXECINSTR 4

namespace opuas {
namespace elf {

ElfObjectWriter::ElfObjectWriter(const std::string& filename) : outputFile(filename, std::ios::binary) {
    if (!outputFile.is_open()) {
        throw std::runtime_error("ElfObjectWriter: Could not open output file " + filename);
    }
}

bool ElfObjectWriter::write(const std::map<std::string, std::vector<uint32_t>>& codeSegments,
                            const std::map<std::string, int>& metadata) {
    if (!outputFile.is_open()) {
        std::cerr << "ElfObjectWriter Error: Output file is not open.\n";
        return false;
    }

    std::cout << "ElfObjectWriter Info: Writing ELF object file...\n";

    // --- Placeholder ELF Writing Logic ---
    // A real implementation is complex. This is a very simplified sketch.

    // 1. Prepare ELF Header
    Elf64_Ehdr ehdr = {}; // Zero-initialize
    // Set magic number
    memcpy(ehdr.e_ident, "\x7f""ELF", 4);
    ehdr.e_ident[4] = 2; // ELFCLASS64
    ehdr.e_ident[5] = 1; // ELFDATA2LSB
    ehdr.e_ident[6] = 1; // EV_CURRENT
    // ehdr.e_ident[7..15] = 0; // OS/ABI, padding

    ehdr.e_type = ET_REL; // Relocatable file
    ehdr.e_machine = EM_OPU; // Our custom machine type
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = 0; // No entry point for .o
    ehdr.e_phoff = 0; // No program headers in .o
    ehdr.e_shoff = 0; // Will be set later
    ehdr.e_flags = 0; // Processor-specific flags (could hold arch info)
    ehdr.e_ehsize = sizeof(Elf64_Ehdr);
    ehdr.e_phentsize = 0; // No program headers
    ehdr.e_phnum = 0;
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
    ehdr.e_shnum = 0; // Will be set later
    ehdr.e_shstrndx = 0; // Will be set later

    // 2. Prepare Section Headers and Data
    // This involves creating sections for code, metadata, symbol table, string table, etc.
    // For simplicity, let's just write one dummy .text section.
    std::vector<char> textData;
    if (!codeSegments.empty()) {
        // Take the first segment as dummy .text
        auto it = codeSegments.begin();
        const std::vector<uint32_t>& segmentWords = it->second;
        // Convert uint32_t words to bytes
        textData.resize(segmentWords.size() * sizeof(uint32_t));
        memcpy(textData.data(), segmentWords.data(), textData.size());
    }

    Elf64_Shdr textShdr = {};
    textShdr.sh_name = 1; // Index into shstrtab (placeholder)
    textShdr.sh_type = SHT_PROGBITS;
    textShdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    textShdr.sh_addr = 0; // Relocatable, so 0
    textShdr.sh_offset = 0; // Will be set later
    textShdr.sh_size = textData.size();
    textShdr.sh_link = 0;
    textShdr.sh_info = 0;
    textShdr.sh_addralign = 16; // Typical alignment
    textShdr.sh_entsize = 0;

    // 3. Prepare String Table (.shstrtab)
    std::string shstrtab = "\0.text\0.shstrtab"; // Null-terminated strings
    std::vector<char> shstrtabData(shstrtab.begin(), shstrtab.end());
    shstrtabData.push_back('\0'); // Ensure null termination at the end

    Elf64_Shdr shstrtabShdr = {};
    shstrtabShdr.sh_name = 8; // Index of ".shstrtab" in shstrtab itself
    shstrtabShdr.sh_type = SHT_STRTAB;
    shstrtabShdr.sh_flags = 0;
    shstrtabShdr.sh_addr = 0;
    shstrtabShdr.sh_offset = 0; // Will be set later
    shstrtabShdr.sh_size = shstrtabData.size();
    shstrtabShdr.sh_link = 0;
    shstrtabShdr.sh_info = 0;
    shstrtabShdr.sh_addralign = 1;
    shstrtabShdr.sh_entsize = 0;

    // 4. Layout ELF in memory/file
    // Calculate offsets
    size_t ehdrSize = sizeof(ehdr);
    size_t shdrSize = sizeof(Elf64_Shdr);
    size_t numShdrs = 3; // NULL, .text, .shstrtab (placeholder)
    size_t shdrsOffset = ehdrSize; // Section headers come right after ELF header
    size_t textOffset = shdrsOffset + numShdrs * shdrSize;
    size_t shstrtabOffset = textOffset + textData.size();

    // Update offsets in headers
    textShdr.sh_offset = textOffset;
    shstrtabShdr.sh_offset = shstrtabOffset;
    ehdr.e_shoff = shdrsOffset;
    ehdr.e_shnum = numShdrs;
    ehdr.e_shstrndx = 2; // Index of .shstrtab section header

    // 5. Write ELF File
    try {
        // Write ELF Header
        outputFile.write(reinterpret_cast<const char*>(&ehdr), ehdrSize);

        // Write Section Headers (placeholder order: NULL, .text, .shstrtab)
        Elf64_Shdr nullShdr = {}; // NULL section header
        outputFile.write(reinterpret_cast<const char*>(&nullShdr), shdrSize);
        outputFile.write(reinterpret_cast<const char*>(&textShdr), shdrSize);
        outputFile.write(reinterpret_cast<const char*>(&shstrtabShdr), shdrSize);

        // Write Section Data
        if (!textData.empty()) {
            outputFile.write(textData.data(), textData.size());
        }
        if (!shstrtabData.empty()) {
            outputFile.write(shstrtabData.data(), shstrtabData.size());
        }

        outputFile.flush();
        std::cout << "ElfObjectWriter Info: ELF object file written successfully (placeholder logic).\n";
        return true;

    } catch (const std::exception& e) {
        std::cerr << "ElfObjectWriter Error: Exception occurred while writing file: " << e.what() << std::endl;
        return false;
    }

    // --- End of Placeholder Logic ---

    // A full implementation would:
    // - Handle multiple code segments.
    // - Generate and write symbol table (.symtab) and associated string table (.strtab).
    // - Encode metadata into custom sections (e.g., .opu.kernels, .opu.version).
    // - Handle relocations if needed.
    // - Ensure correct alignment and padding.
}

ElfObjectWriter::~ElfObjectWriter() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

} // namespace elf
} // namespace opuas
