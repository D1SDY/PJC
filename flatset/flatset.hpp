#pragma once

#include <iostream>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;

template <typename T, typename Comparator = less<T> >
class flat_set {
private:
    Comparator comparator;
public:
    using iterator = typename vector<T>::iterator;
    using const_iterator = typename vector<T>::const_iterator;
    using size_type = size_t;
    using value_type = T;
    vector<T> vectorOfData;

    // Speciální member funkce se chovají běžným stylem
    flat_set() : vectorOfData(), comparator() { };
    flat_set(Comparator const& cmp) : flat_set() {
        comparator = cmp;
    };
    flat_set(flat_set const& rhs) {
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        swap(temp);
    }
    flat_set(flat_set && rhs) : flat_set(rhs) { };
    flat_set& operator=(flat_set const& rhs) {
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        this->swap(temp);
        return *this;
    }
    flat_set& operator=(flat_set && rhs) {
        this->swap(rhs);
        return *this;
    }
    ~flat_set() {};

    // Konstruktory co vytvoří flat_set z prvků definovaných jako
    // [first, last). Alokují pouze jednou pokud je to možné.
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last): flat_set() {
        vectorOfData.reserve(distance(first, last));
        insert(first, last);
    }
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last, Comparator const& cmp) : flat_set() {
        comparator = cmp;
        vectorOfData.reserve(distance(first, last));
        insert(first, last);
    };

    // Vloží prvek do setu, buďto pomocí kopie, nebo pomocí přesunu.
    pair<iterator, bool> insert(T const& item) {
        iterator found = find(item);
        if (found != vectorOfData.end()) {
            return pair<iterator, bool>(found, false);
        }else{
            iterator insert_iterator = vectorOfData.insert(std::upper_bound(begin(), vectorOfData.end(), item, comparator), item);
            return pair<iterator, bool>(insert_iterator, true);
        }

    }

    pair<iterator, bool> insert(T&& item) {
        iterator found = find(item);
        if (found != vectorOfData.end()) {
            return pair<iterator, bool>(found, false);
        }else{
            iterator insert_iterator = vectorOfData.insert(std::upper_bound(begin(), vectorOfData.end(), item, comparator),move(item));
            return pair<iterator, bool>(insert_iterator, true);
        }
    }

    // Vloží prvky z [first, last), alokuje pouze jednou pokud je to možné
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for(;;){
            if(first==last){
                break;
            }
            insert(*first);
            first++;
        }
    }

    // Smaže prvek na který ukazuje i, vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator i) {
        return vectorOfData.erase(i);
    };
    // Smaže všechny prvky z [from, to), vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator from, const_iterator to) {
        return vectorOfData.erase(from, to);
    };
    // Iterátory předané dovnitř erase odkazují dovnitř setu.

    // Smaže prvek rovný klíči pokud existuje.
    // Vrátí kolik prvků bylo smazáno
    size_type erase(value_type const& key) {
        int size=vectorOfData.size();
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if (comparator(*temp, key) == false && comparator(key, *temp) == false) {
                vectorOfData.erase(temp);
                return 1;
            }
            temp++;
        }
        return 0;
    }

    // Běžné funkce k vytvoření iterátorů
    iterator begin() noexcept {
        return vectorOfData.begin();
    };
    iterator end() noexcept {
        return vectorOfData.end();
    };
    const_iterator begin() const noexcept {
        return vectorOfData.begin();
    };
    const_iterator end() const noexcept {
        return vectorOfData.end();
    };
    const_iterator cbegin() const noexcept {
        return vectorOfData.cbegin();
    };
    const_iterator cend() const noexcept {
        return vectorOfData.cend();
    };

    bool empty() const {
        if(vectorOfData.empty()){
            return true;
        }
        return false;
    }

    size_type size() const {
        return vectorOfData.size();
    }

    size_type capacity() const {
        return vectorOfData.capacity();
    }

    // Zajistí, aby se do instance flat_setu dalo vložit c prvků
    // aniž by byla potřeba realokace
    void reserve(size_type c) {
        vectorOfData.reserve(c);
    }

    // Vymaže všechny prvky ze setu
    void clear() {
        vectorOfData.clear();
    }

    // Vrátí iterátor ukazující na prvek ekvivalentní s v, nebo end(),
    // pokud takový prvek není uvnitř setu
    iterator find(T const& v) {
        iterator temp = vectorOfData.begin();
        int size=vectorOfData.size();
        for(int i=0;i<size;i++){
            if (comparator(*temp, v) == false && comparator(v, *temp) == false) {
                return temp;
            }
            temp++;
        }
        return vectorOfData.end();
    }
    const_iterator find(T const& v) const {
        iterator temp = vectorOfData.begin();
        const_iterator fin;
        int size=vectorOfData.size();
        for(int i=0;i<size;i++){
            if (comparator(*temp, v) == false && comparator(v, *temp) == false) {
                fin=temp;
                return fin;
            }
            temp++;
        }
        return vectorOfData.end();
    }

    // Vrátí iterátor k prvnímu prvku, který není menší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator lower_bound(T const& t) {
        int size=vectorOfData.size();
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(*temp>=t){
                return temp;
            }
            temp++;
        }
        return vectorOfData.end();
    }
    const_iterator lower_bound(T const& t) const {
        int size=vectorOfData.size();
        const_iterator fin;
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(*temp>=t){
                fin=temp;
                return fin;
            }
            temp++;
        }
        return vectorOfData.end();
    }

    // Vrátí iterátor k prvnímu prvku, který je větší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator upper_bound(T const& t) {
        int size=vectorOfData.size();
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(*temp>t){
                return temp;
            }
            temp++;
        }
        return vectorOfData.end();
    }
    const_iterator upper_bound(T const& t) const {
        int size=vectorOfData.size();
        const_iterator fin;
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(*temp>t){
                fin=temp;
                return fin;
            }
            temp++;
        }
        return vectorOfData.end();
    }

    // Prohodí obsah dvou setů (včetně komparátoru)
    void swap(flat_set& o) {
        std::swap(vectorOfData, o.vectorOfData);
        std::swap(comparator, o.comparator);
    }
};

// porovnání probíhá lexikograficky

template <typename T>
bool operator==(flat_set<T> const& a, flat_set<T> const& b) {
    return distance(a.begin(), a.end()) == distance(b.begin(), b.end()) && equal(a.begin(), a.end(), b.begin());
}

template <typename T>
bool operator!=(flat_set<T> const& a, flat_set<T> const& b) {
    return !(a == b);
}

template <typename T>
bool operator< (flat_set<T> const& a, flat_set<T> const& b) {
    return a.vectorOfData<b.vectorOfData;
}

template <typename T>
bool operator<=(flat_set<T> const& a, flat_set<T> const& b) {
    return a.vectorOfData<=b.vectorOfData;
}

template <typename T>
bool operator>=(flat_set<T> const& a, flat_set<T> const& b) {
    return  a.vectorOfData>=b.vectorOfData;
}

template <typename T>
bool operator> (flat_set<T> const& a, flat_set<T> const& b) {
    return !(a<b);
}

// Prohodí obsah dvou setů (včetně komparátoru)
template <typename T>
void swap(flat_set<T> const& a, flat_set<T> const& b) {
    a.swap(b);
}