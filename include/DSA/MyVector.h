#pragma once

template <typename T>
class MyVector {
private:
    T* _data;
    size_t _size;
    size_t _capacity;

    void reallocate();

public:
    // constructors & destructor
    MyVector(); // default
    MyVector(const MyVector& v); // copy
    MyVector(const int& n);
    MyVector(size_t n, const T& val = T());
    ~MyVector();

    // push, pop
    void push_back(const T& v);
    void pop_back();

    // operator
    inline T& operator[](size_t index);
    inline const T& operator[](size_t index) const;
    MyVector& operator=(const MyVector& v);

    // checker
    inline size_t size() const;
    inline bool empty() const;
    void clear();
    void resize(size_t newSize);

    // iterator
    inline T* begin();
    inline const T* begin() const;
    inline T* end();
    inline const T* end() const;
    inline T& back();
    inline const T& back() const;
    inline T& front();
    inline const T& front() const;

    void insert(size_t pos, const T& element);
    void erase(T* pos);
};

// DEFINITIONS

template <typename T>
void MyVector<T>::reallocate() {
    size_t newCapacity = _capacity == 0 ? 1 : _capacity * 2;
    T* tmp = new T[newCapacity];
    for (size_t i = 0; i < _size; ++i)
        tmp[i] = _data[i];
    delete[] _data;
    _data = tmp;
    _capacity = newCapacity;
}

template <typename T>
MyVector<T>::MyVector() : _data(nullptr), _size(0), _capacity(0) {}

template <typename T>
MyVector<T>::MyVector(const MyVector& v) : _size(v._size), _capacity(v._capacity) {
    _data = new T[_capacity];
    for (size_t i = 0; i < _size; ++i)
        _data[i] = v._data[i];
}

template <typename T>
MyVector<T>::MyVector(const int& n) : _capacity(n), _size(n) {
    _data = new T[_capacity];
    for (int i = 0; i < _size; ++i)
        _data[i] = T();
}

template <typename T>
MyVector<T>::MyVector(size_t n, const T& val) : _capacity(n), _size(n) {
    _data = new T[_capacity];
    for (size_t i = 0; i < _size; ++i)
        _data[i] = val;
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] _data;
    _data = nullptr;
}

template <typename T>
void MyVector<T>::push_back(const T& v) {
    if (_size == _capacity)
        reallocate();
    _data[_size++] = v;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (_size > 0) --_size;
}

template <typename T>
inline T& MyVector<T>::operator[](size_t i) {
    return _data[i];
}

template <typename T>
inline const T& MyVector<T>::operator[](size_t i) const {
    return _data[i];
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& v) {
    if (this != &v) {
        delete[] _data;
        _capacity = v._capacity;
        _size = v._size;
        _data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i)
            _data[i] = v._data[i];
    }
    return *this;
}

template <typename T>
inline size_t MyVector<T>::size() const {
    return _size;
}

template <typename T>
inline bool MyVector<T>::empty() const {
    return _size == 0;
}

template <typename T>
void MyVector<T>::clear() {
    _size = 0;
}

template <typename T>
void MyVector<T>::resize(size_t newSize) {
    if (newSize > _capacity) {
        size_t newCap = newSize > (_capacity == 0 ? 1 : _capacity * 2) ? newSize : (_capacity == 0 ? 1 : _capacity * 2);
        T* newData = new T[newCap];
        for (size_t i = 0; i < _size; ++i)
            newData[i] = _data[i];
        delete[] _data;
        _data = newData;
        _capacity = newCap;
    }
    for (size_t i = _size; i < newSize; ++i)
        _data[i] = T();
    _size = newSize;
}

template <typename T>
inline T* MyVector<T>::begin() {
    return _data;
}

template <typename T>
inline const T* MyVector<T>::begin() const {
    return _data;
}

template <typename T>
inline T* MyVector<T>::end() {
    return _data + _size;
}

template <typename T>
inline const T* MyVector<T>::end() const {
    return _data + _size;
}

template <typename T>
inline T& MyVector<T>::back() {
    return _data[_size - 1];
}

template <typename T>
inline const T& MyVector<T>::back() const {
    return _data[_size - 1];
}

template <typename T>
inline T& MyVector<T>::front() {
    return _data[0];
}

template <typename T>
inline const T& MyVector<T>::front() const {
    return _data[0];
}

template <typename T>
void MyVector<T>::insert(size_t pos, const T& v) {
    if (pos > _size) return;
    if (_size == _capacity) reallocate();
    if (pos == _size) {
        _data[_size++] = v;
        return;
    }
    for (size_t i = _size; i > pos; --i)
        _data[i] = _data[i - 1];
    _data[pos] = v;
    ++_size;
}

template <typename T>
void MyVector<T>::erase(T* pos) {
    if (pos < _data || pos >= _data + _size) return;
    if (pos == _data + _size - 1) {
        --_size;
        return;
    }
    for (T* it = pos; it != _data + _size - 1; ++it)
        *it = *(it + 1);
    --_size;
}