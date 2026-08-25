#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Appointment {
    int appointmentID;
    int patientID;
    string doctorName;
    string date;
    string time;
    string status;
};

extern vector<Appointment> appointmentList;

void appointmentMenu() {
    cout << "\n\t========================================\n";
    cout << "\t          APPOINTMENT SCHEDULE          \n";
    cout << "\t========================================\n";
    cout << "\n\t[Dennis] System in progress...\n";
}