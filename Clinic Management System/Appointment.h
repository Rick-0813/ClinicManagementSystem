#ifndef APPOINTMENT_H
#define APPOINTMENT_H

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

void appointmentMenu();
void makeAppointment();
void rescheduleAppointment();
void cancelAppointment();
void displayTimetable();

#endif