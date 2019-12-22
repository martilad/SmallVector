#include <cassert>
#include <cstddef>

namespace test7{

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

        // move assignment operator for "small" vector:
        mpc::small_vector<Y, 2> v2;
        v2 = std::move(v);

        assert(v2[0] == 1);
        assert(v2.size() == 1);
        assert(v2.capacity() == 2);

        v2.push_back(2);
        v2.push_back(3);

        // move assignment operator for "large" vector
        mpc::small_vector<Y, 2> v3;
        v3 = std::move(v2);

        assert(v3[0] == 1);
        assert(v3[1] == 2);
        assert(v3[2] == 3);
        assert(v3.size() == 3);

        // other case:
        mpc::small_vector<Y, 2> v4;
        v4.push_back(-1);
        v3 = std::move(v4);
        assert(v3[0] == -1);
        assert(v3.size() == 1);
    }
}