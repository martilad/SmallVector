#include <cassert>
#include <cstddef>
namespace test8{
class Y
{
    int i_;
public:
    Y(int i) : i_(i) { }
    Y(const Y&) = delete;
    Y(Y&&) = default;
    Y& operator=(const Y&) = delete;
    Y& operator=(Y&&) = default;
    operator int() const { return i_; }
};

int main()
{
    mpc::small_vector<Y, 2> v;
    v.push_back(1);

    // swap of small-small vectors:
    mpc::small_vector<Y, 2> v2;
    v2.push_back(-1);
    v2.push_back(-2);
    v.swap(v2);

    assert(v.size() == 2);
    assert(v[1] == -2);
    assert(v2.size() == 1);
    assert(v2[0] == 1);

    // swap of large-small vectors:
    v.push_back(-3);
    v.swap(v2);

    assert(v.size() == 1);
    assert(v[0] == 1);
    assert(v2.size() == 3);
    assert(v2[2] == -3);

    // swap of large-large vectors:
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.swap(v2);

    assert(v.size() == 3);
    assert(v[2] == -3);
    assert(v2.size() == 4);
    assert(v2[3] == 4);

    mpc::small_vector<Y, 2>{}.swap(v);
    assert(v.size() == 0);
    return 0;
}
}