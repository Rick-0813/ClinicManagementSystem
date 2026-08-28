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
int getValidatedInt(string prompt, int minVal, int maxVal);
void loadPatientsFromFile();
void savePatientsToFile();