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

    int choice;

    do {
        cout << "\n +-----------------------------------------+\n";
        cout << " |          PATIENT MANAGEMENT             |\n";
        cout << " +-----------------------------------------+\n";
        cout << " | [1] Register New Patient                |\n";
        cout << " | [2] Consultation & Medical History      |\n";
        cout << " | [3] Display All Patients                |\n";
        cout << " | [4] Generate Medical Certificate (MC)   |\n";
        cout << " | [5] Back to Main Menu                   |\n";
        cout << " +-----------------------------------------+\n";

        choice = getValidatedInt(" Enter your choice (1-5): ", 1, 5);
        cout << "\n";

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
            cout << "\n Returning to Main Menu...\n";
            break;
        }

    } while (choice != 5);
}

string formatPatientID(int id) {
    stringstream ss;
    ss << "P" << setfill('0') << setw(4) << id;
    return ss.str();
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
        cout << " [ERROR] Invalid input! Please enter a valid number between " 
             << setfill('0') << setw(4) << minVal << " and " << setw(4) << maxVal << setfill(' ') << ".\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void savePatientsToFile() {
    ofstream outFile("patients.txt");
    if (!outFile.is_open()) {
        cout << " [ERROR] Failed to save patient records to file!\n";
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
    cout << "\n +-------------------------------------------+\n";
    cout << " |           REGISTER NEW PATIENT            |\n";
    cout << " +-------------------------------------------+\n";
    
    while (true) {
        newPatient.patientID = getValidatedInt(" Enter Patient ID (0001 - 9999): ", 1, 9999);
        if (findPatientByID(newPatient.patientID) == nullptr) {
            break;
        }
        cout << " [ERROR] Patient ID " << formatPatientID(newPatient.patientID)
            << " already exists! Try another ID.\n";
    }

    cout << " Enter Patient Full Name : ";
    getline(cin, newPatient.patientName);

    newPatient.age = getValidatedInt(" Enter Patient Age (0 - 120): ", 0, 120);

    cout << " Enter Known Allergies (e.g. Penicillin / None): ";
    getline(cin, newPatient.allergy);

    newPatient.medicalHistory.push_back("Initial Registration & Health Profile Created.");

    patientList.push_back(newPatient);
    savePatientsToFile(); 

    cout << "\n +--------------------------------------------+\n";
    cout << " |                REGISTRATION                |\n";
    cout << " +--------------------------------------------+\n";
    cout << " | Patient Name : "<< left << setw(27)<< newPatient.patientName<< " |\n";
    cout << " | Patient ID   : "<< left << setw(27)<< formatPatientID(newPatient.patientID)<< " |\n";
    cout << " | Age          : "<< left << setw(27)<< newPatient.age<< " |\n";
    cout << " | Allergy      : "<< left << setw(27)<< newPatient.allergy<< " |\n";
    cout << " +--------------------------------------------+\n";
    cout << " [SUCCESS] Patient registered successfully!\n";
    cout << " [INFO] Patient record saved to patients.txt.\n";
}

void addConsultationRecord() {
    cout << "\n +--------------------------------------------------------+\n";
    cout << " |               CONSULTATION & HISTORY                   |\n";
    cout << " +--------------------------------------------------------+\n";
    if (patientList.empty()) {
        cout << " [INFO] No patients registered yet. Please register a patient first.\n";
        return;
    }

    int searchID = getValidatedInt(" Enter Patient ID (0001 - 9999): ", 1, 9999);
    Patient* patient = findPatientByID(searchID);

    if (patient == nullptr) {
        cout << " [ERROR] Patient with ID " << formatPatientID(searchID) << " not found.\n";
        return;
    }

    cout << "\n +--------------------------------------------------------+\n";
    cout << " |                  PATIENT INFORMATION                   |\n";
    cout << " +--------------------------------------------------------+\n";
    cout << " | Patient ID   : "<< left << setw(39)<< formatPatientID(patient->patientID)<< " |\n";
    cout << " | Patient Name : "<< left << setw(39)<< patient->patientName<< " |\n";
    cout << " | Age          : "<< left << setw(39)<< patient->age<< " |\n";
    cout << " | Allergies    : "<< left << setw(39)<< patient->allergy<< " |\n";
    cout << " +--------------------------------------------------------+\n";
    cout << " |                     MEDICAL HISTORY                    |\n";
    cout << " +--------------------------------------------------------+\n";

    for (size_t i = 0; i < patient->medicalHistory.size(); i++) {

        cout << " | [" << i + 1 << "] "
            << left << setw(34)
            << patient->medicalHistory[i]
            << "     |\n";
    }
    cout << " +--------------------------------------------------------+\n";

    cout << " Enter New Diagnosis / Consultation Notes: ";
    string newDiagnosis;
    getline(cin, newDiagnosis);

    if (!newDiagnosis.empty()) {
        patient->medicalHistory.push_back(newDiagnosis);
        savePatientsToFile(); 
        cout << "\n [SUCCESS] Medical history updated and saved to file!\n";
    } else {
        cout << "\n [WARNING] Empty notes. Consultation entry cancelled.\n";
    }
}

void displayAllPatients() {
    cout << "\n +--------------------------------------------------------------------------------+\n";
    cout << " |                             PATIENT RECORDS                                    |\n";
    cout << " +--------------------------------------------------------------------------------+\n";

    if (patientList.empty()) {
        cout << " [INFO] No patient records found in the system.\n";
        cout << " +--------------------------------------------------------------------------------+\n";
        return;
    }
    cout << " | " << left
        << setw(10) << "Patient ID"
        << " | " << setw(19) << "Name"
        << " | " << setw(5) << "Age"
        << " | " << setw(14) << "Allergy "
        << " | " << setw(18) << "Latest History   "
        << " |\n";
    cout << " +--------------------------------------------------------------------------------+\n";

    for (size_t i = 0; i < patientList.size(); i++) {
        string lastHistory = patientList[i].medicalHistory.empty() ? "N/A" : patientList[i].medicalHistory.back();
        if (lastHistory.length() > 18) {
            lastHistory = lastHistory.substr(0, 15) + "...";
        }

        string idSS = formatPatientID(patientList[i].patientID);

        cout << " | " << left << setw(10) << idSS
            << " | " << setw(19) << (patientList[i].patientName.length() > 19 ? patientList[i].patientName.substr(0, 14) + "..." : patientList[i].patientName)
            << " | " << setw(5) << patientList[i].age
            << " | " << setw(14) << (patientList[i].allergy.length() > 15 ? patientList[i].allergy.substr(0, 12) + "..." : patientList[i].allergy)
            << " | " << setw(18) << lastHistory << " |\n";
    }
    cout << " +--------------------------------------------------------------------------------+\n";
    cout << " Total Registered Patients: " << patientList.size() << "\n";
}

void generateMC() {
    cout << "\n +---------------------------------------------+\n";
    cout << " |       GENERATE MEDICAL CERTIFICATE          |\n";
    cout << " +---------------------------------------------+\n";
    if (patientList.empty()) {
        cout << " [INFO] No patients available in the system.\n";
        return;
    }

    int searchID = getValidatedInt(" Enter Patient ID (0001 - 9999): ", 1, 9999);
    Patient* patient = findPatientByID(searchID);

    if (patient == nullptr) {
        cout << " [ERROR] Patient with ID " << formatPatientID(searchID) << " not found.\n";
        return;
    }

    int mcDays = getValidatedInt(" Enter number of MC days granted (1 - 14): ", 1, 14);
    cout << " Enter Attending Doctor's Name: Dr. ";
    string doctorName;
    getline(cin, doctorName);

    if (doctorName.empty()) {
        doctorName = "Derrick Tan";
    }

    cout << " Enter Medical Reason / Diagnosis: ";
    string reason;
    getline(cin, reason);
    if (reason.empty()) {
        reason = "Acute Upper Respiratory Tract Infection";
    }

    cout << "\n +-------------------------------------------------------------------+\n";
    cout << " |                   HARMONY CLINIC                                  |\n";
    cout << " |                MEDICAL CERTIFICATE                                |\n";
    cout << " +-------------------------------------------------------------------+\n";
    cout << " | Date Issued      : "<< left << setw(45)<< "25/08/2026"<< "  |\n";
    cout << " | Patient Name     : "<< left << setw(45)<< patient->patientName<< "  |\n";
    cout << " | Patient ID       : "<< left << setw(45)<< formatPatientID(patient->patientID)<< "  |\n";
    cout << " | Age              : "<< left << setw(45)<< patient->age<< "  |\n";
    cout << " | Known Allergies  : "<< left << setw(45)<< patient->allergy<< "  |\n";
    cout << " +-------------------------------------------------------------------+\n";
    cout << " | This is to certify that the patient named above is unfit          |\n";
    cout << " | for work / school duties for a period of "<< left << setw(10)<< mcDays<< " day(s).       |\n";
    cout << " +-------------------------------------------------------------------+\n";
    cout << " | Reason / Diagnosis : "<< left << setw(44)<< reason<< " |\n";
    cout << " | Attending Doctor   : Dr. "<< left << setw(40)<< doctorName<< " |\n";
    cout << " +-------------------------------------------------------------------+\n";

    string sigLine = "Signature: [ Dr. " + doctorName + " ]";
    cout << " | "<< right << setw(64)<< sigLine<< "  |\n";
    cout << " | "<< right << setw(64)<< "(Certified Medical Practitioner)"<< "  |\n";
    cout << " +-------------------------------------------------------------------+\n";

    patient->medicalHistory.push_back("MC Issued: " + to_string(mcDays) + " day(s) by Dr. " + doctorName + " (Reason: " + reason + ")");
    savePatientsToFile();
    cout << "\n [SUCCESS] MC generated and saved to patients.txt!\n";
}