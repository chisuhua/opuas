// opuas/src/algorithms/RegisterAllocator.cpp
#include "RegisterAllocator.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm> // For std::find_if

namespace opuas {
namespace algorithms {

RegisterAllocator::RegisterAllocator(/* Parser* parser, CodeGenerator* codeGen */) {
    // Initialize with some free physical registers
    // This is highly simplified. A real allocator needs liveness analysis etc.
    for (int i = 0; i < 32; ++i) { // Assume 32 general-purpose %v registers
        freeVRegisters_.insert(i);
    }
    // Assume 16 predicate registers %p
    for (int i = 0; i < 16; ++i) {
        freePRegisters_.insert(i);
    }
    // Assume 8 double-word registers %vd (occupying pairs of %v)
    for (int i = 0; i < 8; i+=2) { // Even indices for %vd
        freeVdRegisters_.insert(i);
    }
}

bool RegisterAllocator::allocate(const std::vector<std::string>& ptxVirtualRegs) {
    std::cout << "RegisterAllocator Info: Starting register allocation (placeholder)...\n";

    // --- Placeholder Allocation Logic ---
    // A real allocator would analyze liveness, interference graphs, etc.
    // This just assigns the first available physical reg to each PTX virtual reg.

    allocationMap_.clear();

    for (const std::string& ptxReg : ptxVirtualRegs) {
        if (ptxReg.empty() || ptxReg[0] != '%') continue;

        std::string coasmReg;
        if (ptxReg[1] == 'r' || ptxReg[1] == 'f') {
            // Map to %v
            if (!freeVRegisters_.empty()) {
                int physReg = *freeVRegisters_.begin();
                freeVRegisters_.erase(freeVRegisters_.begin());
                coasmReg = "%v" + std::to_string(physReg);
            } else {
                std::cerr << "RegisterAllocator Error: Out of free %v registers for " << ptxReg << std::endl;
                return false;
            }
        } else if (ptxReg[1] == 'r' && ptxReg[2] == 'd') { // %rd
             // Map to %vd
             // Find an even-numbered free %vd reg
             auto it = std::find_if(freeVdRegisters_.begin(), freeVdRegisters_.end(),
                                    [](int r) { return r % 2 == 0; });
             if (it != freeVdRegisters_.end()) {
                 int physReg = *it;
                 freeVdRegisters_.erase(it);
                 // Mark the pair of %v registers as used
                 freeVRegisters_.erase(physReg);
                 freeVRegisters_.erase(physReg + 1);
                 coasmReg = "%vd" + std::to_string(physReg);
             } else {
                 std::cerr << "RegisterAllocator Error: Out of free %vd registers for " << ptxReg << std::endl;
                 return false;
             }
        } else if (ptxReg[1] == 'p') {
             // Map to %p
             if (!freePRegisters_.empty()) {
                 int physReg = *freePRegisters_.begin();
                 freePRegisters_.erase(freePRegisters_.begin());
                 coasmReg = "%p" + std::to_string(physReg);
             } else {
                 std::cerr << "RegisterAllocator Error: Out of free %p registers for " << ptxReg << std::endl;
                 return false;
             }
        } else {
            // Handle other register types (%tid.x, %ctaid.y, etc.) or pass through
            coasmReg = ptxReg; // For special regs, keep as is or map symbolically
            std::cout << "RegisterAllocator Info: Passing through special register " << ptxReg << std::endl;
        }

        if (!coasmReg.empty()) {
            allocationMap_[ptxReg] = coasmReg;
            std::cout << "  Allocated " << coasmReg << " for " << ptxReg << std::endl;
        }
    }

    std::cout << "RegisterAllocator Info: Register allocation completed (placeholder logic).\n";
    return true;
    // --- End of Placeholder Logic ---
}

const std::unordered_map<std::string, std::string>& RegisterAllocator::getAllocationMap() const {
    return allocationMap_;
}

// Methods to deallocate/free registers would also be needed in a full implementation.

} // namespace algorithms
} // namespace opuas
