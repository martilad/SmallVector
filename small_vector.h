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

    small_vector(const small_vector& other){
        // todo: kopírovací konstruktor. Po konstrukci vektoru bude jeho obsah stejný, jako obsah vektoru other. Tj., bude v sobě mít uložený stejný počet prvků a prvky na stejných pozicích budou mít stejné hodnoty (z hledika jejich sémantiky).
        // Prvky nového vektoru musí být zkonstruovány pomocí kopírovací sémantiky, tj. vzniknou jako kopie příslušného prvku vektoru other.
        // Pro volání toho konstruktoru musí hodnotový typ vektoru kopírovací sémantiku podporovat.
    }

    small_vector(small_vector&& other){
        // todo: přesouvací konstruktor. Po konstrukci vektoru bude jeho obsah stejný, jako byl obsah vektoru other před touto operací. Obsah vektoru other se bude po této operaci nacházet ve stavu prázdného vektoru.
    }

    small_vector(std::initializer_list<T> init){
        // todo: konverzní konstruktor, který umožňje naplnit obsah vektoru při jeho inicializaci, a to tak, že zkopíruje prvky inicializačního seznamu init do vznikajícího vektoru.
        // Pro volání toho konstruktoru musí hodnotový typ vektoru podporovat kopírovací sémantiku.
    }

    ~small_vector(){
        // todo: destruktor. Korektně destruuje všechny prvky vektoru a případně uvolní dynamicky alokovanou paměť.
        // Destruktor sám o sobě nesmí vyhodit výjimku.
    }

    small_vector& operator=(const small_vector& other){
        // todo: kopírovací přiřazovací operátor. Po aplikaci operátoru bude obsah vektoru stejný, jako obsah vektoru other.
        // Vrací referenci na sebe sama.
    }

    small_vector& operator=(small_vector&& other){
        // todo: přesouvací přiřazovací operátor. Po aplikaci operátoru bude obsah vektoru stejný, jako byl obsah vektoru other před touto operací. Obsah vektoru other se bude po této operaci nacházet ve stavu prázdného vektoru.
        // Vrací referenci na sebe sama.
    }

    size_t size() const{
        return this->mSize;
    }

    size_t capacity() const {
        return this->mCapacity;
    }

    T* data(){};
    //const_pointer data() const — vrátí ukazatel na první prvek vektoru (varianta pro konstantní vektor).


    void clear (){
        // todo: vymaže obsah vektoru, tj. destruuje všechny jeho prvky, a to v klesajícím pořadí jejich pozic (indexů).
    }

    void reserve(size_t capacity){
        // todo: potenciálně zvýší kapacitu vektoru na hodnotu parametru capacity.
        //    Pokud je capacity menší nebo rovno aktuální kapacitě, nemá tato funkce žádný efekt.
        //    V opačném případě funkce zvýší kapacitu na capacity prvků, tj. naalokuje nový buffer a do něj přesune stávající obsah vektoru.
        //    [Update 3.11.2019] Pokud hodnotový typ nemá noexcept přesouvací konstruktor,
        //    dostane před přesouvacím konstruktorem při přesunu prvků přednost kopírovací konstruktor (viz std::move_if_noexcept).
        //
    }
    void push_back(const T& value) {
        // todo: vloží na konec vektoru nový prvek, který vznikne kopírováním obsahu z parametru value.
        //    Pro volání této funkce musí hodnotový typ vektoru podporovat kopírovací sémantiku.
    }
    void push_back(T&& value) {
        // todo: vloží na konec vektoru nový prvek, který vnikne přesunem obsahu z parametru value.
    };
    template <typename... Ts> void emplace_back(Ts&&... vs){
        // todo: vloží na konec vektoru nový prvek, pro jehož konstrukci budou jako argumenty předány parametry vs..., a to pomocí techniky perfect forwarding.v
    }


    void resize(size_t size, const T& value = T()){
        // todo: změna velikosti vektoru.
        //    Pokud je size rovno aktuální velikosti vektoru, nemá tato funkce žádný efekt.
        //    Pokud je size menší než aktuální velikost vektoru, jsou destruovány prvky na pozicích size až size() - 1, a to v klesajícím pořadí jejich pozic. Kapacita vektoru je zachována.
        //    Pokud je size větší než aktuální velikost vektoru, v případě potřeby zvýší kapacitu vektoru tak, aby se do něj vešlo size prvků, a poté přidá na konec vektoru size - size() prvků jako kopie parametru value.
        //    Pro volání této funkce musí hodnotový typ vektoru podporovat kopírovací sémantiku.
        //    V rámci tohoto textu znamená pojem „velikost vektoru“ počet jeho prvků, nikoliv velikost objektu typu mpc::small_vector v bytech (zjistitelnou pomocí operátoru sizeof).
    }
    T& operator[](size_t index){
        // todo: vrací referenci na prvek vektoru na pozici index.
    }
    const T& operator[](size_t index) const {
        // todo: vrací referenci na prvek vektoru na pozici index (varianta pro konstantní vektor)
    }
    iterator begin() {
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
    }
    void swap(small_vector& other) {
        // todo: prohodí obsah vektoru s vektorem other.v
    }

private:
    size_t mCapacity{};
    size_t mSize{};
    T _data_cache[N];
    T* _data = nullptr;


}; // class small vector
template <typename T, size_t N> void swap(small_vector<T,N>& a, small_vector<T,N>& b){
    //todo: prohození obsahu vektorů a a b.
}
} // namespace mpc

#endif // MPC_SMALL_VECTOR