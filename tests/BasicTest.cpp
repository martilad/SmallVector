#include <cassert>


int basicTest()
{
    mpc::small_vector<int, 16> v;
    assert(v.size() == 0);
    assert(v.capacity() == 16);
    return 0;
}
