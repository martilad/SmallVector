#include <cassert>
#include <cstddef>

namespace test10 {
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
    mpc::small_vector<X, 3> v = { 1, 2, 3 };

    assert(v.size() == 3);
    assert(v.capacity() == 3);
    for (size_t i = 0; i < v.size(); i++)
        assert(v[i] == static_cast<int>(i + 1));

    for (int i = 4; i <= 6; i++)
        v.push_back(i);

    assert(v.size() == 6);
    for (size_t i = 0; i < v.size(); i++)
        assert(v[i] == static_cast<int>(i + 1));
    return 0;
}

}