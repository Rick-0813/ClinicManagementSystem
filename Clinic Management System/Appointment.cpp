#include <iostream>
#include <string>
#include <vector>

using namespace std;



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

void makeAppointment() {
    cout << "\n\t[ Make Appointment Module in Progress ]\n";
}

void rescheduleAppointment() {
    cout << "\n\t[ Reschedule Appointment Module in Progress ]\n";
}

void cancelAppointment() {
    cout << "\n\t[ Cancel Appointment Module in Progress ]\n";
}

void displayTimetable() {
    cout << "\n\t[ Display Timetable Module in Progress ]\n";
}