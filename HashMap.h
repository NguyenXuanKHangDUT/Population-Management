#pragma once
#include "MyVector.h"
#include <utility>   // pair
#include <string>    // string

template <typename K, typename V>
class HashMap {
private:
    struct Bucket {
        MyVector<std::pair<K, V>> data;
    };

    MyVector<Bucket> table;
    size_t bucketCount;
    size_t currentSize;

    size_t hashString(const std::string& s) const {
        const size_t p = 131;
        const size_t mod = 1000000007;
        size_t hash = 0;
        for (char c : s)
            hash = (hash * p + static_cast<unsigned char>(c)) % mod;
        return hash;
    }

    template <typename T>
    size_t defaultHash(const T& key) const {
        return static_cast<size_t>(key * 2654435761u); // Knuth's multiplicative hash
    }

    size_t getIndex(const K& key) const {
        return getIndexImpl(key);
    }

    size_t getIndexImpl(const std::string& key) const {
        return hashString(key) % bucketCount;
    }

    template <typename T>
    size_t getIndexImpl(const T& key) const {
        return defaultHash(key) % bucketCount;
    }

public:
    // ---- iterator ----
    class iterator {
    private:
        MyVector<Bucket>* table;
        size_t bucketIndex;
        size_t elemIndex;

        void moveToNextValid() {
            while (bucketIndex < table->size() &&
                   (*table)[bucketIndex].data.size() == 0) {
                ++bucketIndex;
                elemIndex = 0;
            }
        }

    public:
        iterator(MyVector<Bucket>* t, size_t b, size_t e)
            : table(t), bucketIndex(b), elemIndex(e) {
            moveToNextValid();
        }

        std::pair<K, V>& operator*() {
            return (*table)[bucketIndex].data[elemIndex];
        }

        std::pair<K, V>* operator->() {
            return &(*table)[bucketIndex].data[elemIndex];
        }

        iterator& operator++() {
            ++elemIndex;
            if (bucketIndex < table->size() &&
                elemIndex >= (*table)[bucketIndex].data.size()) {
                ++bucketIndex;
                elemIndex = 0;
                moveToNextValid();
            }
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return table != other.table ||
                   bucketIndex != other.bucketIndex ||
                   elemIndex != other.elemIndex;
        }
    };

    // ---- const_iterator ----
    class const_iterator {
    private:
        const MyVector<Bucket>* table;
        size_t bucketIndex;
        size_t elemIndex;

        void moveToNextValid() {
            while (bucketIndex < table->size() &&
                   (*table)[bucketIndex].data.size() == 0) {
                ++bucketIndex;
                elemIndex = 0;
            }
        }

    public:
        const_iterator(const MyVector<Bucket>* t, size_t b, size_t e)
            : table(t), bucketIndex(b), elemIndex(e) {
            moveToNextValid();
        }

        const std::pair<K, V>& operator*() const {
            return (*table)[bucketIndex].data[elemIndex];
        }

        const std::pair<K, V>* operator->() const {
            return &(*table)[bucketIndex].data[elemIndex];
        }

        const_iterator& operator++() {
            ++elemIndex;
            if (bucketIndex < table->size() &&
                elemIndex >= (*table)[bucketIndex].data.size()) {
                ++bucketIndex;
                elemIndex = 0;
                moveToNextValid();
            }
            return *this;
        }

        bool operator!=(const const_iterator& other) const {
            return table != other.table ||
                   bucketIndex != other.bucketIndex ||
                   elemIndex != other.elemIndex;
        }
    };

    // ---- constructor ----
    HashMap(size_t buckets = 16) : bucketCount(buckets), currentSize(0) {
        table.resize(bucketCount);
    }

    // ---- insert ----
    void insert(const K& key, const V& value) {
        size_t idx = getIndex(key);
        for (auto& kv : table[idx].data) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        table[idx].data.push_back({key, value});
        ++currentSize;
    }

    // ---- find ----
    iterator find(const K& key) {
        size_t idx = getIndex(key);
        for (size_t i = 0; i < table[idx].data.size(); ++i)
            if (table[idx].data[i].first == key)
                return iterator(&table, idx, i);
        return end();
    }

    // ---- const find ----
    const_iterator find(const K& key) const {
        size_t idx = getIndex(key);
        for (size_t i = 0; i < table[idx].data.size(); ++i)
            if (table[idx].data[i].first == key)
                return const_iterator(&table, idx, i);
        return cend();
    }

    // ---- erase ----
    void erase(const K& key) {
        size_t idx = getIndex(key);
        auto& bucket = table[idx].data;
        for (size_t i = 0; i < bucket.size(); ++i) {
            if (bucket[i].first == key) {
                bucket.erase(i);
                --currentSize;
                return;
            }
        }
    }

    // ---- operator[] ----
    V& operator[](const K& key) {
        size_t idx = getIndex(key);
        for (auto& kv : table[idx].data)
            if (kv.first == key)
                return kv.second;
        table[idx].data.push_back({key, V()});
        ++currentSize;
        return table[idx].data.back().second;
    }

    // ---- iterators ----
    iterator begin() { return iterator(&table, 0, 0); }
    iterator end() { return iterator(&table, table.size(), 0); }
    const_iterator begin() const { return const_iterator(&table, 0, 0); }
    const_iterator end() const { return const_iterator(&table, table.size(), 0); }
    const_iterator cbegin() const { return const_iterator(&table, 0, 0); }
    const_iterator cend() const { return const_iterator(&table, table.size(), 0); }

    // ---- size ----
    size_t size() const { return currentSize; }

    // ---- clear ----
    void clear() {
        for (auto& bucket : table)
            bucket.data.clear();
        currentSize = 0;
    }
};