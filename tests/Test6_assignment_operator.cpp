#include <cassert>
#include <cstddef>

namespace test6{
class X
{
    int i_;
public:
    X(int i) : i_(i) { }
    X(const X&) = default;
    X(X&&) = default;
    X& operator=(const X&) = default;
    X& operator=(X&&) = default;
    operator int() const { return i_; }
};

int main()
{
    mpc::small_vector<X, 2> v;
    v.push_back(1);

    // copy assignment operator for "small" vector:
    mpc::small_vector<X, 2> v2;
    v2 = v;

    assert(v[0] == 1);
    assert(v.size() == 1);
    assert(v.capacity() == 2);

    assert(v2[0] == 1);
    assert(v2.size() == 1);
    assert(v2.capacity() == 2);

    v.push_back(2);
    v.push_back(3);

    // copy assignment operator for "large" vector
    mpc::small_vector<X, 2> v3;
    v3 = v;

    assert(v.size() == 3);
    assert(v.capacity() == 4);

    assert(v3[0] == 1);
    assert(v3[1] == 2);
    assert(v3[2] == 3);
    assert(v3.size() == 3);

    // other case #1:
    mpc::small_vector<X, 2> v4;
    v3 = v4;

    assert(v3.size() == 0);

    // other case #2:
    v = v2;

    assert(v2[0] == 1);
    assert(v2.size() == 1);

    assert(v[0] == 1);
    assert(v.size() == 1);
}
}