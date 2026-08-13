#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Patient{
    int patientID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

//extern mean this vector already exist in ClinicManagementSystem.cpp
extern vector<Patient> patientList;

void patientMenu(){
    cout << " +----------------------------------------+\n";
    cout << " |          PATIENT MANAGEMENT             |\n";
    cout << " +----------------------------------------+\n";
    cout << "\n [Derrick] System in progress...\n";
}