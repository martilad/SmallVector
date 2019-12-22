#include <cassert>
#include <cstddef>
namespace test2{

class Y
{
    static size_t created_; // number of created instances
    static size_t live_;    // number of "live" instances
public:
    Y() = delete;
    Y(int) { created_++; live_++; }
    Y(const Y&) : Y(0) { }
    Y(Y&&) : Y(0) { }
    ~Y() { live_--;}

    static size_t created() { return created_; }
    static size_t live() { return live_; }
};

int main()
{
    mpc::small_vector<Y, 2> v;

    assert(Y::created() == 0);

    v.push_back(1);

    assert(Y::created() >= 1);
    assert(Y::live() == 1);

    v.clear();
    assert(Y::live() == 0);

    size_t temp = Y::created();
    v.emplace_back(2);
    assert(Y::created() == temp + 1);
    assert(Y::live() == 1);
    v.reserve(4);
    assert(Y::created() == temp + 2);
    assert(Y::live() == 1);
    return 0;
}

size_t Y::created_ = 0;
size_t Y::live_ = 0;
}