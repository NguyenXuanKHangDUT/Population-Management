#pragma once

#include <iostream>
#include <string>

using namespace std;

class date 
{
private:
    int day;
    int month;
    int year;
    static int day_in_month[13];
public:
    date(const int& = 9, const int& = 11, const int& = 2025);
    date(const string&);

    inline int getYear() const;
    string getDate_String() const;

    inline bool isLeapYear() const;

    friend ostream& operator<<(ostream&, const date&);
    friend istream& operator>>(istream&, date&);
};

inline int date::getYear() const {
    return this->year;
}

inline bool date::isLeapYear() const {
    if (this->year % 4 != 0) return false;
    else if (this->year % 100 != 0) return true;
    else if (this->year % 400 != 0) return false;
    else return true;
}