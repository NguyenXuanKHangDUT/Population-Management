#include "Admin.hpp"
#include "Person.hpp"
#include "Household.hpp"
#include "Host.hpp"

#include <iostream>
#include <iomanip>
#include <string>

#include <queue>
#include <cmath>

#include "DSA/Algorithms.h"

using namespace std;

extern MyVector<Person*> profiles;
extern MyVector<Household*> Families;
extern HashMap<string, Person*> IDHash;
extern HashMap<string, int> PersonIndex;

Admin::Admin(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) 
        : Host(pID, hID, fName, bDay, gend, addr, pnID, job, inc, pwd) {}

Admin::~Admin() {}

bool Admin::banishMember() {
    // Admin can banish any citizen from profiles
    name:
    cout << "ID: "; string id; cin >> id;
    auto target = IDHash.find(id);
    if (target == IDHash.end()) {
        cout << "\u001b[33m";
        cout << "There were no members with ID: " << id <<  ". Please type again.\n";
        cout << "\u001b[0m";
        goto name;
    }
    Person* member = target->second;
    if (member->getHost() == member) {
        cout << "This person is the host. Deleting the host will also delete all members in the household.\nAre you sure? (1/0: yes/no): ";
        int ch; cin >> ch;
        if (ch != 1) {
            cout << "\u001b[33m";
            cout << "command cancelled\n";
            cout << "\u001b[0m";
            goto name;
        }
        // delete all members in that household
        Household* hh = member->getHost()->getHousehold();
        while (!hh->Member.empty()) {
            Person* m = hh->Member[0];
            hh->removeMember(m);
        }
        for (int i = 0; i < Families.size(); ++i) {
            if (Families[i]->getHouseholdID() == hh->getHouseholdID()) {
                Families.erase(Families.begin()+i);
                break;
            }
        }
    }
    else {
        // remove from his/her household and profiles
        string name = member->getFullName(), id = member->getPersonID();
        Household* hh = member->getHost()->getHousehold();
        hh->removeMember(member);
    }
    cout << id << " " << member->getFullName() << " has been removed from profiles successfully\n";
    return true;
}

bool Admin::summonMember() {
    cout << "The member will be added to the profiles and set as the new household host.\n";
    
    cout << "Full Name: "; string name; cin.ignore(); getline(cin, name);
    cout << "Birthday (dd/mm/yyyy):\n "; date bDay; cin >> bDay; string bd = bDay.getDate_String();
    cout << "Gender (1/0 : male/female): "; bool gd; cin >> gd;
    ADDRESS:
    cout << "Address (Region/{1 sub-region} .. Region must be Maria/Rose/Sina):\n   "; string addr; cin.ignore(); getline(cin, addr);
    if (addr.find("Maria") != 0 && addr.find("Rose") != 0 && addr.find("Sina") != 0) {
        cout << "\u001b[31m";
        cout << "Invalid region! Please type again.\n";
        cout << "\u001b[0m";
        goto ADDRESS;
    }
    string partnerID = "'null'";
    cout << "Job: "; string j; getline(cin, j);
    cout << "Income (USD): "; double ic; cin >> ic;
    string rg = addr.substr(0, addr.find('/'));

    string newID = makeUniqueID(name, bd, gd, rg);
    string hhID = newID.substr(0,2)+newID.substr(6,6);
    Host* p = new Host(newID, hhID, name, bd, gd, addr, partnerID, j, ic, "11111111");
    Household* hh = new Household(hhID, addr, newID, newID.substr(0,2));
    p->setHousehold(hh);
    p->setHost(p);
    hh->addMember(p);
    hh->setHost(p);

    // add to global profiles and Families
    profiles.push_back(p);
    Families.push_back(hh);
    IDHash[newID] = p;
    PersonIndex[p->getPersonID()] = profiles.size()-1;

    cout << p->getFullName() << " has been added successfully with Personal ID: " << p->getPersonID() << " and Household ID: " << p->getHouseholdID() << "\n";
    cout << *hh;
    return true;
}

void Admin::searchName() {
    cout << "\n\u001b[36m";
    cout << "Type the name to search: "; string name; cin.ignore(); getline(cin, name);
    cout << "\u001b[0m";
    MyVector<Person*> _list;
    
    for (Person* p : profiles) {
        if (p->getFullName() == name)
            _list.push_back(p);
    }
    if (_list.size() == 0) {
        cout << "\u001b[33m";
        cout << "There were no person with that name!\n";
        cout << "\u001b[0m";

    }
    else {
        // cyan
        cout << "\u001b[36m";
        cout << "There were " << _list.size() << " person who name: " << name << "\n";
        cout << "\u001b[0m";
        for (Person* p : _list)
            cout << *p;
        // cyan
        cout << "\u001b[36m";
        cout << "-----------------------------------\n";
        cout << "\u001b[0m";
    }
}

void Admin::searchID() {
    cout << "\n\u001b[36m";
    cout << "Type ID to search: "; string id; cin >> id;
    cout << "\u001b[0m";
    auto target = IDHash.find(id);
    if (target == IDHash.end()) {
        cout << "\u001b[33m";
        cout << " There were no person id by " << id << "\n";
        cout << "\u001b[0m";
    }
    else {
        cout << *(target->second);
    }
    
}

void printBarPyramid(double percent, char symbol, int max_bar_length, bool isMale, int& current_width) {
    int length = static_cast<int>(round(percent * max_bar_length / 100.0));
    current_width = length;

    if (isMale) {
        // right align
        int padding = max_bar_length - length;
        cout << string(padding, ' ');
        cout << fixed << setprecision(1) << percent << "% ";
        cout << "\u001b[31m";
        cout << string(length, symbol);

        cout << "\u001b[0m";
    } else {
        cout << "\u001b[34m"; 
        cout << string(length, symbol);

        cout << "\u001b[0m";
        cout << " " << fixed << setprecision(1) << percent << "%";
    }
}
void Admin::populationPyramid() {
    MyVector<int> maleCount(200, 0), femaleCount(200, 0);
    int maxAge = 0, M = 0, R = 0, S = 0;

    // calculate age
    for (Person* p : profiles) {
        if (p->getAddress().substr(0,4) == "Sina") S++;
        else if (p->getAddress().substr(0,4) == "Rose") R++;
        else M++;
        int age = p->getAge();
        if (age > maxAge)
            maxAge = age;
        if (p->getGender())
            maleCount[age]++;
        else femaleCount[age]++;
    }
    // divide into age groups
    // 0-4, 5-9, ..., 80-84, 85+ (18 groups)
    MyVector<int> maleGroup(18, 0), femaleGroup(18, 0);
    for (int i = 0; i <= maxAge; ++i) {
        int j;
        if (i >= 85) j = 17;
        else j = i/5;
        maleGroup[j] += maleCount[i];
        femaleGroup[j] += femaleCount[i];
    }

    int totalPop = 0;
    for (int i = 0; i < 18; ++i)
        totalPop += maleGroup[i] + femaleGroup[i];

    // Have no permission to use STL, so MyVector cannot have initializer_list constructor,
    // so we manually create ageRanges :__(
    MyVector<string> ageRanges;
    ageRanges.push_back("0-4"); ageRanges.push_back("5-9"); ageRanges.push_back("10-14");
    ageRanges.push_back("15-19"); ageRanges.push_back("20-24"); ageRanges.push_back("25-29");
    ageRanges.push_back("30-34"); ageRanges.push_back("35-39"); ageRanges.push_back("40-44");
    ageRanges.push_back("45-49"); ageRanges.push_back("50-54"); ageRanges.push_back("55-59");
    ageRanges.push_back("60-64"); ageRanges.push_back("65-69"); ageRanges.push_back("70-74");
    ageRanges.push_back("75-79"); ageRanges.push_back("80-84"); ageRanges.push_back("85+");

    const int maxBarLen = 30; 
    const char barSymbol = '=';

    cout << "\n      Population Pyramid     \n";
    cout << string(maxBarLen * 2 + 15, '-') << "\n";
    

    cout << setw(maxBarLen + 5) << right << "Male" 
            << " | " << setw(6) << left << "Age " << " | "
            << setw(maxBarLen + 5) << left << "Female\n";
    cout << string(maxBarLen * 2 + 15, '-') << "\n";

    for (int i = 18-1; i >= 0; --i) {
        double maleRate = (double)maleGroup[i] / totalPop * 100.0;
        double femaleRate = (double)femaleGroup[i] / totalPop * 100.0;
        int maleLen = 0, femaleLen = 0;

        printBarPyramid(maleRate, barSymbol, maxBarLen, true, maleLen);

        cout << " | " << setw(6) << left << ageRanges[i] << " | ";
    
        printBarPyramid(femaleRate, barSymbol, maxBarLen, false, femaleLen);
        cout << '\n';
    }
    cout << string(maxBarLen*2 + 15, '-') << "\n";
    cout << "Population in all three Regions: Maria (rural area), Rose (urban area), Sina (capital)\n\n";
    cout << "Total Population: " << totalPop << " persons\n\n";
    cout << "Sina (capital): " << S << " persons\n";
    cout << "Rose (urban area): " << R << " persons\n";
    cout << "Maria (rural area): " << M << " persons\n";
    cout << "\nThe region with the highest population is ";;
    if (M >= R && M >= S) cout << "Maria.\n";
    else if (R >= M && R >= S) cout << "Rose.\n";
    else cout << "Sina.\n";
}

void printBar(double percent, char symbol, int max_bar_length) {
    int length = static_cast<int>(round(percent * max_bar_length / 100.0));
    
    for (int i = 0; i < length; ++i)
        cout << symbol;
}
void Admin::marriageRate() {
    MyVector<MyPair<int,int>> maleCount(200, {0,0}), femaleCount(200, {0,0});
    int maxAge = 0;

    // calculate age distribution
    for (Person* p : profiles) {
        int age = p->getAge();
        if (age > maxAge)
            maxAge = age;
        if (p->getGender()) {
            maleCount[age].first++;
            if (p->getPartner() != nullptr) 
                maleCount[age].second++;
        }
        else {
            femaleCount[age].first++;
            if (p->getPartner() != nullptr) 
                femaleCount[age].second++;
        }
    }
    // divide into age groups
    // 15-19, 20-24, ... 80-84, 85+ (15 groups)
    MyVector<MyPair<int,int>> maleGroup(15, {0,0}), femaleGroup(15, {0,0}); // 15 groups
    for (int i = 15; i <= maxAge; ++i) {
        int j = -1;
        if (i >= 15 && i <= 84)
            j = (i-15) / 5; 
        else if (i >= 85)
            j = 14; // 85+
        
        if (j != -1 && j < 15) {
            maleGroup[j].first += maleCount[i].first;
            maleGroup[j].second += maleCount[i].second;
            
            femaleGroup[j].first += femaleCount[i].first;
            femaleGroup[j].second += femaleCount[i].second;
        }
    }

    // Have no permission to use STL, so MyVector cannot have initializer_list constructor,
    // so we manually create ageRanges :__(
    MyVector<string> ageRanges;
    ageRanges.push_back("15-19");
    ageRanges.push_back("20-24");
    ageRanges.push_back("25-29");
    ageRanges.push_back("30-34");
    ageRanges.push_back("35-39");
    ageRanges.push_back("40-44");
    ageRanges.push_back("45-49");
    ageRanges.push_back("50-54");
    ageRanges.push_back("55-59");
    ageRanges.push_back("60-64");
    ageRanges.push_back("65-69");
    ageRanges.push_back("70-74");
    ageRanges.push_back("75-79");
    ageRanges.push_back("80-84");
    ageRanges.push_back("85+");

    const int maxBarLen = 50;
    cout << "Marriage Rate by Age Groups:\n";
    cout << "Signature: = = Male, Signature: * = Female\n";
    cout << string(maxBarLen + 20, '-') << "\n";

    for (int i = 0; i < 15; ++i) {
        double maleRate = maleGroup[i].first == 0 ? 0.0 : (double)maleGroup[i].second / maleGroup[i].first * 100.0;
        double femaleRate = femaleGroup[i].first == 0 ? 0.0 : (double)femaleGroup[i].second / femaleGroup[i].first * 100.0;

        cout << setw(5) << left << ageRanges[i] << " | ";
        cout << "\u001b[31m"; // red
        printBar(maleRate, '=', maxBarLen);
        cout << "\u001b[0m";
        cout << fixed << setprecision(0) << maleRate << "%";

        cout << "\n" << setw(5) << left << " " << " | ";
        
        cout << "\u001b[34m"; // blue
        printBar(femaleRate, '*', maxBarLen);
        cout << fixed << setprecision(0) << femaleRate << "%";
        
        cout << "\n";
    }
    cout << string(maxBarLen + 20, '-') << "\n";
    cout << "\u001b[0m";
}

void printHorizontalBar(double income, int max_bar_length, double max_income_value, char bar_symbol) {
    int bar_length = 0;
    if (max_income_value > 0)
        bar_length = static_cast<int>(round((income / max_income_value) * max_bar_length));
    
    
    if (bar_length < 0) bar_length = 0;
    cout << "\u001b[36m"; // Cyan
    cout << string(bar_length, bar_symbol);
    cout << "\u001b[0m"; // Reset color

    cout << " " << fixed << setprecision(1) << income << " USD/month";
}
void Admin::averageIncome() {
    MyVector<MyPair<int,double>> age_income(200, {0,0.0});
    MyPair<int, double> M(0,0.0), R(0,0.0), S(0,0.0);
    int maxAge = 0;
    // calculate age - income
    for (Person* p : profiles) {
        int age = p->getAge();
        if (age > maxAge)
            maxAge = age;
        age_income[age].first++;
        age_income[age].second += p->getIncome();

        if (p->getAddress().substr(0,4) == "Sina") {
            S.first++;
            S.second += p->getIncome();
        }
        else if (p->getAddress().substr(0,4) == "Rose") {
            R.first++;
            R.second += p->getIncome();
        }
        else {
            M.first++;
            M.second += p->getIncome();
        }
    }

    // divide into age groups
    // 15-19, 20-24, ... 60-64, 65+ (11 groups)
    MyVector<MyPair<int,double>> ageGroup(11, {0,0.0});
    for (int i = 15; i <= maxAge; ++i) {
        int j = -1;
        if (i >= 15 && i <= 64)
            j = (i-15) / 5; 
        else if (i >= 65)
            j = 10; // 65+
        
        if (j != -1 && j < 11) {
            ageGroup[j].first += age_income[i].first;
            ageGroup[j].second += age_income[i].second;
        }
    }

    // Have no permission to use STL, so MyVector cannot have initializer_list constructor,
    // so we manually create ageRanges :__(
    MyVector<string> ageRanges;
    ageRanges.push_back("15-19");
    ageRanges.push_back("20-24");
    ageRanges.push_back("25-29");
    ageRanges.push_back("30-34");
    ageRanges.push_back("35-39");
    ageRanges.push_back("40-44");
    ageRanges.push_back("45-49");
    ageRanges.push_back("50-54");
    ageRanges.push_back("55-59");
    ageRanges.push_back("60-64");
    ageRanges.push_back("65+");

    MyVector<double> avgIncomes(11, 0.0);
    double maxIncomeValue = 0.0;

    for (int i = 0; i < 11; ++i) {
        if (ageGroup[i].first > 0) {
            avgIncomes[i] = ageGroup[i].second / ageGroup[i].first;
        }
        if (avgIncomes[i] > maxIncomeValue) {
            maxIncomeValue = avgIncomes[i];
        }
    }

    if (maxIncomeValue == 0) {
        cout << "No income data available to display.\n";
        return;
    }

    const int maxBarLength = 60;
    const char barSymbol = '=';
    const int labelWidth = 7;

    cout << "\n=== Average Income by Age Groups ===\n";
    cout << "Income: USD/month\n";
    cout << string(labelWidth + maxBarLength + 20, '-') << "\n";

    for (int i = 11 - 1; i >= 0; --i) {
        cout << setw(labelWidth) << left << ageRanges[i] << " | ";
        printHorizontalBar(avgIncomes[i], maxBarLength, maxIncomeValue, barSymbol);
        cout << "\n";
    }
    cout << string(labelWidth + maxBarLength + 20, '-') << "\n";
    cout << "Income scale: 1 USD ~ " << fixed << setprecision(5) << (double)maxBarLength / maxIncomeValue << " characters\n";

    cout << "\ntotal income in the country (GDP): "; cout << fixed << setprecision(2) << (M.second + R.second + S.second) * 12 << " USD/year\n";
    cout << " - Sina (capital): " << fixed << setprecision(2) << S.second * 12 << " USD/year\n";
    cout << " - Rose (urban area): " << fixed << setprecision(2) << R.second * 12 << " USD/year\n";
    cout << " - Maria (rural area): " << fixed << setprecision(2) << M.second * 12 << " USD/year\n";
    cout << "the region with the highest income/year is ";
    if (M.second >= R.second && M.second >= S.second) cout << "Maria.\n";
    else if (R.second >= M.second && R.second >= S.second) cout << "Rose.\n";
    else cout << "Sina.\n\n";
    
    
    cout << "average income per capita: ";
    cout << fixed << setprecision(2) << (M.second + R.second + S.second) / profiles.size() << " USD/person/month\n";
    cout << " - Sina (capital): " << fixed << setprecision(2) << S.second / S.first << " USD/person/month\n";
    cout << " - Rose (urban area): " << fixed << setprecision(2) << R.second / R.first << " USD/person/month\n";
    cout << " - Maria (rural area): " << fixed << setprecision(2) << M.second / M.first << " USD/person/month\n";
    cout << "the region with the highest average income/person/month is ";
    if (M.second / M.first >= R.second / R.first && M.second / M.first >= S.second / S.first) cout << "Maria.\n";
    else if (R.second / R.first >= M.second / M.first && R.second / R.first >= S.second / S.first) cout << "Rose.\n";
    else cout << "Sina.\n";
    

}

void Admin::sixDegreesOfSeparation() {
    cout << "\u001b[36m";
    cout << "Person header ID: "; string idA; cin >> idA;
    cout << "Person target ID: "; string targetID; cin >> targetID;
    cout << "\u001b[0m";
    auto targetA = IDHash.find(idA);
    auto targetB = IDHash.find(targetID);
    if (targetA == IDHash.end()) {
        cout << "There were no person ID by " << idA << "\n";
        return;
    }
    if (targetB == IDHash.end()) {
        cout << "There were no person ID by " << targetID << "\n";
        return;
    }
    Person* header = targetA->second;
    Person* target = targetB->second;

    MyVector<Person*> path;

    if (header == target) {
        cout << "Both IDs belong to the same person: " << header->getFullName() << "\n";
        return;
    }

    // init adjacency list
    HashMap<string, MyVector<string>> adj; // static adjacency list
    for (Person* p : profiles) {
        string pID = p->getPersonID();
        Person* partner = p->getPartner();

        if (partner) {
            adj[pID].push_back(partner->getPersonID());
            adj[partner->getPersonID()].push_back(pID);
        }

        Household* hh = p->getHost()->getHousehold();
        for (Person* member : hh->Member) {
            if (member == p) continue;
            adj[pID].push_back(member->getPersonID());
            adj[member->getPersonID()].push_back(pID);
        }
    }

    // BFS
    queue<string> q;
    HashMap<string, string> parent;

    q.push(header->getPersonID());
    parent[header->getPersonID()] = ""; // root has no parent

    while (!q.empty()) {
        string curID = q.front();
        q.pop();

        MyVector<string>& neighbors = adj[curID];
        for (const string& eachID : neighbors) {
            if (parent.find(eachID) == parent.end()) {
                parent[eachID] = curID;

                if (eachID == targetID) {
                    // found the linked path, reconstruct the path
                    string nextID = targetID;
                    while (nextID != "") {
                        Person* p = IDHash[nextID];
                        path.push_back(p);
                        nextID = parent[nextID];
                    }
                    // reverse the path
                    MyVector<Person*> reversed;
                    for (int i = path.size()-1; i >= 0; --i) {
                        reversed.push_back(path[i]);
                    }
                    path = reversed;
                    cout << "Path found! Degrees of separation: " << path.size()-1 << "\n";
                    cout << "\u001b[36m";
                    cout << "ID: " << header->getPersonID() << " Name: " << header->getFullName() << "\n";
                    cout << "\u001b[0m";
                    for (int i = 1; i < path.size()-2; ++i) {
                        cout << "  |\n  v\n";
                        cout << "ID: " << path[i]->getPersonID() << " Name: " << path[i]->getFullName() << "\n";
                    }
                    cout << "  |\n  v\n";
                    cout << "\u001b[36m";
                    cout << "ID: " << target->getPersonID() << " Name: " << target->getFullName() << "\n";
                    cout << "\u001b[0m";
                    return;
                }
                q.push(eachID);
            }
        }

    }
    cout << "No connection found between\n" << header->getFullName() << " and\n" << target->getFullName() << "\n";
}

ostream& operator<<(ostream& out, const Admin& p) {
    out << "\u001b[36m";
    out << "\n----------------------------------" << endl;
    out << "\u001b[0m";
    out << "Personal information:" << endl;
    out << "Person ID: " << p.PersonID << endl;
    out << "Full Name: " << p.FullName << endl;
    out << "Birthday: " << p.Birthday << " (Age: " << p.Age << ")" << endl;
    out << "Gender: " << (p.Gender?"Male":"Female") << endl;
    out << "Address: " << p.Address << endl;
    out << "Job: " << p.Job << " (Income: (per month) " << p.Income << " USD)" << endl;
    out << "Marriage: ";
    if (p.Partner != nullptr)
        out << p.Partner->getFullName() << "    ID: " << p.Partner->getPersonID() << (p.Gender?" (Wife)":" (Husband)") << endl;
    else out << "Single" << endl;
    out << "Household ID: " << p.HouseholdID << endl;
    // if (p.Host != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonID() << endl;
    out << "Region: " << p.Region << endl;
    out << "\u001b[36m";
    out << "\n----------------------------------" << endl;
    out << "\u001b[0m";
    return out;
}