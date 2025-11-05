#pragma once

template <typename Type1, typename Type2>
class MyPair {
public:
    Type1 first;
    Type2 second;

    // Constructors
    MyPair();
    MyPair(const Type1& a, const Type2& b);
    MyPair(const MyPair& other);

    // Assignment
    MyPair& operator=(const MyPair& other);

    // Comparison
    inline bool operator==(const MyPair& other) const;
    inline bool operator!=(const MyPair& other) const;
    inline bool operator<(const MyPair& other) const;
    inline bool operator>(const MyPair& other) const;
    inline bool operator<=(const MyPair& other) const;
    inline bool operator>=(const MyPair& other) const;

    // Swap
    void swap(MyPair& other);
};

// Constructors Definitions
template <typename Type1, typename Type2>
inline MyPair<Type1, Type2>::MyPair() : first(Type1()), second(Type2()) {}

template <typename Type1, typename Type2>
inline MyPair<Type1, Type2>::MyPair(const Type1& a, const Type2& b) : first(a), second(b) {}

template <typename Type1, typename Type2>
inline MyPair<Type1, Type2>::MyPair(const MyPair& other) : first(other.first), second(other.second) {}

// Assignment Definition
template <typename Type1, typename Type2>
inline MyPair<Type1, Type2>& MyPair<Type1, Type2>::operator=(const MyPair& other) {
    if (this != &other) {
        first = other.first;
        second = other.second;
    }
    return *this;
}

// Comparison Definitions
template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator==(const MyPair& other) const {
    return (first == other.first && second == other.second);
}

template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator!=(const MyPair& other) const {
    return !(*this == other);
}

template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator<(const MyPair& other) const {
    return (first < other.first) || 
           (!(other.first < first) && second < other.second);
}

template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator>(const MyPair& other) const {
    return other < *this;
}

template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator<=(const MyPair& other) const {
    return !(other < *this);
}

template <typename Type1, typename Type2>
inline bool MyPair<Type1, Type2>::operator>=(const MyPair& other) const {
    return !(*this < other);
}

// Swap Definition
template <typename Type1, typename Type2>
inline void MyPair<Type1, Type2>::swap(MyPair& other) {
    Type1 tempFirst = first;
    first = other.first;
    other.first = tempFirst;

    Type2 tempSecond = second;
    second = other.second;
    other.second = tempSecond;
}