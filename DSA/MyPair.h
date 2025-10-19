#pragma once

template <typename Type1, typename Type2>
class MyPair {
public:
    Type1 first;
    Type2 second;

    // Constructors
    MyPair() : first(Type1()), second(Type2()) {}

    MyPair(const Type1& a, const Type2& b) : first(a), second(b) {}
    MyPair(const MyPair& other) : first(other.first), second(other.second) {}

    // Assignment
    MyPair& operator=(const MyPair& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    // Comparison
    bool operator==(const MyPair& other) const {
        return (first == other.first && second == other.second);
    }
    bool operator!=(const MyPair& other) const {
        return !(*this == other);
    }
    bool operator<(const MyPair& other) const {
        return (first < other.first) || 
               (!(other.first < first) && second < other.second);
    }
    bool operator>(const MyPair& other) const {
        return other < *this;
    }
    bool operator<=(const MyPair& other) const {
        return !(other < *this);
    }
    bool operator>=(const MyPair& other) const {
        return !(*this < other);
    }

    // Swap
    void swap(MyPair& other) {
        Type1 tempFirst = first;
        first = other.first;
        other.first = tempFirst;

        Type2 tempSecond = second;
        second = other.second;
        other.second = tempSecond;
    }
};