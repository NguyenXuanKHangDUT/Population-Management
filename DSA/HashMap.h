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
    size_t defaultHash(const K& key) const {
        return static_cast<size_t>(key * 2654435761u); // 2654435761u = 2^32 / φ, (φ is Golden rate)
    }
    size_t hashString(const string& key) const {
        const size_t p = 131; // a low value prime number, but big enough to reduce collision
        const size_t mod = 1e9 + 7; // Kaneki Ken's inspiration
        size_t hash = 0;

        for (char c : key)
            hash = (hash * p + static_cast<size_t>(c)) % mod;
        
        return hash;
    }
    
    // getIndex, use in many methods
    size_t getIndex(const KeyType& key) const { return getIndexImpl(key);}
    size_t getIndexImpl(const string& key) const { return hashString(key) % bucketCnt;}
    template<typename T>
    size_t getIndexImpl(const T& key) const { return defaultHash(key) % bucketCnt;}

public:
    // constructor
    HashMap(size_t bucketCnt = 16) : bucketCnt(bucketCnt), curSize(0) {
        table.resize(bucketCnt);
    }

    // unordered_map methods:----------
    // insert
    void insert(const KeyType& key, const ValueType& value) {
        size_t idx = getIndex(key);

        for (size_t i = 0; i < table[idx].data.size(); ++i) 
            if (table[idx].data[i].first == key) {
                table[idx].data[i].second = value;
                return;
            }
        
        table[idx].data.push_back(MyPair<KeyType, ValueType>(key, value));
        ++curSize;
    }
    class iterator; // forward declaration to organize code format
    // find
    iterator find(const KeyType& key) {
        size_t idx = getIndex(key);
        for (size_t i = 0; i < table[idx].data.size(); ++i)
            if (table[idx].data[i].first == key)
                return iterator(this, idx, i);
        return end();
    }
    iterator find(const KeyType& key) const { // const version
        size_t idx = getIndex(key);
        for (size_t i = 0; i < table[idx].data.size(); ++i)
            if (table[idx].data[i].first == key)
                return iterator(const_cast<HashMap*>(this), idx, i);
        return end();
    }
    // erase
    void erase(const KeyType& key) {
        size_t idx = getIndex(key);
        auto& bucket = table[idx].data;
        for (size_t i = 0; i < bucket.size(); ++i) 
            if (bucket[i].first == key) {
                bucket.erase(i);
                --curSize;
                return;
            }
    }
    // access operator
    ValueType& operator[](const KeyType& key) {
        size_t idx = getIndex(key);
        for (size_t i = 0; i < table[idx].data.size(); ++i)
            if (table[idx].data[i].first == key)
                return table[idx].data[i].second;
        table[idx].data.push_back(MyPair<KeyType, ValueType>(key, ValueType()));
        ++curSize;
        return table[idx].data.back().second;
    }
    // clear
    void clear() {
        for (size_t i = 0; i < bucketCnt; ++i)
            table[i].data.clear();
        curSize = 0;
    }

    // checkers-------------------------
    size_t size() const { return curSize;}
    bool empty() const { return curSize == 0;}

    // iterators------------------------
    class iterator {
    private:
        size_t bucketID;
        size_t elmID;
        HashMap* unordered_mapPtr;
        // move to next valid position
        void jump() {
            while (bucketID < unordered_mapPtr->table.size() && unordered_mapPtr->table[bucketID].data.size() == 0) {
                ++bucketID; elmID = 0;
            }
        }
    
    public:
        iterator(HashMap* m, size_t b, size_t e) : unordered_mapPtr(m), bucketID(b), elmID(e) {
            jump();
        }
        MyPair<KeyType, ValueType>& operator*() {
            return unordered_mapPtr->table[bucketID].data[elmID];
        }
        MyPair<KeyType, ValueType>* operator->() {
            return &unordered_mapPtr->table[bucketID].data[elmID];
        }
        iterator& operator++() {
            ++elmID;
            if (bucketID < unordered_mapPtr->table.size() && elmID >= unordered_mapPtr->table[bucketID].data.size()) {
                ++bucketID; elmID = 0;
                jump();
            }
            return *this;
        }
        bool operator==(const iterator& other) const {
            return unordered_mapPtr == other.unordered_mapPtr &&
                   bucketID == other.bucketID &&
                   elmID == other.elmID;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    iterator begin() { return iterator(this, 0, 0); }
    iterator end() { return iterator(this, table.size(), 0); }
    iterator begin() const { return iterator(const_cast<HashMap*>(this), 0, 0); }
    iterator end() const { return iterator(const_cast<HashMap*>(this), table.size(), 0); }


    /*
    void rehash(size_t newBucketCnt) {
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

    void reserve(size_t expected) {
        const double maxLoadFactor = 1.0; // tương tự STL
        size_t require = static_cast<size_t>(expected / maxLoadFactor) + 1;

        if (require > bucketCnt)
            rehash(require);
    }
    */
};