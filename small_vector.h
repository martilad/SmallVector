#ifndef MPC_SMALL_VECTOR
#define MPC_SMALL_VECTOR

#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cstring>

namespace mpc {

    template<typename T, size_t N>
    class small_vector {
    public:
        small_vector() : mCapacity(N), mSize(0),
                         mData(reinterpret_cast<typename std::aligned_storage<sizeof(T), alignof(T)>::type *>(mBuff)) {

        }

        small_vector(const small_vector &other) : mCapacity(N), mSize(0),
                                                  mData(reinterpret_cast<typename std::aligned_storage<sizeof(T), alignof(T)>::type *>(mBuff)) {
            this->reserve(other.mSize);
            if (other.mSize > N) {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(this->mData + i) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            } else {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            }
            this->mSize = other.mSize;
        }

        small_vector(small_vector &&other) noexcept : mCapacity(N), mSize(0),
                                                      mData(reinterpret_cast<typename std::aligned_storage<sizeof(T), alignof(T)>::type *>(mBuff)) {
            this->reserve(other.mSize);
            if (other.mSize > N) {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(this->mData + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            } else {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            }
            this->mSize = other.mSize;
        }

        small_vector(std::initializer_list<T> init) : mCapacity(N), mSize(0),
                                                      mData(reinterpret_cast<typename std::aligned_storage<sizeof(T), alignof(T)>::type *>(mBuff)) {
            this->reserve(init.size());
            size_t i = 0;
            if (init.size() > N) {
                for (auto it = init.begin(); it < init.end(); ++it, ++i) {
                    new(this->mData + i) T(std::move_if_noexcept(*it));
                }
            } else {
                for (auto it = init.begin(); it < init.end(); ++it, ++i) {
                    new(&this->mBuff[i]) T(std::move_if_noexcept(*it));
                }
            }
            this->mSize = init.size();
        }

        ~small_vector() noexcept {
            this->clear();
            if (this->mIsAllocated) {
                delete[]this->mData; // deallocate original memory
            }
        }

        small_vector &operator=(const small_vector &other) { // copy assignment operator -- both vectors will have same content
            this->clear();
            this->reserve(other.mSize);
            if (other.mSize > N || this->mIsAllocated) {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(this->mData + i) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            } else {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            }
            this->mSize = other.mSize;
            return *this;
        }

        small_vector &operator=(small_vector &&other) { // move assignment operator - other will be empty after operation
            this->clear();
            this->reserve(other.mSize);
            if (other.mSize > N || this->mIsAllocated) {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(this->mData + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            } else {
                for (size_t i = 0; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            }
            this->mSize = other.mSize;
            other.clear();
            return *this;
        }

        size_t size() const {
            return this->mSize;
        }

        size_t capacity() const {
            return this->mCapacity;
        }

        T *data() { // return pointer to the firs element in the vector
            return reinterpret_cast<T *>(this->mData);
        };

        const T *data() const { // return const pointer to the first element in vector
            return reinterpret_cast<const T *>(this->mData);
        }

        void clear() { // erase elements in array - decreasing index order
            for (int i = this->mSize - 1; i >= 0; i--) {
                reinterpret_cast<T *>(&this->mData[i])->~T();
            }
            this->mSize = 0;
        }

        /**
         * Allocate new memory for the vector and move content to new vector and destroy the old one
         * use move semantic if is noexcept
         * @param capacity new capacity of the vector
         */
        void reserve(size_t capacity) {
            if (capacity <= this->mCapacity) return;
            // allocate new memory
            typename std::aligned_storage<sizeof(T), alignof(T)>::type *tmp = nullptr;
            try {
                tmp = new typename std::aligned_storage<sizeof(T), alignof(T)>::type[capacity];
                size_t i = 0;
                for (; i < this->mSize; i++) {
                    // move from old
                    new(tmp + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&this->mData[i])));
                }
                clear(); // destroy original (now empty) elements
                if (this->mIsAllocated) {
                    delete[]this->mData; // deallocate original memory
                }
                this->mData = tmp;
                this->mCapacity = capacity;
                this->mSize = i;
                this->mIsAllocated = true;
            } catch (std::runtime_error &e) {
                if (tmp != nullptr) delete[]tmp;
                throw e;
            } catch (std::exception &e) {
                if (tmp != nullptr) delete[]tmp;
                throw e;
            }
        }

        void push_back(const T &value) { // inserts a new element at the end of the vector - use copy semantic
            if (this->size() < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(value);
            } else if (this->size() >= this->capacity()) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(value);
            } else {
                new(this->mData + this->mSize) T(value);
            }
            this->mSize++;
        }

        void push_back(T &&value) { // inserts a new element at the end of the vector - use moving semantic
            if (this->mSize < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(std::move(value));
            } else if (this->mSize >= this->mCapacity) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(std::move(value));
            } else {
                new(this->mData + this->mSize) T(std::move(value));
            }
            this->mSize++;
        };

        template<typename... Ts>
        void emplace_back(Ts &&... vs) { // insert new last element created with arguments vs ... (perfect forwarding)
            if (this->mSize < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(std::forward<Ts>(vs)...);
            } else if (this->mSize >= this->mCapacity) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(std::forward<Ts>(vs)...);
            } else {
                new(this->mData + this->mSize) T(std::forward<Ts>(vs)...);
            }
            this->mSize++;
        }

        /**
         * Resize function - To call this function, the vector value type must support copy semantics.
         * @param size new size of vector - if is smaller the elements are destruct
         * @param value value whithc is copy to new element if size is bigger than this->mSize
         */
        void resize(size_t size, const T &value = T()) {
            if (size < this->mSize) {
                for (size_t i = this->mSize - 1; i >= size; i--) {
                    reinterpret_cast<T *>(&this->mData[i])->~T();
                }
                this->mSize = size;
            }
            if (size > this->mSize) {
                this->reserve(size);
                if (size > N || this->mIsAllocated) {
                    for (size_t i = this->mSize; i < size; i++) {
                        new(this->mData + i) T(value);
                    }
                } else {
                    for (size_t i = this->mSize; i < size; i++) {
                        new(&this->mBuff[i]) T(value);
                    }
                }
                this->mSize = size;
            }
        }

        T &operator[](size_t index) { // return reference on element at index
            return *reinterpret_cast<T *>(&this->mData[index]);
        }

        const T &operator[](size_t index) const { // return const reference on element at index
            return *reinterpret_cast<const T *>(&this->mData[index]);
        }

        T *begin() { // return iterator on first element in array
            return reinterpret_cast<T *>(&this->mData[0]);
            //  vrací iterátor na první prvek vektoru.
        }

        const T *begin() const { // return const iterator on first element in array
            return *reinterpret_cast<const T *>(&this->mData[0]);
        }

        T *end() { // return iterator on last element in array
            return reinterpret_cast<T *>(&this->mData[this->mSize]);
        }

        const T *end() const { // return const iterator on last element in array
            return *reinterpret_cast<const T *>(&this->mData[this->mSize]);
        }

        void swap(small_vector &other) noexcept { // swap content with small_vector other
            if (this == &other) return;
            std::swap(this->mSize, other.mSize);
            std::swap(this->mCapacity, other.mCapacity);
            std::swap(this->mIsAllocated, other.mIsAllocated);
            std::swap(this->mData, other.mData);
        }

    private:
        // switch to check if is use buffer or memory are allocated
        bool mIsAllocated = false;
        size_t mCapacity{};
        size_t mSize{};
        //Aligned_storage which is "type suitable for use as uninitialized storage" no need default constructor of type
        typename std::aligned_storage<sizeof(T), alignof(T)>::type mBuff[N];
        typename std::aligned_storage<sizeof(T), alignof(T)>::type *mData = nullptr;


    }; // class small vector

    template<typename T, size_t N>
    void swap(small_vector<T, N> &a, small_vector<T, N> &b) { //swap two vectors using small_vector swap
        a.swap(b);
    }
} // namespace mpc

#endif // MPC_SMALL_VECTOR
