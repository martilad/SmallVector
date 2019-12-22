#include <cassert>
#include <cstddef>

namespace test11{
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
    mpc::small_vector<X, 3> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    assert(v.size() == 3);

    int i = 0;
    for (const auto& x : v)
        assert(x == ++i);

    v.push_back(4);
    v.push_back(5);
    v.push_back(6);

    i = 0;
    for (const auto& x : v)
        assert(x == ++i);
    return 0;
}
}