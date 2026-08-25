#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<string> doctorList;
void loadDoctorsFromFile();
const vector<string> CLINIC_TIMES = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};

extern vector<Appointment> appointmentList;

void makeAppointment();
void rescheduleAppointment();
void cancelAppointment();
void displayTimetable();
int getValidatedInt(string prompt, int minVal, int maxVal); 


void appointmentMenu() {
    int choice = 0;
    
    do {
        cout << "\n\t======================================================\n";
        cout << "\t                 APPOINTMENT SCHEDULE                 \n";
        cout << "\t======================================================\n";
        cout << "\t [ 1 ] Make an Appointment\n";
        cout << "\t [ 2 ] Reschedule an Appointment\n";
        cout << "\t [ 3 ] Cancel an Appointment\n";
        cout << "\t [ 4 ] Display Doctor Available Timetable\n";
        cout << "\t [ 5 ] Return to Main Menu\n";
        cout << "\t------------------------------------------------------\n";

        choice = getValidatedInt("\tEnter your choice (1-5): ", 1, 5);

        switch (choice) {
            case 1:
                makeAppointment();
                break;
            case 2:
                rescheduleAppointment();
                break;
            case 3:
                cancelAppointment();
                break;
            case 4:
                displayTimetable();
                break;
            case 5:
                cout << "\n\tReturning to Main Menu...\n";
                break;
        }
    } while (choice != 5);
}

void loadDoctorsFromFile() {
    doctorList.clear();
    ifstream inFile("doctor.txt");
    if (!inFile.is_open()) {
        cout << "\t[ERROR] doctor.txt not found!\n";
        return;
    }
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            doctorList.push_back(line);
        }
    }
    inFile.close();
}

void makeAppointment() {
   cout << "\n\t---------------- MAKE AN APPOINTMENT ----------------\n";
    
    loadDoctorsFromFile();

    Appointment newAppt;
    newAppt.appointmentID = appointmentList.size() + 1001;
    
    newAppt.patientID = getValidatedInt("\tEnter Patient ID (1 - 9999): ", 1, 9999);
    
    cout << "\n\t--- Available Doctors ---\n";
    for (size_t i = 0; i < doctorList.size(); i++) {
        cout << "\t [" << (i + 1) << "] Dr. " << doctorList[i] << "\n";
    }
    
    string prompt = "\tSelect Doctor (1 - " + to_string(doctorList.size()) + "): ";
    int docChoice = getValidatedInt(prompt, 1, doctorList.size());
    newAppt.doctorName = doctorList[docChoice - 1];
    
    cout << "\tEnter Appointment Date (DD/MM/YYYY): ";
    getline(cin, newAppt.date);
    
    cout << "\tEnter Appointment Time (HH:MM): ";
    getline(cin, newAppt.time);
    
    newAppt.status = "Scheduled";
    
    appointmentList.push_back(newAppt);
    
    cout << "\n\t[SUCCESS] Appointment created with Dr. " << newAppt.doctorName << "! ID: " << newAppt.appointmentID << "\n";
}

void rescheduleAppointment() {
    cout << "\n\t[ Reschedule Appointment Module in Progress ]\n";
}

void cancelAppointment() {
    cout << "\n\t[ Cancel Appointment Module in Progress ]\n";
}

void displayTimetable() {
    cout << "\n\t---------------- DOCTOR TIMETABLE ----------------\n";
    
    loadDoctorsFromFile();
    if (doctorList.empty()) {
        cout << "\t[ERROR] No doctors available in doctor.txt.\n";
        return;
    }

    cout << "\n\t--- Available Doctors ---\n";
    for (size_t i = 0; i < doctorList.size(); i++) {
        cout << "\t [" << (i + 1) << "] Dr. " << doctorList[i] << "\n";
    }
    
    string prompt = "\tSelect Doctor (1 - " + to_string(doctorList.size()) + "): ";
    int docChoice = getValidatedInt(prompt, 1, doctorList.size());
    string selectedDoc = doctorList[docChoice - 1];

    string searchDate;
    cout << "\tEnter Date to check (DD/MM/YYYY): ";
    getline(cin, searchDate);

    cout << "\n\t======================================================\n";
    cout << "\t Timetable for Dr. " << selectedDoc << " on " << searchDate << "\n";
    cout << "\t======================================================\n";
    cout << "\t" << left << setw(15) << "Time Slot" << "Status\n";
    cout << "\t------------------------------------------------------\n";

    for (size_t i = 0; i < CLINIC_TIMES.size(); i++) {
        string currentSlot = CLINIC_TIMES[i];
        string status = "Available";

        for (size_t j = 0; j < appointmentList.size(); j++) {
            if (appointmentList[j].doctorName == selectedDoc && 
                appointmentList[j].date == searchDate && 
                appointmentList[j].time == currentSlot &&
                appointmentList[j].status != "Cancelled") {
                
                status = "Booked (Patient ID: " + to_string(appointmentList[j].patientID) + ")";
                break;
            }
        }
        
        cout << "\t" << left << setw(15) << currentSlot << status << "\n";
    }
    cout << "\t======================================================\n";
}