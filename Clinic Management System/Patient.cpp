#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

struct Patient {
    int patientID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

extern vector<Patient> patientList;

void registerPatient();
void addConsultationRecord();
void displayAllPatients();
void generateMC();
Patient* findPatientByID(int id);
int getValidatedInt(string prompt, int minVal, int maxVal);
void loadPatientsFromFile();
void savePatientsToFile();

void patientMenu() {
    loadPatientsFromFile(); 
    int choice = 0;
    do {
        cout << "\n\t======================================================\n";
        cout << "\t               PATIENT MANAGEMENT                     \n";
        cout << "\t======================================================\n";
        cout << "\t [ 1 ] Register New Patient Profile\n";
        cout << "\t [ 2 ] Consultation Diagnosis & Update History\n";
        cout << "\t [ 3 ] Display All Patients Information\n";
        cout << "\t [ 4 ] Generate Medical Certificate (MC)\n";
        cout << "\t [ 5 ] Return to Main Menu\n";
        cout << "\t------------------------------------------------------\n";

        choice = getValidatedInt("\tEnter your choice (1-5): ", 1, 5);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                addConsultationRecord();
                break;
            case 3:
                displayAllPatients();
                break;
            case 4:
                generateMC();
                break;
            case 5:
                cout << "\n\tReturning to Main Menu...\n";
                break;
        }
    } while (choice != 5);
}

Patient* findPatientByID(int id) {
    for (size_t i = 0; i < patientList.size(); i++) {
        if (patientList[i].patientID == id) {
            return &patientList[i];
        }
    }
    return nullptr;
}

int getValidatedInt(string prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            cin.ignore(1000, '\n');
            return value;
        }
        cout << "\t[ERROR] Invalid input! Please enter a valid number between " 
             << setfill('0') << setw(4) << minVal << " and " << setw(4) << maxVal << setfill(' ') << ".\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void savePatientsToFile() {
    ofstream outFile("patients.txt");
    if (!outFile.is_open()) {
        cout << "\t[ERROR] Failed to save patient records to file!\n";
        return;
    }

    for (size_t i = 0; i < patientList.size(); i++) {
        outFile << patientList[i].patientID << "|"
                << patientList[i].patientName << "|"
                << patientList[i].age << "|"
                << patientList[i].allergy << "|"
                << patientList[i].medicalHistory.size() << "|";
        
        for (size_t j = 0; j < patientList[i].medicalHistory.size(); j++) {
            outFile << patientList[i].medicalHistory[j];
            if (j + 1 < patientList[i].medicalHistory.size()) {
                outFile << ";";
            }
        }
        outFile << "\n";
    }
    outFile.close();
}

void loadPatientsFromFile() {
    ifstream inFile("patients.txt");
    if (!inFile.is_open()) {
        return; 
    }

    patientList.clear(); 
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        Patient p;

        if (getline(ss, item, '|')) p.patientID = stoi(item);
        if (getline(ss, item, '|')) p.patientName = item;
        if (getline(ss, item, '|')) p.age = stoi(item);
        if (getline(ss, item, '|')) p.allergy = item;

        int histCount = 0;
        if (getline(ss, item, '|')) histCount = stoi(item);

        string allHistory;
        if (getline(ss, allHistory)) {
            stringstream histSS(allHistory);
            string singleHistory;
            while (getline(histSS, singleHistory, ';')) {
                if (!singleHistory.empty()) {
                    p.medicalHistory.push_back(singleHistory);
                }
            }
        }

        patientList.push_back(p);
    }
    inFile.close();
}

void registerPatient() {
    Patient newPatient;
    cout << "\n\t---------------- REGISTER NEW PATIENT ----------------\n";
    
    while (true) {
        newPatient.patientID = getValidatedInt("\tEnter Patient ID (0001 - 9999): ", 1, 9999);
        if (findPatientByID(newPatient.patientID) == nullptr) {
            break;
        }
        cout << "\t[ERROR] Patient ID " << setfill('0') << setw(4) << newPatient.patientID 
             << setfill(' ') << " already exists! Try another ID.\n";
    }

    cout << "\tEnter Patient Full Name : ";
    getline(cin, newPatient.patientName);

    newPatient.age = getValidatedInt("\tEnter Patient Age (0 - 120): ", 0, 120);

    cout << "\tEnter Known Allergies (e.g. Penicillin / None): ";
    getline(cin, newPatient.allergy);

    newPatient.medicalHistory.push_back("Initial Registration & Health Profile Created.");

    patientList.push_back(newPatient);
    savePatientsToFile(); 

    cout << "\n\t[SUCCESS] Patient " << newPatient.patientName 
         << " (ID: " << setfill('0') << setw(4) << newPatient.patientID << setfill(' ') 
         << ") registered & saved to patients.txt successfully!\n";
}

void addConsultationRecord() {
    cout << "\n\t------------ CONSULTATION & MEDICAL HISTORY ------------\n";
    if (patientList.empty()) {
        cout << "\t[INFO] No patients registered yet. Please register a patient first.\n";
        return;
    }

    int searchID = getValidatedInt("\tEnter Patient ID (0001 - 9999): ", 1, 9999);
    Patient* patient = findPatientByID(searchID);

    if (patient == nullptr) {
        cout << "\t[ERROR] Patient with ID " << setfill('0') << setw(4) << searchID 
             << setfill(' ') << " not found.\n";
        return;
    }

    cout << "\n\tPatient ID   : " << setfill('0') << setw(4) << patient->patientID << setfill(' ') << "\n";
    cout << "\tPatient Name : " << patient->patientName << "\n";
    cout << "\tAge          : " << patient->age << "\n";
    cout << "\tAllergies    : " << patient->allergy << "\n";
    cout << "\t--- Past Medical History ---\n";
    for (size_t i = 0; i < patient->medicalHistory.size(); i++) {
        cout << "\t  [" << (i + 1) << "] " << patient->medicalHistory[i] << "\n";
    }

    cout << "\n\tEnter New Diagnosis / Consultation Notes: ";
    string newDiagnosis;
    getline(cin, newDiagnosis);

    if (!newDiagnosis.empty()) {
        patient->medicalHistory.push_back(newDiagnosis);
        savePatientsToFile(); 
        cout << "\n\t[SUCCESS] Medical history updated and saved to file!\n";
    } else {
        cout << "\n\t[WARNING] Empty notes. Consultation entry cancelled.\n";
    }
}

void displayAllPatients() {
    cout << "\n\t===============================================================================\n";
    cout << "\t                             PATIENT RECORDS LIST                              \n";
    cout << "\t===============================================================================\n";

    if (patientList.empty()) {
        cout << "\t [INFO] No patient records found in the system.\n";
        cout << "\t===============================================================================\n";
        return;
    }

    cout << "\t" << left << setw(8)  << "ID"
         << setw(22) << "Name"
         << setw(6)  << "Age"
         << setw(18) << "Allergies"
         << "Latest Medical Note\n";
    cout << "\t" << string(79, '-') << "\n";

    for (size_t i = 0; i < patientList.size(); i++) {
        string lastHistory = patientList[i].medicalHistory.empty() ? "N/A" : patientList[i].medicalHistory.back();
        if (lastHistory.length() > 24) {
            lastHistory = lastHistory.substr(0, 21) + "...";
        }

        stringstream idSS;
        idSS << setfill('0') << setw(4) << patientList[i].patientID;

        cout << "\t" << left << setw(8)  << idSS.str()
             << setw(22) << (patientList[i].patientName.length() > 20 ? patientList[i].patientName.substr(0, 17) + "..." : patientList[i].patientName)
             << setw(6)  << patientList[i].age
             << setw(18) << (patientList[i].allergy.length() > 16 ? patientList[i].allergy.substr(0, 13) + "..." : patientList[i].allergy)
             << lastHistory << "\n";
    }
    cout << "\t===============================================================================\n";
    cout << "\t Total Registered Patients: " << patientList.size() << "\n";
}

void generateMC() {
    cout << "\n\t---------------- GENERATE MEDICAL CERTIFICATE (MC) ----------------\n";
    if (patientList.empty()) {
        cout << "\t[INFO] No patients available in the system.\n";
        return;
    }

    int searchID = getValidatedInt("\tEnter Patient ID (0001 - 9999): ", 1, 9999);
    Patient* patient = findPatientByID(searchID);

    if (patient == nullptr) {
        cout << "\t[ERROR] Patient with ID " << setfill('0') << setw(4) << searchID 
             << setfill(' ') << " not found.\n";
        return;
    }

    int mcDays = getValidatedInt("\tEnter number of MC days granted (1 - 14): ", 1, 14);
    cout << "\tEnter Attending Doctor's Name: Dr. ";
    string doctorName;
    getline(cin, doctorName);

    if (doctorName.empty()) {
        doctorName = "Derrick Tan";
    }

    cout << "\tEnter Medical Reason / Diagnosis: ";
    string reason;
    getline(cin, reason);
    if (reason.empty()) {
        reason = "Acute Upper Respiratory Tract Infection";
    }

    stringstream idSS;
    idSS << setfill('0') << setw(4) << patient->patientID;

    cout << "\n\t" << string(66, '=') << "\n";    
    cout << "\t|" << setw(50) << right << "HARMONY CLINIC MEDICAL CERTIFICATE" << setw(16) << " |\n";
    cout << "\t|" << setw(45) << right << "Kuala Lumpur, Malaysia" << setw(21) << " |\n";
    cout << "\t" << string(66, '=') << "\n";
    cout << "\t" << left << setw(20) << " Date Issued"     << ": " << "25/08/2026" << "\n";
    cout << "\t" << left << setw(20) << " Patient Name"    << ": " << patient->patientName << "\n";
    cout << "\t" << left << setw(20) << " Patient ID"      << ": " << idSS.str() << "\n";
    cout << "\t" << left << setw(20) << " Age"             << ": " << patient->age << "\n";
    cout << "\t" << left << setw(20) << " Known Allergies" << ": " << patient->allergy << "\n";
    cout << "\t" << string(66, '-') << "\n";
    cout << "\t This is to certify that the patient named above is unfit\n";
    cout << "\t for work / school duties for a period of " << mcDays << " day(s).\n\n";
    cout << "\t" << left << setw(20) << " Reason / Diagnosis" << ": " << reason << "\n";
    cout << "\t" << left << setw(20) << " Attending Doctor"   << ": Dr. " << doctorName << "\n";
    cout << "\t" << string(66, '_') << "\n\n";

    string sigLine = "Signature: [ Dr. " + doctorName + " ]";
    cout << "\t" << right << setw(40) << sigLine << "\n";    
    cout << "\t" << right << setw(49) << "(Certified Medical Practitioner)\n";
    cout << "\t" << string(66, '=') << "\n";

    patient->medicalHistory.push_back("MC Issued: " + to_string(mcDays) + " day(s) by Dr. " + doctorName + " (Reason: " + reason + ")");
    savePatientsToFile();
    cout << "\n\t[SUCCESS] MC generated and saved to patients.txt!\n";
}