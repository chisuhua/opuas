// opuas/src/Validator.h

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

// Function to validate COASM syntax using coasm_infra's generated ANTLR parser
bool validateCoasmSyntax(const std::string& coasmCode);

#endif // VALIDATOR_H
