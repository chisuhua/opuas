// opuas/src/utils.cpp
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem> // C++17

namespace opuas {
namespace utils {

// --- File I/O Utilities ---

bool readFileToString(const std::string& filename, std::string& content) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Utils Error: Could not open file '" << filename << "' for reading." << std::endl;
        return false;
    }

    try {
        // Efficiently read entire file into string
        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Utils Error: Exception occurred while reading file '" << filename << "': " << e.what() << std::endl;
        file.close();
        return false;
    }
}

bool writeStringToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Utils Error: Could not open file '" << filename << "' for writing." << std::endl;
        return false;
    }

    try {
        file << content;
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Utils Error: Exception occurred while writing file '" << filename << "': " << e.what() << std::endl;
        file.close();
        return false;
    }
}

// --- String Manipulation Utilities ---

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return ""; // No non-whitespace characters
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

std::string toUpper(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

// --- Path and Directory Utilities (C++17) ---

bool createDirectories(const std::string& path) {
    try {
        std::filesystem::create_directories(path);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Utils Error: Failed to create directories '" << path << "': " << e.what() << std::endl;
        return false;
    }
}

std::string getParentPath(const std::string& path) {
    try {
        return std::filesystem::path(path).parent_path().string();
    } catch (const std::exception& e) {
        std::cerr << "Utils Error: Failed to get parent path for '" << path << "': " << e.what() << std::endl;
        return "";
    }
}

std::string getFilename(const std::string& path) {
    try {
        return std::filesystem::path(path).filename().string();
    } catch (const std::exception& e) {
        std::cerr << "Utils Error: Failed to get filename for '" << path << "': " << e.what() << std::endl;
        return "";
    }
}

bool pathExists(const std::string& path) {
    try {
        return std::filesystem::exists(path);
    } catch (const std::exception& e) {
        // std::cerr << "Utils Error: Failed to check existence of '" << path << "': " << e.what() << std::endl;
        return false; // Assume non-existent on error
    }
}

// --- Error and Logging Utilities ---

void logInfo(const std::string& message) {
    std::cout << "Info: " << message << std::endl;
}

void logWarning(const std::string& message) {
    std::cerr << "Warning: " << message << std::endl;
}

void logError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

// --- COASM Specific Utilities (if any arise) ---

// Placeholder for any truly COASM-specific utility functions that don't fit elsewhere.
// For example, functions to validate COASM register names, parse COASM directives, etc.
// These would be added as needed during development.

// Example: A simple function to check if a string looks like a COASM register
bool isCoasmRegister(const std::string& operand) {
    if (operand.empty() || operand[0] != '%') {
        return false;
    }
    // Basic check: % followed by alphanumeric/underscore
    // A more robust check would involve regex or parsing the register name structure
    return operand.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_%.") == std::string::npos;
}

// Example: A simple function to extract register number (very basic)
int extractRegisterNumber(const std::string& regName) {
    if (!isCoasmRegister(regName)) {
        return -1; // Invalid register name
    }
    // Find the start of the number part
    size_t numStart = 1;
    while(numStart < regName.size() && (std::isalpha(regName[numStart]) || regName[numStart] == '_')) {
        numStart++;
    }
    if (numStart < regName.size()) {
        try {
            return std::stoi(regName.substr(numStart));
        } catch (const std::exception&) {
            return -1; // Could not parse number
        }
    }
    return -1; // No number found
}


} // namespace utils
} // namespace opuas
