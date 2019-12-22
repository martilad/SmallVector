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
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(this->mData + i) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            } else {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            }
            this->mSize = other.mSize;
            // kopírovací konstruktor. Po konstrukci vektoru bude jeho obsah stejný, jako obsah vektoru other.
            // Tj., bude v sobě mít uložený stejný počet prvků a prvky na stejných pozicích budou mít stejné hodnoty (z hledika jejich sémantiky).
            // Prvky nového vektoru musí být zkonstruovány pomocí kopírovací sémantiky, tj. vzniknou jako kopie příslušného prvku vektoru other.
            // Pro volání toho konstruktoru musí hodnotový typ vektoru kopírovací sémantiku podporovat.
        }

        small_vector(small_vector &&other) noexcept : mCapacity(N), mSize(0),
                                             mData(reinterpret_cast<typename std::aligned_storage<sizeof(T), alignof(T)>::type *>(mBuff)) {
            this->reserve(other.mSize);
            if (other.mSize > N) {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(this->mData + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            } else {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            }
            this->mSize = other.mSize;
            // přesouvací konstruktor. Po konstrukci vektoru bude jeho obsah stejný, jako byl obsah vektoru other před touto operací.
            // Obsah vektoru other se bude po této operaci nacházet ve stavu prázdného vektoru.
        }

        small_vector(std::initializer_list<T> init) {
            // todo: konverzní konstruktor, který umožňje naplnit obsah vektoru při jeho inicializaci, a to tak, že zkopíruje prvky inicializačního seznamu init do vznikajícího vektoru.
            // Pro volání toho konstruktoru musí hodnotový typ vektoru podporovat kopírovací sémantiku.
        }

        ~small_vector() {
            // destruktor. Korektně destruuje všechny prvky vektoru a případně uvolní dynamicky alokovanou paměť.
            // Destruktor sám o sobě nesmí vyhodit výjimku.
            this->clear();
            if (this->mIsAllocated) {
                delete []this->mData; // deallocate original memory
            }
        }

        small_vector &operator=(const small_vector &other) {
            this->clear();
            this->reserve(other.mSize);
            if (other.mSize > N || this->mIsAllocated) {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(this->mData + i) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            } else {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(*reinterpret_cast<T *>(&other.mData[i]));
                }
            }
            this->mSize = other.mSize;
            return *this;
            // kopírovací přiřazovací operátor. Po aplikaci operátoru bude obsah vektoru stejný, jako obsah vektoru other.
            // Vrací referenci na sebe sama.
        }

        small_vector &operator=(small_vector &&other) {
            this->clear();
            this->reserve(other.mSize);
            if (other.mSize > N || this->mIsAllocated) {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(this->mData + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            } else {
                size_t i = 0;
                for (; i < other.mSize; i++) {
                    new(&this->mBuff[i]) T(std::move_if_noexcept(*reinterpret_cast<T *>(&other.mData[i])));
                }
            }
            this->mSize = other.mSize;
            other.clear();
            return *this;
            // přesouvací přiřazovací operátor. Po aplikaci operátoru bude obsah vektoru stejný,
            //  jako byl obsah vektoru other před touto operací. Obsah vektoru other se bude po této operaci nacházet ve stavu prázdného vektoru.
            // Vrací referenci na sebe sama.
        }

        size_t size() const {
            return this->mSize;
        }

        size_t capacity() const {
            return this->mCapacity;
        }

        T *data() {
            return reinterpret_cast<T *>(this->mData);
        };

        /*const_pointer data() const {
          //todo:   vrátí ukazatel na první prvek vektoru (varianta pro konstantní vektor).
        }*/


        void clear() {
            for (int i = this->mSize - 1; i >= 0; i--) {
                reinterpret_cast<T *>(&this->mData[i])->~T();
            }
            this->mSize = 0;
            // vymaže obsah vektoru, tj. destruuje všechny jeho prvky, a to v klesajícím pořadí jejich pozic (indexů).
        }

        void reserve(size_t capacity) {
            if (capacity <= this->mCapacity) return;
            // allocate new memory
            auto *tmp = new typename std::aligned_storage<sizeof(T), alignof(T)>::type[capacity];
            size_t i = 0;
            for (; i < this->mSize; i++) {
                // move from old
                new(tmp + i) T(std::move_if_noexcept(*reinterpret_cast<T *>(&this->mData[i])));
            }
            clear(); // destroy original (now empty) elements
            if (this->mIsAllocated) {
                delete []this->mData; // deallocate original memory
            }
            this->mData = tmp;
            this->mCapacity = capacity;
            this->mSize = i;
            this->mIsAllocated = true;

            // potenciálně zvýší kapacitu vektoru na hodnotu parametru capacity.
            //    Pokud je capacity menší nebo rovno aktuální kapacitě, nemá tato funkce žádný efekt.
            //    V opačném případě funkce zvýší kapacitu na capacity prvků, tj. naalokuje nový buffer a do něj přesune stávající obsah vektoru.
            //    [Update 3.11.2019] Pokud hodnotový typ nemá noexcept přesouvací konstruktor,
            //    dostane před přesouvacím konstruktorem při přesunu prvků přednost kopírovací konstruktor (viz std::move_if_noexcept).
            //
        }

        void push_back(const T &value) {
            if (this->size() < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(value);
            } else if (this->size() >= this->capacity()) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(value);
            } else {
                new(this->mData + this->mSize) T(value);
            }
            this->mSize++;
            //  vloží na konec vektoru nový prvek, který vznikne kopírováním obsahu z parametru value.
            //    Pro volání této funkce musí hodnotový typ vektoru podporovat kopírovací sémantiku.
        }

        void push_back(T &&value) {
            if (this->mSize < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(std::move(value));
            } else if (this->mSize >= this->mCapacity) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(std::move(value));
            } else {
                new(this->mData + this->mSize) T(std::move(value));
            }
            this->mSize++;
            //  vloží na konec vektoru nový prvek, který vnikne přesunem obsahu z parametru value.
        };

        template<typename... Ts>
        void emplace_back(Ts &&... vs) {
            if (this->mSize < N && !this->mIsAllocated) {
                new(&this->mBuff[this->mSize]) T(std::forward<Ts>(vs)...);
            } else if (this->mSize >= this->mCapacity) {
                this->reserve(this->mCapacity * 2);
                new(this->mData + this->mSize) T(std::forward<Ts>(vs)...);
            } else {
                new(this->mData + this->mSize) T(std::forward<Ts>(vs)...);
            }
            this->mSize++;
            // vloží na konec vektoru nový prvek, pro jehož konstrukci budou jako argumenty předány parametry vs..., a to pomocí techniky perfect forwarding.v
        }

        void resize(size_t size, const T &value = T()) {
            if (size < this->mSize){
                for (size_t i = this->mSize - 1; i >= size; i--) {
                    reinterpret_cast<T *>(&this->mData[i])->~T();
                }
                this->mSize = size;
            }
            if (size > this->mSize){
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
            //  změna velikosti vektoru.
            //    Pokud je size rovno aktuální velikosti vektoru, nemá tato funkce žádný efekt.
            //    Pokud je size menší než aktuální velikost vektoru, jsou destruovány prvky na pozicích size až size() - 1, a to v klesajícím pořadí jejich pozic. Kapacita vektoru je zachována.
            //    Pokud je size větší než aktuální velikost vektoru, v případě potřeby zvýší kapacitu vektoru tak, aby se do něj vešlo size prvků, a poté přidá na konec vektoru size - size() prvků jako kopie parametru value.
            //    Pro volání této funkce musí hodnotový typ vektoru podporovat kopírovací sémantiku.
            //    V rámci tohoto textu znamená pojem „velikost vektoru“ počet jeho prvků, nikoliv velikost objektu typu mpc::small_vector v bytech (zjistitelnou pomocí operátoru sizeof).
        }

        T &operator[](size_t index) {
            return *reinterpret_cast<T *>(&this->mData[index]);
            //return this->mData[index];
            //  vrací referenci na prvek vektoru na pozici index.
        }

        const T &operator[](size_t index) const {
            return *reinterpret_cast<const T *>(&this->mData[index]);
            // vrací referenci na prvek vektoru na pozici index (varianta pro konstantní vektor)
        }

        /*iterator begin() {
            // todo: vrací iterátor na první prvek vektoru.
        }
        const_iterator begin() const {
            // todo: vrací konstantní iterátor na první prvek konstantního vektoru.
        }
        iterator end() {
            // todo: vrací iterátor za poslední prvek vektoru.
        }
        const_iterator end() const {
            // todo:  vrací konstantní iterátor za poslední prvek konstantního vektoru.
        }*/

        void swap(small_vector &other) {
            if (this == &other) return;
            std::swap(this->mSize, other.mSize);
            std::swap(this->mCapacity, other.mCapacity);
            std::swap(this->mIsAllocated, other.mIsAllocated);
            std::swap(this->mData, other.mData);
            // prohodí obsah vektoru s vektorem other.v
        }

    private:
        bool mIsAllocated = false;
        size_t mCapacity{};
        size_t mSize{};
        // properly aligned uninitialized storage for N T's
        typename std::aligned_storage<sizeof(T), alignof(T)>::type mBuff[N];
        //alignas(alignof(T)) T mBuff[N * sizeof(T)];
        typename std::aligned_storage<sizeof(T), alignof(T)>::type *mData = nullptr;


    }; // class small vector

    template<typename T, size_t N>
    void swap(small_vector<T, N> &a, small_vector<T, N> &b) { //swap two vectors using small_vector swap
        a.swap(b);
    }
} // namespace mpc

#endif // MPC_SMALL_VECTOR