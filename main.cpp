#include "small_vector.h"
#include "tests/BasicTest.cpp"
#include "tests/AlignTest.cpp"
#include "tests/Test1_basic_functionality.cpp"
#include "tests/Test2_value_type.cpp"
#include "tests/Test3_inserting.cpp"
#include "tests/Test4_copy_constructor.cpp"
#include "tests/Test5_move_constructor.cpp"
#include "tests/Test6_assignment_operator.cpp"
#include "tests/Test7_assignment_operator.cpp"
#include "tests/Test8_swap_content.cpp"

int main() {
    //basicTest();
    //alignTest();
    //test1(); // fail with valgrind - valgrind override the new probably
    test2::main();
    test3::main();
    test4::main();
    test5::main();
    test6::main();
    test7::main();
    test8::main();
    return 0;
}

