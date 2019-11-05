#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "small_vector.h"

void basicStartTest() {
    mpc::small_vector<int, 16> v;
    assert(v.size() == 0);
    assert(v.capacity() == 16);
}

/*const size_t max_align = alignof(std::max_align_t);

struct X
{
    alignas(max_align) char _[max_align];
};

void alignTest(){
    std::cout << "Maximum fundamental alignment: " << max_align << std::endl;
    std::cout << "Alignment of X: " << alignof(X) << std::endl;

    mpc::small_vector<X, 4> v;
    uintptr_t addr = reinterpret_cast<uintptr_t>(v.data());
    std::cout << "Address of SBO buffer: " << std::hex << addr << std::endl;
    assert(addr % max_align == 0);
}

static std::size_t allocated = 0;
void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;
void* operator new(std::size_t size)
{
    allocated += size;
    return std::malloc(size);
}
void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

void basicFunctionality1(){
    // základní funkcionalita pro hodnotový typ int,
    // testování ukládání prkvů, testování správné velikosti a kapacity vektoru,
    // testování využití bufferu a dynamických alokací:
    allocated = 0;

    mpc::small_vector<int, 2> v;

    assert(v.size() == 0);
    assert(v.capacity() == 2);
    assert(allocated == 0);

    v.push_back(1);
    v.push_back(2);

    assert(v.size() == 2);
    assert(v.capacity() == 2);
    assert(allocated == 0);
    assert(v[0] == 1);
    assert(v[1] == 2);

    v.push_back(3);

    assert(v.capacity() == 4);
    assert(v.size() == 3);
    assert(allocated == sizeof(int) * 4);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    v.reserve(6);
    assert(v.capacity() == 6);
    assert(v.size() == 3);
    assert(allocated == sizeof(int) * (4 + 6));
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    v.clear();
    assert(v.capacity() == 6);
    assert(v.size() == 0);
}*/


int main() {
    basicStartTest();
    //alignTest();
    //basicFunctionality1();
}

