
#!/bin/bash

# --- Test Script for opuas ---

# Get script directory and project root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_INPUT="$SCRIPT_DIR/test_simple.coasm"
TEST_OUTPUT="$SCRIPT_DIR/test_output.asm" # Or .o, .cubin

echo "Testing opuas project..."

# --- 1. Ensure coasm_infra is built and artifacts are generated ---
echo "Ensuring coasm_infra artifacts are present..."
# This assumes coasm_infra's build system or generator script is run separately
# or that opuas's CMakeLists.txt handles it via add_custom_target.
# For manual testing, ensure coasm_infra has been run:
# pushd $PROJECT_ROOT/../coasm_infra
# python scripts/coasm_gen.py --input coasm_isa.md --output-dir generated --antlr
# popd

# --- 2. Build opuas ---
echo "Building opuas..."
cd "$BUILD_DIR" || exit 1
# cmake .. # Run if build dir is fresh or CMakeLists.txt changed
make clean # Optional, for a clean test
make || { echo "Build failed!"; exit 1; }

# --- 3. Run the test ---
echo "Running assembly test on '$TEST_INPUT'..."
./opuas assemble "$TEST_INPUT" "$TEST_OUTPUT"

if [ $? -eq 0 ]; then
    echo "Test PASSED: opuas successfully assembled '$TEST_INPUT'."
    echo "Output written to '$TEST_OUTPUT'."
    echo "Contents of '$TEST_OUTPUT':"
    cat "$TEST_OUTPUT"
else
    echo "Test FAILED: opuas failed to assemble '$TEST_INPUT'."
    exit 1
fi

echo "Test completed."
