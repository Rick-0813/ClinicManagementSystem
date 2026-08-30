#include "Appointment.h"
#include "Patient.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

vector<string> doctorList;
void loadDoctorsFromFile();
const vector<string> CLINIC_TIMES = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };

void makeAppointment();
void rescheduleAppointment();
void cancelAppointment();
void completeAppointment();
void displayTimetable();
void viewPatientAppointments();
int getValidatedInt(string prompt, int minVal, int maxVal);

//SAVING MODULES
void loadAppointmentsFromFile() {
    ifstream inFile("appointments.txt");
    if (!inFile.is_open()) {
        return; // It's completely fine if the file doesn't exist yet on the first run
    }

    appointmentList.clear();
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string itemStr;
        Appointment appt;

        // Extract each piece of data separated by '|'
        if (getline(ss, itemStr, '|')) appt.appointmentID = stoi(itemStr);
        if (getline(ss, itemStr, '|')) appt.patientID = stoi(itemStr);
        if (getline(ss, itemStr, '|')) appt.doctorName = itemStr;
        if (getline(ss, itemStr, '|')) appt.date = itemStr;
        if (getline(ss, itemStr, '|')) appt.time = itemStr;
        if (getline(ss, itemStr, '|')) {
            while (!itemStr.empty() && (itemStr.back() == '\r' || itemStr.back() == '\n' || itemStr.back() == ' ')) {
                itemStr.pop_back();
            }
            appt.status = itemStr;
        }

        appointmentList.push_back(appt);
    }
    inFile.close();
}
void saveAppointmentsToFile() {
    ofstream outFile("appointments.txt");
    if (!outFile.is_open()) {
        cout << " [ERROR] Failed to save appointment records to file!\n";
        return;
    }

    for (int i = 0; i < appointmentList.size(); i++) {
        outFile << appointmentList[i].appointmentID << "|"
            << appointmentList[i].patientID << "|"
            << appointmentList[i].doctorName << "|"
            << appointmentList[i].date << "|"
            << appointmentList[i].time << "|"
            << appointmentList[i].status << "\n";
    }
    outFile.close();
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

//VALIDATIONS
bool isValidDate(const string& date) {
    //10 chrcters
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;

        //check if number
        if (date[i] < '0' || date[i] > '9') {
            return false;
        }
    }

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (year < 2026) return false; 

    //use to check leapYear
    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && isLeap) daysInMonth[1] = 29;

    //check the date is over the maximum day of the month or not
    if (day > daysInMonth[month - 1]) return false;

    return true;
} 
string getValidatedDate(string prompt) {
    string inputDate;
    while (true) {
        cout << prompt;
        getline(cin, inputDate);
        if (isValidDate(inputDate)) {
            return inputDate;
        }
        cout << " [ERROR] Invalid date format! Please use DD/MM/YYYY (e.g., 27/08/2026).\n";
    }
}
string getValidatedTime(string prompt) {
    string inputTime;
    while (true) {
        cout << prompt;
        getline(cin, inputTime);

        for (int i = 0; i < (int)CLINIC_TIMES.size(); i++) {
            if (inputTime == CLINIC_TIMES[i]) {
                return inputTime;
            }
        }

        cout << " [ERROR] Invalid time! Clinic operates hourly from 09:00 to 16:00.\n";
        cout << " Available slots: ";
        for (int i = 0; i < (int)CLINIC_TIMES.size(); i++) {
            cout << CLINIC_TIMES[i] << "  ";
        }
        cout << "\n";
    }
}

//FUNCTIONS
void appointmentMenu() {

    int choice = 0;

    do {
        cout << "\n +-----------------------------------------+\n";
        cout << " |           APPOINTMENT SCHEDULE          |\n";
        cout << " +-----------------------------------------+\n";
        cout << " | [1] Make an Appointment                 |\n";
        cout << " | [2] Reschedule an Appointment           |\n";
        cout << " | [3] Cancel an Appointment               |\n";
        cout << " | [4] Mark Appointment as Completed       |\n";
        cout << " | [5] Display Doctor Available Timetable  |\n";
        cout << " | [6] Return to Main Menu                 |\n";
        cout << " +-----------------------------------------+\n";

        choice = getValidatedInt(" Enter your choice (1-6): ", 1, 6);

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
            completeAppointment();
            break;
        case 5:
            displayTimetable();
            break;
        case 6:
            cout << "\n Returning to Main Menu...\n";
            break;
        }
    } while (choice != 6);
}
void makeAppointment() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |           MAKE AN APPOINTMENT           |\n";
    cout << " +-----------------------------------------+\n";

    if (doctorList.empty()) {
        cout << " [ERROR] No doctors available in system.\n";
        return;
    }

    int pID = getValidatedInt(" Enter Patient ID (1 - 9999, or 0 to cancel): ", 0, 9999);
    if (pID == 0) {
        cout << " [INFO] Operation cancelled. Returning to menu...\n";
        return;
    }

    //Validation
    //use the function form patient module to find the patient based on the pID
    Patient* p = findPatientByID(pID);
    //if p equal to nullptr mean that the pID did not found in any patient ID and display the error message
    if (p == nullptr) {
        cout << " [ERROR] Patient ID " << pID << " not found!\n";
        return;
    }
    //it mean the patient name is take form the pointer p patient name
    string patientName = p->patientName;

    cout << " [INFO] Patient Found: " << patientName << "\n\n";

    Appointment newAppt;
    
    newAppt.appointmentID = appointmentList.empty() ? 1001 : appointmentList.back().appointmentID + 1;
    newAppt.patientID = pID;

    cout << " +-----------------------------------------+\n";
    cout << " |            AVAILABLE DOCTORS            |\n";
    cout << " +-----------------------------------------+\n";
    for (int i = 0; i < doctorList.size(); i++) {
        cout << " | [" << (i + 1) << "] Dr. " << left << setw(30) << doctorList[i] << " |\n";
    }
    cout << " +-----------------------------------------+\n";

    cout << right << setfill(' ');
    string prompt = " Select Doctor (1 - " + to_string(doctorList.size()) + "): ";
    int docChoice = getValidatedInt(prompt, 1, doctorList.size());
    newAppt.doctorName = doctorList[docChoice - 1];

    bool isSlotTaken = true;
    while (isSlotTaken) {
        newAppt.date = getValidatedDate(" Enter Appointment Date (DD/MM/YYYY): ");
        newAppt.time = getValidatedTime(" Enter Appointment Time (HH:MM): ");

        isSlotTaken = false;
        for (int i = 0; i < (int)appointmentList.size(); i++) {
            if (appointmentList[i].doctorName == newAppt.doctorName &&
                appointmentList[i].date == newAppt.date &&
                appointmentList[i].time == newAppt.time &&
                appointmentList[i].status != "Cancelled") {

                isSlotTaken = true;
                cout << " [ERROR] Dr. " << newAppt.doctorName << " is already booked at that time! Please choose another slot.\n\n";
                break;
            }
        }
              
    }


    newAppt.status = "Scheduled";
    appointmentList.push_back(newAppt);
    saveAppointmentsToFile();

    string apptIDStr = "A" + to_string(newAppt.appointmentID);

    cout << "\n +-----------------------------------------------------------+\n";
    cout << " | [SUCCESS] Appointment Created Successfully!               |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Appt ID : " << left << setw(45) << apptIDStr << "   |\n";
    cout << " | Doctor  : Dr. " << left << setw(41) << newAppt.doctorName << "   |\n";
    cout << " | Date    : " << left << setw(45) << newAppt.date << "   |\n";
    cout << " | Time    : " << left << setw(45) << newAppt.time << "   |\n";
    cout << " +-----------------------------------------------------------+\n";
}
void rescheduleAppointment() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |         RESCHEDULE APPOINTMENT          |\n";
    cout << " +-----------------------------------------+\n";

    if (appointmentList.empty()) {
        cout << " [INFO] No appointments available in the system.\n";
        return;
    }
    
    int apptID = getValidatedInt(" Enter Appointment ID to Reschedule (e.g. 1001, or 0 to cancel): ", 0, 99999);
    if (apptID == 0) return;

    int apptIndex = -1;
    for (int i = 0; i < (int)appointmentList.size(); i++) {
        if (appointmentList[i].appointmentID == apptID) {
            apptIndex = i;
            break;
        }
    }

    if (apptIndex == -1) {
        cout << " [ERROR] Appointment ID A" << apptID << " not found!\n";
        return;
    }

    if (appointmentList[apptIndex].status == "Cancelled") {
        cout << " [ERROR] Cannot reschedule a Cancelled appointment!\n";
        return;
    }

    cout << "\n Current Appointment Details:\n";
    cout << " Doctor : Dr. " << appointmentList[apptIndex].doctorName << "\n";
    cout << " Date   : " << appointmentList[apptIndex].date << "\n";
    cout << " Time   : " << appointmentList[apptIndex].time << "\n\n";

    bool isSlotTaken = true;
    string newDate, newTime;
    while (isSlotTaken) {
        newDate = getValidatedDate(" Enter New Date (DD/MM/YYYY): ");
        newTime = getValidatedTime(" Enter New Time (HH:MM): ");

        isSlotTaken = false;
        for (int i = 0; i < (int)appointmentList.size(); i++) {
            // Check if slot is taken (make sure we don't check against the current appointment itself!)
            if (i != apptIndex &&
                appointmentList[i].doctorName == appointmentList[apptIndex].doctorName &&
                appointmentList[i].date == newDate &&
                appointmentList[i].time == newTime &&
                appointmentList[i].status != "Cancelled") {

                isSlotTaken = true;
                break;
            }
        }
    }
    appointmentList[apptIndex].date = newDate;
    appointmentList[apptIndex].time = newTime;
    appointmentList[apptIndex].status = "Rescheduled";

    saveAppointmentsToFile();

    cout << " [SUCCESS] Appointment A" << apptID << " successfully rescheduled to " << newDate << " at " << newTime << "!\n";
}
void cancelAppointment() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |           CANCEL APPOINTMENT            |\n";
    cout << " +-----------------------------------------+\n";

    if (appointmentList.empty()) {
        cout << " [INFO] No appointments available in the system.\n";
        return;
    }

    int apptID = getValidatedInt(" Enter Appointment ID to Cancel (e.g. 1001, or 0 to go back): ", 0, 99999);
    if (apptID == 0) return;

    int apptIndex = -1;
    for (int i = 0; i < (int)appointmentList.size(); i++) {
        if (appointmentList[i].appointmentID == apptID) {
            apptIndex = i;
            break;
        }
    }

    if (apptIndex == -1) {
        cout << " [ERROR] Appointment ID A" << apptID << " not found!\n";
        return;
    }

    if (appointmentList[apptIndex].status == "Cancelled") {
        cout << " [ERROR] This appointment is already Cancelled!\n";
        return;
    }

    if (appointmentList[apptIndex].status == "Completed") {
        cout << " [ERROR] Cannot cancel an appointment that is already Completed!\n";
        return;
    }

    cout << "\n Current Appointment Details:\n";
    cout << " Doctor : Dr. " << appointmentList[apptIndex].doctorName << "\n";
    cout << " Date   : " << appointmentList[apptIndex].date << "\n";
    cout << " Time   : " << appointmentList[apptIndex].time << "\n\n";

    cout << " Are you sure you want to cancel this appointment? (Y/N): ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        appointmentList[apptIndex].status = "Cancelled";
        saveAppointmentsToFile();
        cout << " [SUCCESS] Appointment A" << apptID << " successfully cancelled!\n";
    }
    else {
        cout << " [INFO] Cancellation aborted.\n";
    }
}
void completeAppointment() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |          COMPLETE APPOINTMENT           |\n";
    cout << " +-----------------------------------------+\n";

    //check the appointment list is empty or not if is empty then show info message
    if (appointmentList.empty()) {
        cout << " [INFO] No appointments available in the system.";
        return;
    }

    int apptID = getValidatedInt(" Enter Appointment ID to Complete (e.g. 1001, or 0 to go back): ", 0, 99999);

    if (apptID == 0) 
        return;

    int apptIndex = -1;
    //find the appointment based on the apptID
    for (int i = 0; i < (int)appointmentList.size(); i++) {
        if (appointmentList[i].appointmentID == apptID) {
            // If found, save the index and stop searching
            apptIndex = i;
            break;
        }
    }

    //if not found mean it apptIndex did not change which is -1 then print the error message
    if (apptIndex == -1) {
        cout << " [ERROR] Appointment ID A" << apptID << " not found!\n";
        return;
    }

    //check the appointment status
    if (appointmentList[apptIndex].status == "Cancelled") {
        cout << " [ERROR] Cannot complete a Cancelled appointment!\n";
        return;
    }
    if (appointmentList[apptIndex].status == "Completed") {
        cout << " [ERROR] This appointment is already marked as Completed!\n";
        return;
    }

    //display the information about the appointment
    cout << "\n Current Appointment Details:\n";
    cout << " Doctor : Dr. " << appointmentList[apptIndex].doctorName << "\n";
    cout << " Date   : " << appointmentList[apptIndex].date << "\n";
    cout << " Time   : " << appointmentList[apptIndex].time << "\n\n";

    cout << " Mark this appointment as Completed? (Y/N): ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        appointmentList[apptIndex].status = "Completed";
        saveAppointmentsToFile();
        cout << " [SUCCESS] Appointment A" << apptID << " successfully marked as Completed!\n";
    }
    else {
        cout << " [INFO] Operation aborted.\n";
    }
}
void displayTimetable() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |            DOCTOR TIMETABLE             |\n";
    cout << " +-----------------------------------------+\n";

    if (doctorList.empty()) {
        cout << " [ERROR] No doctors available in doctor.txt.\n";
        return;
    }

    cout << " +-----------------------------------------+\n";
    cout << " |            AVAILABLE DOCTORS            |\n";
    cout << " +-----------------------------------------+\n";
    for (int i = 0; i < doctorList.size(); i++) {
        cout << " | [" << (i + 1) << "] Dr. " << left << setw(30) << doctorList[i] << "  |\n";
    }
    cout << " +-----------------------------------------+\n";

    cout << right << setfill(' ');
    string prompt = " Select Doctor (1 - " + to_string(doctorList.size()) + "): ";
    int docChoice = getValidatedInt(prompt, 1, doctorList.size());
    string selectedDoc = doctorList[docChoice - 1];

    string searchDate = getValidatedDate(" Enter Date to check (DD/MM/YYYY): ");

    cout << "\n +------------------------------------------------------+\n";
    cout << " | Timetable for Dr. " << left << setw(18) << selectedDoc << " on " << setw(10) << searchDate << "   |\n";
    cout << " +------------------------------------------------------+\n";
    cout << " | " << left << setw(15) << "Time Slot" << "| " << setw(34) << "Status" << "  |\n";
    cout << " +------------------------------------------------------+\n";

    for (int i = 0; i < CLINIC_TIMES.size(); i++) {
        string currentSlot = CLINIC_TIMES[i];
        string status = "Available";

        for (int j = 0; j < appointmentList.size(); j++) {
            if (appointmentList[j].doctorName == selectedDoc &&
                appointmentList[j].date == searchDate &&
                appointmentList[j].time == currentSlot &&
                appointmentList[j].status != "Cancelled") {

                string pName = "Unknown";
                for (int k = 0; k < (int)patientList.size(); k++) {
                    if (patientList[k].patientID == appointmentList[j].patientID) {
                        pName = patientList[k].patientName;
                        break;
                    }
                }
                stringstream ss_pid;
                ss_pid << "P" << right << setfill('0') << setw(4) << appointmentList[j].patientID;
                status = "Booked (" + ss_pid.str() + " - " + pName + ")";
                cout << setfill(' ');
                break;

            }
        }

        cout << " | " << left << setw(15) << currentSlot << "| " << setw(34) << status << "  |\n";
    }
    cout << " +------------------------------------------------------+\n";
}
void viewPatientAppointments() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |        VIEW PATIENT APPOINTMENTS        |\n";
    cout << " +-----------------------------------------+\n";

    if (appointmentList.empty()) {
        cout << " [INFO] No appointments available in the system.\n";
        return;
    }

    int pID = getValidatedInt(" Enter Patient ID (1 - 9999, or 0 to cancel): ", 0, 9999);
    if (pID == 0) return;

    string pName = "Unknown";
    bool patientExists = false;
    for (int i = 0; i < (int)patientList.size(); i++) {
        if (patientList[i].patientID == pID) {
            pName = patientList[i].patientName;
            patientExists = true;
            break;
        }
    }
    if (!patientExists) {
        cout << " [ERROR] Patient ID " << pID << " not found!\n";
        return;
    }
    stringstream ss_pid;
    ss_pid << "P" << setfill('0') << setw(4) << pID;
    string searchStr = ss_pid.str() + " - " + pName;

    cout << "\n +--------------------------------------------------------------------------+\n";
    cout << " | Appointments for: " << left << setfill(' ') << setw(54) << searchStr << " |\n";
    cout << " +--------------------------------------------------------------------------+\n";
    cout << " | Appt ID | Doctor Name          | Date       | Time  | Status         |\n";
    cout << " +--------------------------------------------------------------------------+\n";

    bool hasAppt = false;
    for (int i = 0; i < (int)appointmentList.size(); i++) {
        if (appointmentList[i].patientID == pID) {
            hasAppt = true;
            string apptIDStr = "A" + to_string(appointmentList[i].appointmentID);

            cout << " | " << left << setw(7) << apptIDStr
                << " | Dr. " << setw(15) << appointmentList[i].doctorName
                << " | " << setw(10) << appointmentList[i].date
                << " | " << setw(5) << appointmentList[i].time
                << " | " << setw(14) << appointmentList[i].status << " |\n";
        }
    }
    if (!hasAppt) {
        cout << " | " << setw(72) << "No appointments found for this patient." << " |\n";
    }
    cout << " +--------------------------------------------------------------------------+\n";
}