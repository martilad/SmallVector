#ifndef MPC_SMALL_VECTOR
#define MPC_SMALL_VECTOR

#include <cstddef>

namespace mpc {

template <typename T, size_t N>
class small_vector {
public:
    small_vector(){
        this->mCapacity = N;
        this->mSize = 0;
    }
    size_t size() const{return this->mSize;}
    size_t capacity() const {return this->mCapacity;}

    T* data(){};
    void reserve(size_t capacity){}
    void clear (){}
    void push_back(T item){}
    T &operator[] (int){}

private:
    size_t mCapacity{};
    size_t mSize{};


}; // class small vector

} // namespace mpc

#endif // MPC_SMALL_VECTOR