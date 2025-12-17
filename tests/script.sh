#!/bin/bash
echo "Testing memory fix..."

# Create test
cat > test_memory.cpp << 'EOF'
#include <iostream>

class Test {
public:
    Test() { std::cout << "Test created\n"; }
    ~Test() { std::cout << "Test destroyed\n"; }
};

int main() {
    Test* t = new Test();
    
    // Simulating the bug: double delete
    delete t;  // First delete
    // delete t;  // Second delete would crash
    
    std::cout << "Program completed\n";
    return 0;
}
EOF

g++ -std=c++17 test_memory.cpp -o test_memory
./test_memory

# Clean up
rm -f test_memory test_memory.cpp

echo ""
echo "Now rebuilding pipeline with fix..."
make clean
make all

echo "Testing with convolution mode..."
timeout 3 ./bin/pipeline_sim assets/test_simple.ppm output/memory_test.ppm --mode=conv 2>&1

if [ $? -eq 0 ]; then
    echo "✓ No segmentation fault!"
else
    echo "✗ Still crashing"
fi