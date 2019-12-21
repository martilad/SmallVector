#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>

const size_t max_align = alignof(std::max_align_t);

struct X
{
    alignas(max_align) char _[max_align];
};

int alignTest()
{

    std::cout << "Maximum fundamental alignment: " << max_align << std::endl;
    std::cout << "Alignment of X: " << alignof(X) << std::endl;

    mpc::small_vector<X, 4> v;
    uintptr_t addr = reinterpret_cast<uintptr_t>(v.data());
    std::cout << "Address of SBO buffer: " << std::hex << addr << std::endl;
    assert(addr % max_align == 0);
    return 0;
}