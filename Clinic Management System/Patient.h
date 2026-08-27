#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Patient {
    int patientID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

extern vector<Patient> patientList;

void patientMenu();
void loadPatientsFromFile();
void savePatientsToFile();