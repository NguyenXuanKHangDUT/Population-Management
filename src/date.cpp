#include "DSA/date.h"

int date::day_in_month[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

date::date(const int& d, const int& m, const int& y) 
    : day(d), month(m), year(y)
{}

date::date(const string& bd) { // d/m/y
    int p1 = bd.find('/');
    int p2 = bd.find('/', p1+1);

    string d = bd.substr(0, p1);
    string m = bd.substr(p1+1, p2-p1-1);
    string y = bd.substr(p2+1);

    this->day = stoi(d);
    this->month = stoi(m);
    this->year = stoi(y);
}

string date::getDate_String() const {
    string d = to_string(this->day);
    if (d.size() == 1) d = "0"+d;
    
    string m = to_string(this->month);
    if (m.size() == 1) m = "0"+m;

    string s = d + "/" + m + "/" + to_string(this->year);
    return s;
}

ostream& operator<<(ostream& o, const date& d) {
    o << d.day << "/" << d.month << "/" << d.year << "\n";
    return o;
}
istream& operator>>(istream& i, date& d) {
DAY_INPUT:
    cout << "Day (1-31): ";
    if (!(i >> d.day) || d.day < 1 || d.day > 31) {
        cout << "Invalid day range! Day must be between 1 and 31.\n";
        i.clear();
        i.ignore(100, '\n');
        goto DAY_INPUT;
    }

MONTH_INPUT:
    cout << "Month (1-12): ";
    if (!(i >> d.month) || d.month < 1 || d.month > 12) {
        cout << "Invalid month range! Month must be between 1 and 12.\n";
        i.clear(); 
        i.ignore(100, '\n');
        goto MONTH_INPUT;
    }

YEAR_INPUT:
    cout << "Year (1-2025): ";
    if (!(i >> d.year) || d.year < 1 || d.year > 2025) {
        cout << "Invalid year range! Year must be between 1 and 2025.\n";
        i.clear(); 
        i.ignore(100, '\n');
        goto YEAR_INPUT;
    }
    
    int maxDay = date::day_in_month[d.month];
    if (d.month == 2 && d.isLeapYear())
        maxDay = 29;
    
    if (d.day > maxDay) d.day = maxDay;
    
    return i;
}