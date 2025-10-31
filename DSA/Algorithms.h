#pragma once

#include "MyVector.h"
#include "MyPair.h"
#include "HashMap.h"

#include <string>
#include <cmath>


using namespace std;

#define ll long long

template <typename T> 
void insertionSort(int l, int r, MyVector<T>& a, bool (*cmp)(const T&, const T&)) {
    for (int i = l+1; i <= r; ++i) {
        T key = a[i];
        int j = i-1;
        for (; j >= l && cmp(key, a[j]); --j)
            a[j+1] = a[j];
        a[j+1] = key;
    }
}

template <typename T>
void heapify(T* a, int n, int i, bool (*cmp)(const T&, const T&)) {
    int largest = i;
    int l = 2*i+1;
    int r = 2*i+2;

    if (l < n && cmp(a[largest], a[l]))
        largest = l;
    if (r < n && cmp(a[largest], a[r]))
        largest = r;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapify(a, n, largest, cmp);
    }
}

template <typename T>
void heapSort(int l, int r, MyVector<T>& a, bool (*cmp)(const T&, const T&)) {
    if (l >= r) return;
    int n = r-l+1;
    T* arr = a.begin()+l;

    for (int i = n/2-1; i >= 0; --i)
        heapify(arr, n, i, cmp);

    for (int i = n-1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, cmp);
    }
}

template <typename T>
int medianOf3(int i, int j, int k, MyVector<T>& a, bool (*cmp)(const T&, const T&)) {
    // cmp(x, y) == true means x < y
    if (cmp(a[i], a[j])) {
        if (cmp(a[j], a[k])) return j;     // i < j < k
        else if (cmp(a[i], a[k])) return k; // i < k < j
        else return i;                     // k < i < j
    } else {
        if (cmp(a[i], a[k])) return i;     // j < i < k
        else if (cmp(a[j], a[k])) return k; // j < k < i
        else return j;                     // k < j < i
    }
}

template <typename T>
void introSort(int l, int r, MyVector<T>& a, int depthLim, bool (*cmp)(const T&, const T&)) {
    while (l < r) {
        int n = r-l+1;
        if (n < 16) {
            insertionSort(l, r, a, cmp); return;
        }
        if (depthLim == 0) {
            heapSort(l, r, a, cmp); return;
        }

        int pos = medianOf3(l, (l+r)/2, r, a, cmp);
        if (pos != r) {
            swap(a[pos], a[r]);
            pos = r;
        }
        T pivot = a[pos];

        // Dutch National Flag partition
        int lt = l, gt = r, i = l;
        while (i <= gt) {
            if (cmp(a[i], pivot)) swap(a[lt++], a[i++]);
            else if (cmp(pivot, a[i])) swap(a[i], a[gt--]);
            else i++;
        }
        // [l, lt-1] < pivot, [lt, gt] == pivot, [gt+1, r] > pivot

        --depthLim;
        // Tail recursion optimization
        if (lt-l < r-gt) {
            introSort(l, lt-1, a, depthLim, cmp);
            l = gt+1;
        } else {
            introSort(gt+1, r, a, depthLim, cmp);
            r = lt-1;
        }
    }
}

template <typename T>
void introSort(MyVector<T>& a, bool (*cmp)(const T&, const T&)) {
    if (a.size() <= 1) return;
    int depthLim = 2*floor(log2(a.size()));
    introSort(0, a.size()-1, a, depthLim, cmp);
}

/*
template <typename T>
T MyMax(const T& a, const T& b, bool (*cmp)(const T&, const T&)) {
    return cmp(a, b) ? b : a;
}
template <typename T>
T MyMin(const T& a, const T& b, bool (*cmp)(const T&, const T&)) {
    return cmp(a, b) ? a : b;
}
*/

inline int getCharValue(char c) {
    c = tolower(c); // or toupper(c)
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    return 0;
}

inline string makeUniqueID(const string& fname, const string& bd, bool gd, const string& rg) {
    string ID;

    // region code
    if (rg == "Sina") ID = "11";
    else if (rg == "Rose") ID = "10";
    else ID = "01";

    // gender code
    if (gd) ID += "1";
    else ID += "0";

    // const hashing parameters
    const ll P = 37;
    const ll M = 1000000007;
    const ll C1 = 1315423911; // Jenkins's hash
    const ll C2 = 2654435761; // Knuth's const
    const ll MOD9 = 1000000000;

    // removw " " from name
    string fullname = fname;
    for (int i = 0; i < fullname.size(); ++i) {
        if (fullname[i] == ' ') {
            fullname.erase(i, 1);
            --i;
        }
    }
    // hash fullname
    ll name_hash = 0;
    for (char c : fullname)
        name_hash = (name_hash * P + getCharValue(c)) % M;
    
    // hash birthday dd/mm/yyyy
    int pos1 = bd.find('/');
    int pos2 = bd.find('/', pos1+1);

    ll day = stoll(bd.substr(0, pos1));
    ll month = stoll(bd.substr(pos1+1, pos2-pos1-1));
    ll year = stoll(bd.substr(pos2+1));
    ll birth_hash = day*1000000 + month*10000 + year;

    ll final_hash = ((name_hash * C1) + (birth_hash * C2)) % MOD9;

    // pad leading zeros to make it 9 digits
    string hash_str = to_string(final_hash);
    while (hash_str.size() < 9) hash_str = '0' + hash_str;

    ID += hash_str;
    return ID;
}