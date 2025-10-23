#pragma once

#include "MyVector.h"
#include "MyPair.h"
#include "HashMap.h"

#include <cmath>

using namespace std;

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