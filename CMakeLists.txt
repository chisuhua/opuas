# opuas/CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(opuas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find ANTLR Runtime ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(ANTLR REQUIRED IMPORTED_TARGET antlr4-runtime)

# --- Locate coasm_infra Project ---
option(COASM_INFRA_ROOT "Path to the coasm_infra project root directory" "")
if(NOT COASM_INFRA_ROOT)
    set(COASM_INFRA_ROOT $ENV{COASM_INFRA_ROOT})
endif()
if(NOT COASM_INFRA_ROOT)
    # Default relative path - adjust if needed
    set(COASM_INFRA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../coasm_infra")
endif()
get_filename_component(COASM_INFRA_ROOT "${COASM_INFRA_ROOT}" ABSOLUTE)
if(NOT EXISTS "${COASM_INFRA_ROOT}/coasm_isa.md")
    message(FATAL_ERROR "coasm_infra directory '${COASM_INFRA_ROOT}' not found or invalid. Please set COASM_INFRA_ROOT.")
endif()
message(STATUS "Found coasm_infra at: ${COASM_INFRA_ROOT}")

# --- Define paths to coasm_infra generated artifacts ---
set(COASM_INFRA_GENERATED_DIR "${COASM_INFRA_ROOT}/generated")
set(COASM_GRAMMAR_FILE "${COASM_INFRA_GENERATED_DIR}/coasm.g4")
set(COASM_INFRA_ANTLR_GENERATED_DIR "${COASM_INFRA_GENERATED_DIR}/antlr_generated")

# --- Ensure coasm_infra artifacts are present (Conceptual/Placeholder) ---
# This assumes coasm_infra has a script or mechanism to generate its outputs.
add_custom_target(ensure_coasm_infra_artifacts
    COMMAND ${CMAKE_COMMAND} -E echo "Ensuring coasm_infra artifacts exist..."
    # This command should run coasm_infra's generator if files are missing/outdated
    # Example (adjust path/script name):
    COMMAND python ${COASM_INFRA_ROOT}/scripts/coasm_gen.py --input ${COASM_INFRA_ROOT}/coasm_isa.md --output-dir ${COASM_INFRA_GENERATED_DIR} --antlr
    DEPENDS ${COASM_INFRA_ROOT}/coasm_isa.md ${COASM_INFRA_ROOT}/scripts/coasm_gen.py # Add other deps if needed
    COMMENT "Ensuring coasm_infra artifacts (grammar, ANTLR C++) are present..."
    VERBATIM
)

# --- Generate OPUAS's ANTLR C++ code from coasm_infra's coasm.g4 ---
set(ANTLR_OPUAS_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/antlr_opuas)
file(MAKE_DIRECTORY ${ANTLR_OPUAS_GENERATED_DIR})

# Copy coasm.g4 to our build area to avoid polluting coasm_infra
configure_file(${COASM_GRAMMAR_FILE} ${ANTLR_OPUAS_GENERATED_DIR}/coasm.g4 COPYONLY)

set(ANTLR_OPUAS_OUTPUT_FILES
    ${ANTLR_OPUAS_GENERATED_DIR}/coasmLexer.cpp
    ${ANTLR_OPUAS_GENERATED_DIR}/coasmLexer.h
    ${ANTLR_OPUAS_GENERATED_DIR}/coasmParser.cpp
    ${ANTLR_OPUAS_GENERATED_DIR}/coasmParser.h
    # Add other generated files if needed (BaseVisitor, Visitor, etc.)
)

add_custom_command(
    OUTPUT ${ANTLR_OPUAS_OUTPUT_FILES}
    COMMAND ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_EXECUTABLE}
        -Dlanguage=Cpp
        -visitor # If you plan to use Visitor pattern
        -no-listener # If you don't need Listener pattern
        -o ${ANTLR_OPUAS_GENERATED_DIR}
        ${ANTLR_OPUAS_GENERATED_DIR}/coasm.g4 # Use the copied grammar file
    DEPENDS ${COASM_GRAMMAR_FILE} ensure_coasm_infra_artifacts # Depend on coasm.g4 and ensure it's generated first
    COMMENT "Generating ANTLR C++ files for COASM (for opuas validation)..."
    VERBATIM
)

add_custom_target(antlr_gen_opuas DEPENDS ${ANTLR_OPUAS_OUTPUT_FILES})
add_dependencies(antlr_gen_opuas ensure_coasm_infra_artifacts) # Ensure coasm_infra runs first


# --- Source Files ---
set(SOURCES
    src/main.cpp
    src/OpuAssembler.cpp
    src/OpuDisassembler.cpp
    src/Validator.cpp
    src/Parser.cpp
    src/CodeGenerator.cpp
    src/elf/ElfObjectWriter.cpp
    src/elf/ElfObjectReader.cpp
    src/algorithms/RegisterAllocator.cpp
    src/algorithms/StallSetter.cpp
    src/utils.cpp
    # Add OPUAS ANTLR generated files
    ${ANTLR_OPUAS_OUTPUT_FILES}
    # Add other source files as needed
)

# --- Create Executable ---
add_executable(opuas ${SOURCES})

# --- Ensure Dependencies ---
add_dependencies(opuas antlr_gen_opuas) # Ensure ANTLR codegen happens first
add_dependencies(opuas ensure_coasm_infra_artifacts) # Ensure coasm_infra artifacts exist

# --- Include Directories ---
target_include_directories(opuas PRIVATE
    ${ANTLR_OPUAS_GENERATED_DIR}   # Include OPUAS ANTLR headers
    ${COASM_INFRA_GENERATED_DIR}   # Include coasm_infra's generated .def files if needed directly
    src/                           # Include project's own src directory
    src/elf/                       # Include elf subdir
    src/algorithms/                # Include algorithms subdir
    # Add paths to third-party libraries (e.g., ELFIO) if used
)

# --- Link Libraries ---
target_link_libraries(opuas PkgConfig::ANTLR)
# Link other libraries (e.g., ELFIO for ELF manipulation)
# target_link_libraries(opuas /path/to/elfio/libelfio.a) # Or find_package + target_link_libraries

# --- Installation (Optional) ---
install(TARGETS opuas DESTINATION bin)
