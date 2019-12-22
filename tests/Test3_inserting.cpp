#include <cassert>
#include <cstddef>
namespace test3 {
    class A {
        static size_t copied_; // how many times copy ctor invoked
        static size_t moved_;  // how many times move ctor invoked
        int i_;
    public:
        A(int i) : i_(i) {}

        A(const A &other) : i_(other.i_) { copied_++; };

        A(A &&other) noexcept : i_(other.i_) { moved_++; }

        static size_t copied() { return copied_; }

        static size_t moved() { return moved_; }
    };

    class B {
        static size_t copied_; // how many times copy ctor invoked
        static size_t moved_;  // how many times move ctor invoked
        int i_;
    public:
        B(int i) : i_(i) {}

        B(const B &other) : i_(other.i_) { copied_++; };

        B(B &&other) : i_(other.i_) { moved_++; } // NOT noexcept !!!
        static size_t copied() { return copied_; }

        static size_t moved() { return moved_; }
    };

    int main() {
        mpc::small_vector<A, 2> v;
        v.push_back(1);
        assert(A::moved() >= 1);
        assert(A::copied() == 0);

        size_t temp = A::moved();
        v.reserve(4);
        assert(A::moved() > temp);
        assert(A::copied() == 0);

        // the very same with Y:
        mpc::small_vector<B, 2> w;
        w.push_back(1);
        assert(B::moved() >= 1);
        assert(B::copied() == 0);

        temp = B::moved();
        w.reserve(4);
        assert(B::moved() == temp);
        assert(B::copied() == 1);
        return 0;
    }

    size_t A::copied_ = 0;
    size_t A::moved_ = 0;
    size_t B::copied_ = 0;
    size_t B::moved_ = 0;
}