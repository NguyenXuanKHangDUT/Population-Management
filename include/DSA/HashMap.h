#pragma once

#include "MyPair.h"
#include "MyVector.h"
#include <string>

using namespace std;

template <typename KeyType, typename ValueType>
class HashMap {
private:
    struct Bucket {
        MyVector<MyPair<KeyType, ValueType>> data;
    };

    MyVector<Bucket> table;
    size_t bucketCnt;
    size_t curSize;

    // hash function----------
    template<typename K>
    size_t defaultHash(const K& key) const;

    size_t hashString(const string& key) const;

    // getIndex, use in many methods
    size_t getIndex(const KeyType& key) const;
    size_t getIndexImpl(const string& key) const;
    template<typename T>
    size_t getIndexImpl(const T& key) const;

public:
    // constructor
    HashMap(size_t bucketCnt = 16);

    // unordered_map methods:----------
    // insert
    void insert(const KeyType& key, const ValueType& value);

    class iterator; // forward declaration to organize code format

    // find
    iterator find(const KeyType& key);
    iterator find(const KeyType& key) const; // const version

    // erase
    void erase(const KeyType& key);

    // operator
    ValueType& operator[](const KeyType& key);

    void clear();

    // checkers-------------------------
    inline size_t size() const;
    inline bool empty() const;

    // iterators------------------------
    class iterator {
    private:
        size_t bucketID;
        size_t elmID;
        HashMap* unordered_mapPtr;
        // move to next valid position
        void jump();

    public:
        iterator(HashMap* m, size_t b, size_t e);
        MyPair<KeyType, ValueType>& operator*();
        MyPair<KeyType, ValueType>* operator->();
        iterator& operator++();
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };

    inline iterator begin();
    inline iterator end();
    inline iterator begin() const;
    inline iterator end() const;

    /*
    void rehash(size_t newBucketCnt);
    void reserve(size_t expected);
    */
};

template<typename KeyType, typename ValueType>
template<typename K>
size_t HashMap<KeyType, ValueType>::defaultHash(const K& key) const {
    return static_cast<size_t>(key * 2654435761u); // 2654435761u = 2^32 / φ, (φ is Golden rate)
}

template<typename KeyType, typename ValueType>
size_t HashMap<KeyType, ValueType>::hashString(const string& key) const {
    const size_t p = 131; // a low value prime number, but big enough to reduce collision
    const size_t mod = 1000000007; // Kaneki Ken's inspiration
    size_t hash = 0;

    for (char c : key)
        hash = (hash * p + static_cast<size_t>(c)) % mod;

    return hash;
}

template<typename KeyType, typename ValueType>
size_t HashMap<KeyType, ValueType>::getIndex(const KeyType& key) const {
    return getIndexImpl(key);
}

template<typename KeyType, typename ValueType>
size_t HashMap<KeyType, ValueType>::getIndexImpl(const string& key) const {
    return hashString(key) % bucketCnt;
}

template<typename KeyType, typename ValueType>
template<typename T>
size_t HashMap<KeyType, ValueType>::getIndexImpl(const T& key) const {
    return defaultHash(key) % bucketCnt;
}

// Constructor Definition
template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::HashMap(size_t bucketCnt) : bucketCnt(bucketCnt), curSize(0) {
    table.resize(bucketCnt);
}

// Public Method Definitions
template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    size_t idx = getIndex(key);

    for (size_t i = 0; i < table[idx].data.size(); ++i)
        if (table[idx].data[i].first == key) {
            table[idx].data[i].second = value;
            return;
        }

    table[idx].data.push_back(MyPair<KeyType, ValueType>(key, value));
    ++curSize;
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::find(const KeyType& key) {
    size_t idx = getIndex(key);
    for (size_t i = 0; i < table[idx].data.size(); ++i)
        if (table[idx].data[i].first == key)
            return iterator(this, idx, i);
    return end();
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::find(const KeyType& key) const { // const version
    size_t idx = getIndex(key);
    for (size_t i = 0; i < table[idx].data.size(); ++i)
        if (table[idx].data[i].first == key)
            return iterator(const_cast<HashMap*>(this), idx, i);
    return end();
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::erase(const KeyType& key) {
    size_t idx = getIndex(key);
    auto& bucket = table[idx].data;
    for (size_t i = 0; i < bucket.size(); ++i)
        if (bucket[i].first == key) {
            bucket.erase(bucket.begin()+i);
            --curSize;
            return;
        }
}

template <typename KeyType, typename ValueType>
ValueType& HashMap<KeyType, ValueType>::operator[](const KeyType& key) {
    size_t idx = getIndex(key);
    for (size_t i = 0; i < table[idx].data.size(); ++i)
        if (table[idx].data[i].first == key)
            return table[idx].data[i].second;
    table[idx].data.push_back(MyPair<KeyType, ValueType>(key, ValueType()));
    ++curSize;
    return table[idx].data.back().second;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::clear() {
    for (size_t i = 0; i < bucketCnt; ++i)
        table[i].data.clear();
    curSize = 0;
}

// Checkers Definitions
template <typename KeyType, typename ValueType>
inline size_t HashMap<KeyType, ValueType>::size() const { return curSize;}

template <typename KeyType, typename ValueType>
inline bool HashMap<KeyType, ValueType>::empty() const { return curSize == 0;}

// Iterator Definitions (outside class, but inside namespace)
template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::iterator::jump() {
    while (bucketID < unordered_mapPtr->table.size() && unordered_mapPtr->table[bucketID].data.size() == 0) {
        ++bucketID; elmID = 0;
    }
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::iterator::iterator(HashMap* m, size_t b, size_t e) : unordered_mapPtr(m), bucketID(b), elmID(e) {
    jump();
}

template <typename KeyType, typename ValueType>
MyPair<KeyType, ValueType>& HashMap<KeyType, ValueType>::iterator::operator*() {
    return unordered_mapPtr->table[bucketID].data[elmID];
}

template <typename KeyType, typename ValueType>
MyPair<KeyType, ValueType>* HashMap<KeyType, ValueType>::iterator::operator->() {
    return &unordered_mapPtr->table[bucketID].data[elmID];
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::iterator& HashMap<KeyType, ValueType>::iterator::operator++() {
    ++elmID;
    if (bucketID < unordered_mapPtr->table.size() && elmID >= unordered_mapPtr->table[bucketID].data.size()) {
        ++bucketID; elmID = 0;
        jump();
    }
    return *this;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::iterator::operator==(const iterator& other) const {
    return unordered_mapPtr == other.unordered_mapPtr &&
           bucketID == other.bucketID &&
           elmID == other.elmID;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

// Begin/End 
template <typename KeyType, typename ValueType>
inline typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::begin() {
    return iterator(this, 0, 0);
}

template <typename KeyType, typename ValueType>
inline typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::end() {
    return iterator(this, table.size(), 0);
}

template <typename KeyType, typename ValueType>
inline typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::begin() const {
    return iterator(const_cast<HashMap*>(this), 0, 0);
}

template <typename KeyType, typename ValueType>
inline typename HashMap<KeyType, ValueType>::iterator HashMap<KeyType, ValueType>::end() const {
    return iterator(const_cast<HashMap*>(this), table.size(), 0);
}

/*
// The rehash and reserve implementations remain commented out as in the original code.

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::rehash(size_t newBucketCnt) {
    if (newBucketCnt < bucketCnt)
        return;

    MyVector<Bucket> newTable;
    newTable.resize(newBucketCnt);

    for (size_t i = 0; i < bucketCnt; ++i) {
        for (size_t j = 0; j < table[i].data.size(); ++j) {
            const KeyType& key = table[i].data[j].first;
            const ValueType& value = table[i].data[j].second;

            size_t idx = getIndexImpl(key) % newBucketCnt;
            newTable[idx].data.push_back(MyPair<KeyType, ValueType>(key, value));
        }
    }

    table = newTable;
    bucketCnt = newBucketCnt;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::reserve(size_t expected) {
    const double maxLoadFactor = 1.0; // same like STL
    size_t require = static_cast<size_t>(expected / maxLoadFactor) + 1;

    if (require > bucketCnt)
        rehash(require);
}
*/