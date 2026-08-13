#include<iostream>
#include<string>
#include <vector>
using namespace std;

struct Appointment{
    int appointmentID;
    int patientID;
    string doctorName;
    string date;
    string time;
    string status;
};

//extern mean this vector already exist in ClinicManagementSystem.cpp
extern vector<Appointment> appointmentList;

void appointmentMenu(){
    cout << " +----------------------------------------+\n";
    cout << " |         APPOINTMENT SCHEDULE            |\n";
    cout << " +----------------------------------------+\n";
    cout << "\n [Dennis] System in progress...\n";
}