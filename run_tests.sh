#!/bin/bash

################################################################################
# run_tests.sh - Test Runner for Restock VCV Rack Plugin
################################################################################
#
# PURPOSE:
#   This script compiles and runs all unit tests for the Restock plugin.
#   It ensures tests are built properly and reports results clearly.
#
# USAGE:
#   ./run_tests.sh [options]
#
# OPTIONS:
#   --clean      Clean build directory before compiling
#   --no-build   Skip build step and run existing test binaries
#   --verbose    Show detailed compilation output
#   --help       Display this help message
#
# EXIT CODES:
#   0 - All tests passed
#   1 - One or more tests failed
#   2 - Build failed
#   3 - No test files found
#
################################################################################

set -e  # Exit on error (will be disabled for test execution)

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="build"
TEST_DIR="src/tests"
TEST_BUILD_DIR="${BUILD_DIR}/tests"
RACK_SDK="dep/Rack-SDK"

# Parse command line arguments
CLEAN_BUILD=false
NO_BUILD=false
VERBOSE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --no-build)
            NO_BUILD=true
            shift
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --help)
            grep "^#" "$0" | grep -v "#!/bin/bash" | sed 's/^# //' | sed 's/^#//'
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

################################################################################
# Helper Functions
################################################################################

print_header() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

################################################################################
# Main Script
################################################################################

print_header "Restock Plugin Test Runner"

# Check if we're in the right directory
if [ ! -f "plugin.json" ]; then
    print_error "plugin.json not found. Please run this script from the repository root."
    exit 2
fi

# Check if Rack SDK exists
if [ ! -d "$RACK_SDK" ]; then
    print_error "Rack SDK not found at $RACK_SDK"
    print_info "Please ensure the Rack SDK is properly installed in the dep/ directory"
    exit 2
fi

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned"
    fi
fi

# Create test build directory
mkdir -p "$TEST_BUILD_DIR"

# Find all test source files
TEST_FILES=$(find "$TEST_DIR" -name "*_test.cpp" 2>/dev/null || true)

if [ -z "$TEST_FILES" ]; then
    print_error "No test files found in $TEST_DIR"
    print_info "Test files should be named *_test.cpp"
    exit 3
fi

TEST_COUNT=$(echo "$TEST_FILES" | wc -l | tr -d ' ')
print_info "Found $TEST_COUNT test file(s)"

################################################################################
# Build Tests
################################################################################

if [ "$NO_BUILD" = false ]; then
    print_header "Building Tests"
    
    # Compilation flags based on Rack SDK
    CXXFLAGS="-std=c++11 -I./src -I${RACK_SDK}/include -I${RACK_SDK}/dep/include"
    LDFLAGS="-L${RACK_SDK} -L${RACK_SDK}/dep/lib"
    
    if [ "$(uname)" = "Darwin" ]; then
        # macOS specific flags
        CXXFLAGS="$CXXFLAGS -DARCH_MAC"
    elif [ "$(uname)" = "Linux" ]; then
        # Linux specific flags
        CXXFLAGS="$CXXFLAGS -DARCH_LIN"
    fi
    
    BUILD_FAILED=false
    
    for test_file in $TEST_FILES; do
        test_name=$(basename "$test_file" .cpp)
        test_binary="${TEST_BUILD_DIR}/${test_name}"
        
        print_info "Compiling $test_name..."
        
        # Compile the test (tests are now standalone and don't need wavetable.cpp)
        compile_cmd="g++ $CXXFLAGS $test_file -o $test_binary -lm"
        
        if [ "$VERBOSE" = true ]; then
            echo "$compile_cmd"
            if ! eval "$compile_cmd"; then
                print_error "Failed to compile $test_name"
                BUILD_FAILED=true
            else
                print_success "Compiled $test_name"
            fi
        else
            if ! eval "$compile_cmd" 2>&1 | grep -i "error" && eval "$compile_cmd" &>/dev/null; then
                print_success "Compiled $test_name"
            else
                # Try to compile again with output for debugging
                print_error "Failed to compile $test_name"
                print_info "Compilation errors:"
                eval "$compile_cmd" 2>&1 | head -20
                BUILD_FAILED=true
            fi
        fi
    done
    
    if [ "$BUILD_FAILED" = true ]; then
        print_error "Build failed. Please fix compilation errors."
        exit 2
    fi
    
    echo ""
    print_success "All tests compiled successfully"
else
    print_info "Skipping build step (--no-build specified)"
fi

################################################################################
# Run Tests
################################################################################

print_header "Running Tests"

# Disable exit on error for test execution
set +e

TESTS_PASSED=0
TESTS_FAILED=0
FAILED_TESTS=()

for test_file in $TEST_FILES; do
    test_name=$(basename "$test_file" .cpp)
    test_binary="${TEST_BUILD_DIR}/${test_name}"
    
    if [ ! -f "$test_binary" ]; then
        print_warning "Test binary not found: $test_binary"
        print_info "Run without --no-build to compile tests"
        continue
    fi
    
    if [ ! -x "$test_binary" ]; then
        chmod +x "$test_binary"
    fi
    
    echo ""
    print_info "Running $test_name..."
    echo ""
    
    # Run the test and capture output
    if "$test_binary"; then
        TESTS_PASSED=$((TESTS_PASSED + 1))
        echo ""
        print_success "$test_name passed"
    else
        TESTS_FAILED=$((TESTS_FAILED + 1))
        FAILED_TESTS+=("$test_name")
        echo ""
        print_error "$test_name failed"
    fi
done

################################################################################
# Summary
################################################################################

print_header "Test Summary"

echo "Total test suites: $TEST_COUNT"
echo ""

if [ $TESTS_PASSED -gt 0 ]; then
    print_success "Passed: $TESTS_PASSED"
fi

if [ $TESTS_FAILED -gt 0 ]; then
    print_error "Failed: $TESTS_FAILED"
    echo ""
    print_info "Failed tests:"
    for failed_test in "${FAILED_TESTS[@]}"; do
        echo "  - $failed_test"
    done
fi

echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    print_header "All Tests Passed! ✓"
    exit 0
else
    print_header "Some Tests Failed ✗"
    exit 1
fi
