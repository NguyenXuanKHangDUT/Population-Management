#include "Database.h"
#include "UserInterface.h"

#include <iostream>
#include <string>

using namespace std;

MyVector<Person*> profiles;
HashMap<string, Person*> IDHash;
HashMap<string, int> PersonIndex;

MyVector<Household*> Families;

Person* User = nullptr;

/*
g++ ___Launcher.cpp Household.cpp Host.cpp Admin.cpp Person.cpp date.cpp UserInterface.cpp -o a
*/
// 111654714251 admin xk
// 101026170669 host ak
/*
'111654714251','11714251',Nguyen Xuan Khang,13/10/2006,male,Sina/Mitras,'null',admin,0,11111119
'101026170669','10170669',Huynh Nguyen Anh Kiet,10/11/2006,male,Rose/Trost,'null',Teacher,40,11111112

'11714251',Sina/Mitras,'111654714251','01'
'10170669',Rose/Trost,'101026170669','01'
*/
int main() {
    // Init database
    Database* db = Database::Instance();

    profiles = db->getProfiles();
    Families = db->getFamilies();
    PersonIndex = db->getPersonIndex();
    IDHash = db->getIDHash();

    while (1) {
        login(); 
        
        UserInterface* UI = nullptr;
        if (User->getJob() == "admin")
            UI = new AdminInterface();
        else if (User->getPersonID().substr(7,5) == User->getHouseholdID().substr(3,5))
            UI = new HostInterface();
        else
            UI = new NetizenInterface();
        
        bool isExit = UI->launch();
        delete UI;

        if (isExit) break;
        else User = nullptr;
    }

    // reassign global containers before updating
    db->getProfiles() = profiles;
    db->getFamilies() = Families;
    db->getPersonIndex() = PersonIndex;
    db->getIDHash() = IDHash;
    
    cout << "Updating data ... \n";
    delete db;
    cout << "Data updated!";
    return 0;
}