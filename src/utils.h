// opuas/src/utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace opuas {
namespace utils {

// --- File I/O Utilities ---
bool readFileToString(const std::string& filename, std::string& content);
bool writeStringToFile(const std::string& filename, const std::string& content);

// --- String Manipulation Utilities ---
std::string trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);

// --- Path and Directory Utilities (C++17) ---
bool createDirectories(const std::string& path);
std::string getParentPath(const std::string& path);
std::string getFilename(const std::string& path);
bool pathExists(const std::string& path);

// --- Error and Logging Utilities ---
void logInfo(const std::string& message);
void logWarning(const std::string& message);
void logError(const std::string& message);

// --- COASM Specific Utilities (Placeholders) ---
bool isCoasmRegister(const std::string& operand);
int extractRegisterNumber(const std::string& regName);

} // namespace utils
} // namespace opuas

#endif // UTILS_H
