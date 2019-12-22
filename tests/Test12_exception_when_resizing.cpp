#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace test12{

class X
{
    int i_;
    static size_t cnt_;
public:
    X() : i_(0) { }
    X(int i) : i_(i) { }

    X(const X&)
    {
        if (++cnt_ == 2)
            throw std::runtime_error("copy error");
    }

    X& operator=(const X&) = delete;
    operator int() const { return i_; }
};

size_t X::cnt_ = 0;

int main()
{
    mpc::small_vector<X, 2> v;
    v.emplace_back(1);
    v.emplace_back(2);

    try
    {
        v.emplace_back(3);
    }
    catch (std::runtime_error& e)
    {
        assert(strcmp(e.what(), "copy error") == 0);
    }

    assert(v.size() == 2);
    assert(v.capacity() == 2);
    assert(v[0] == 1);
    assert(v[1] == 2);
}

}