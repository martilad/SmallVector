#include <cassert>
#include <cstddef>
namespace test5{
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
    mpc::small_vector<Y, 2> v;
    v.push_back(1);

    // move construcor for "small" vector:
    mpc::small_vector<Y, 2> v2(std::move(v));

    assert(v2[0] == 1);
    assert(v2.size() == 1);
    assert(v2.capacity() == 2);

    v2.push_back(2);
    v2.push_back(3);

    // move constructor for "large" vector
    mpc::small_vector<Y, 2> v3(std::move(v2));

    assert(v3[0] == 1);
    assert(v3[1] == 2);
    assert(v3[2] == 3);
    assert(v3.size() == 3);
    return 0;
}
}