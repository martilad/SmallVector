#include "small_vector.h"
#include "tests/BasicTest.cpp"
#include "tests/AlignTest.cpp"
#include "tests/Test1_basic_functionality.cpp"
#include "tests/Test2_value_type.cpp"
#include "tests/Test3_inserting.cpp"

int main() {
    basicTest();
    alignTest();
    test1();
    test2::main();
    test3::main();
    return 0;
}

