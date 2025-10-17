#pragma once

template <typename T>
class MyVector {
private:
    T* _data;
    size_t _size;
    size_t _capacity;

    void reallocate() {
        size_t newCapacity = _capacity == 0 ? 1 : _capacity * 2;
        T* tmp = new T[newCapacity];

        for (size_t i = 0; i < _size; ++i)
            tmp[i] = std::move(_data[i]);

        delete[] _data;
        _data = tmp;
        _capacity = newCapacity;
    }

public:
    // constructor & destructor
    MyVector() : _data(nullptr), _size(0), _capacity(0) {}
    MyVector(const MyVector& v) : _size(v._size), _capacity(v._capacity) {
        _data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i)
            _data[i] = v._data[i];
    }
    MyVector(const int& n) : _capacity(n), _size(n) {
        _data = new T[_capacity];
        for (int i = 0; i < _size; ++i) 
            _data[i] = T();
    }
    MyVector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size()) {
        _data = new T[_capacity];
        size_t i = 0;
        for (const T& element : init) 
            _data[i++] = element;
    }
    MyVector(size_t n, const T& val = T()) : _capacity(n), _size(n) {
        _data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i)
            _data[i] = val;
    }
    ~MyVector() {
        delete[] _data;
        _data = nullptr;
    }


    // push, pop
    void push_back(const T& element) {
        if (_size == _capacity)
            reallocate();
        _data[_size++] = element;
    }
    void pop_back() {
    if (_size > 0) --_size;
    }


    // operator
    T& operator[](size_t index) {
        return _data[index];
    }

    const T& operator[](size_t index) const {
        return _data[index];
    }
    MyVector& operator=(const MyVector& v) {
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
    

    // checker
    size_t size() const {
        return _size;
    }
    bool empty() const {
        return _size == 0;
    }
    void clear() {
        _size = 0;
    }
    void resize(size_t newSize) {
        if (newSize > _capacity) {
            size_t newCap = std::max(newSize, _capacity == 0 ? 1 : _capacity * 2);
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



    // iterator
    T* begin() {
        return _data;
    }
    const T* begin() const {
        return _data;
    }
    T* end() {
        return _data+_size;
    }
    const T* end() const {
        return _data+_size;
    }
    T& back() {
        return _data[_size - 1]; 
    }
    const T& back() const {
        return _data[_size - 1]; 
    }
    T& front() {
        return _data[0]; 
    }
    const T& front() const {
        return _data[0]; 
    }

    // some STL functions
    void reverse(T* l, T* r) { 
        while (l < r) {
            std::swap(*l, *r);
            ++l; --r;
        }
    }
    void insert(size_t pos, const T& element) {
        if (pos > _size) return;
        if (_size == _capacity) reallocate();

        if (pos == _size) {
            _data[_size++] = element;
            return;
        }

        for (size_t i = _size; i > pos; --i)
            _data[i] = std::move(_data[i-1]);
        _data[pos] = element;
        ++_size;
    }
    void erase(T* pos) {
        if (pos < _data || pos >= _data+_size) return;

        if (pos == _data+_size-1) {
            --_size;
            return;
        }

        for (T* it = pos; it != _data+_size-1; ++it)
            *it = std::move(*(it+1));
        --_size;
    }

};