#include <cassert>
#include <cstddef>
namespace test9{
class X
{
    int i_;
public:
    X() : i_(0) { }
    X(int i) : i_(i) { }
    X(const X&) = default;
    X(X&&) = default;
    X& operator=(const X&) = delete;
    X& operator=(X&&) = default;
    operator int() const { return i_; }
};

int main()
{
    mpc::small_vector<X, 2> v;
    v.resize(3, -1);

    assert(v.size() == 3);
    assert(v[0] == -1);
    assert(v[1] == -1);
    assert(v[2] == -1);

    auto temp = v.capacity();

    v.resize(1);

    assert(v.size() == 1);
    assert(v[0] == -1);
    assert(v.capacity() == temp);
    return 0;
}
}