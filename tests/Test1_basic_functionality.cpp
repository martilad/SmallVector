#include <cassert>

static std::size_t allocated = 0;
void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;

int test1()
{
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
    return 0;
}

void* operator new(std::size_t size)
{
    allocated += size;
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}
