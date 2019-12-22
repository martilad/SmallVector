#include <cassert>
#include <cstddef>
namespace test4{
class X
{
    int i_;
public:
    X(int i) : i_(i) { }
    X(const X&) = default;
    X(X&&) = default;
    operator int() const { return i_; }
};

class Y
{
    int i_;
public:
    Y(int i) : i_(i) { }
    Y(const Y&) = delete; // NON-COPYABLE
    Y(Y&&) = default;
    operator int() const { return i_; }
};

int main()
{
    mpc::small_vector<X, 2> v;
    v.push_back(1);

    // copy construcor for "small" vector:
    mpc::small_vector<X, 2> v2(v);

    assert(v[0] == 1);
    assert(v.size() == 1);
    assert(v.capacity() == 2);

    assert(v2[0] == 1);
    assert(v2.size() == 1);
    assert(v2.capacity() == 2);

    v.push_back(2);
    v.push_back(3);

    // copy constructor for "large" vector
    mpc::small_vector<X, 2> v3(v);

    assert(v.size() == 3);
    assert(v.capacity() == 4);

    assert(v3[0] == 1);
    assert(v3[1] == 2);
    assert(v3[2] == 3);
    assert(v3.size() == 3);

    // copy constructor for non-copyable value type must not compile:
    mpc::small_vector<Y, 2> w;
    w.push_back(1);

    //mpc::small_vector<Y, 2> w2(w); // triggers compilation error (e.g., 'use of deleted function Y::Y(const Y&)')
    return 0;
}
}