#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;

template <typename T, typename Comparator>
class flat_set {
private:
    vector<T> vectorOfData;
    Comparator comparator;
public:
    using iterator = typename vector<T>::iterator ;
    using const_iterator =typename vector<T>::const_iterator;
    using size_type = size_t;
    using value_type =T;

    // Speciální member funkce se chovají běžným stylem
    flat_set(): vectorOfData(), comparator(){};
    flat_set(Comparator const& cmp):flat_set(){ comparator=cmp;};
    flat_set(flat_set const& rhs){
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        swap(temp);
    }
    flat_set(flat_set && rhs):flat_set(rhs){};
    flat_set& operator=(flat_set const& rhs){
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        return *this->swap(temp);
    }
    flat_set& operator=(flat_set && rhs){
        return *this->swap(rhs);
    }
    ~flat_set();

    // Konstruktory co vytvoří flat_set z prvků definovaných jako
    // [first, last). Alokují pouze jednou pokud je to možné.
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last): flat_set() {
        vectorOfData.reserve(std::distance(first, last));
        insert(first, last);
    }
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last, Comparator const& cmp) : flat_set() {
        comparator = cmp;
        vectorOfData.reserve(std::distance(first, last));
        insert(first, last);
    };


    // Vloží prvek do setu, buďto pomocí kopie, nebo pomocí přesunu.
    std::pair<iterator, bool> insert(T const& v){
        iterator found = find(v);
        if (found != end()) {
            return std::pair<iterator, bool>(found, false);
        }
        iterator place =upper_bound(begin(), end(), v, comparator);
        iterator insert_iterator = vectorOfData.insert(place, v);
        return pair<iterator, bool>(insert_iterator, true);
    }
    std::pair<iterator, bool> insert(T&& v){
        iterator found = find(v);
        if (found != end()) {
            return std::pair<iterator, bool>(found, false);
        }
        iterator place = upper_bound(begin(), end(), v, comparator);
        iterator insert_iterator = vectorOfData.insert(place, std::move(v));
        return pair<iterator, bool>(insert_iterator, true);
    }
    // Vloží prvky z [first, last), alokuje pouze jednou pokud je to možné
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last){
        while(first!=last){
            insert(first);
            first++;
        }
    }

    // Smaže prvek na který ukazuje i, vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator i){
        return vectorOfData.erase(i);
    }
    // Smaže všechny prvky z [from, to), vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator from, const_iterator to){
        return vectorOfData.erase(from,to);
    }
    // Iterátory předané dovnitř erase odkazují dovnitř setu.

    // Smaže prvek rovný klíči pokud existuje.
    // Vrátí kolik prvků bylo smazáno
    size_type erase(value_type const& key){
        iterator temp=find(key);
        vectorOfData.erase(temp);
        return 1;
    }

    // Běžné funkce k vytvoření iterátorů
    iterator begin() noexcept{
        return vectorOfData.begin();
    }
    iterator end() noexcept{
        return vectorOfData.end();
    }
    const_iterator begin() const noexcept{
        return vectorOfData.begin();
    }
    const_iterator end() const noexcept{
        return vectorOfData.end();
    }
    const_iterator cbegin() const noexcept{
        return vectorOfData.cbegin();
    }
    const_iterator cend() const noexcept{
        return vectorOfData.cend();
    }

    bool empty() const{
        if(vectorOfData.empty()){
            return true;
        }
        return false;
    }
    size_type size() const{
        return vectorOfData.size();
    }
    size_type capacity() const{
        return vectorOfData.capacity();
    }

    // Zajistí, aby se do instance flat_setu dalo vložit c prvků
    // aniž by byla potřeba realokace
    void reserve(size_type c){
        return vectorOfData.reserve(c);
    }

    // Vymaže všechny prvky ze setu
    void clear(){
        return vectorOfData.clear();
    }

    // Vrátí iterátor ukazující na prvek ekvivalentní s v, nebo end(),
    // pokud takový prvek není uvnitř setu
    iterator find(T const& v){
        int size=vectorOfData.size();
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(temp==v){
                return temp;
            }else{
                temp++;
            }
        }
        return vectorOfData.end();
    }
    const_iterator find(T const& v) const{
        int size=vectorOfData.size();
        const_iterator fin;
        iterator temp=vectorOfData.begin();
        for(int i=0;i<size;i++){
            if(temp==v){
                fin=temp;
                return fin;
            }else{
                temp++;
            }
        }
        return vectorOfData.end();
    }

    // Vrátí iterátor k prvnímu prvku, který není menší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator lower_bound(T const& t){
        iterator temp;
        int size=vectorOfData.size();
        for(int i=0;i<size;i++){
            if(temp>=t){
                return temp;
            }else{
                temp++;
            }
        }
        return vectorOfData.end();
    }
    const_iterator lower_bound(T const& t) const{
        const_iterator fin;
        int size=vectorOfData.size();
        iterator temp;
        for(int i=0;i<size;i++){
            if(temp>=t){
                fin=temp;
                return fin;
            }else{
                temp++;
            }
        }
        return vectorOfData.end();
    }

    // Vrátí iterátor k prvnímu prvku, který je větší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator upper_bound(T const& t){
        iterator temp=lower_bound(t);
        if(temp==vectorOfData.end()){
            return temp;
        }
        return temp++;
    }
    const_iterator upper_bound(T const& t) const{
        const_iterator fin=lower_bound(t);
        if(fin==vectorOfData.end()){
            return fin;
        }
        return fin++;
    }

    // Prohodí obsah dvou setů (včetně komparátoru)
    void swap(flat_set& o){
        swap(vectorOfData,o.vectorOfData);
        swap(comparator,o.comparator);
    }
};

// porovnání probíhá lexikograficky
bool operator==(flat_set const& a, flat_set const& b){}
bool operator!=(flat_set const& a, flat_set const& b){

}
bool operator< (flat_set const& a, flat_set const& b){

}
bool operator<=(flat_set const& a, flat_set const& b){

}
bool operator>=(flat_set const& a, flat_set const& b){

}
bool operator> (flat_set const& a, flat_set const& b){

}

// Prohodí obsah dvou setů (včetně komparátoru)
template <typename T>
void swap(flat_set<T> const& a, flat_set<T> const& b){
    a.swap(b);
}