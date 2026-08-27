#include <iostream>
#include <vector>
#include <string>
#include "Pharmacy.h"

using namespace std;

struct Patient {
    int patientID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

struct Appointment {
    int appointmentID;
    int patientID;
    string doctorName;
    string date;
    string time;
    string status;
};

struct BillItem {
    int medicineID;
    string medicineName;
    int quantity;
    double unitPrice;
    double subtotal;
};

struct Bill {
    int billID;
    int patientID;
    string patientName;
    vector<BillItem> items;
    double totalAmount;
    double taxAmount;
    double discountAmount;
    double finalAmount;
    double amountPaid;
    double change;
    string date;
    string status;
};

vector<Patient> patientList;
vector<Appointment> appointmentList;
vector<Bill> billList;

void patientMenu();
void appointmentMenu();
void billMenu();
void loadPatientsFromFile();
void loadAppointmentsFromFile();
void loadBillsFromFile();
void savePatientsToFile();
void saveAppointmentsToFile();
void saveBillsToFile();

void pauseScreen() {
    cout << "\n Press Enter to Continue ....";
    cin.ignore(1000, '\n');
}

void displayMainMenu() {
    cout << " +============================================================================================================================+\n";
    cout << " |                                                                                                                            |\n";
    cout << " |   /$$   /$$                                                                     /$$$$$$  /$$ /$$          /$$              |\n";
    cout << " |  | $$  | $$                                                                    /$$__  $$| $$|__/         |__/              |\n";
    cout << " |  | $$  | $$  /$$$$$$   /$$$$$$  /$$$$$$/$$$$   /$$$$$$  /$$$$$$$  /$$   /$$  | $$  \\__/| $$ /$$ /$$$$$$$  /$$  /$$$$$$$    |\n";
    cout << " |  | $$$$$$$$ |____  $$ /$$__  $$| $$_  $$_  $$ /$$__  $$| $$__  $$| $$  | $$  | $$      | $$| $$| $$__  $$| $$ /$$_____/    |\n";
    cout << " |  | $$__  $$  /$$$$$$$| $$  \\__/| $$ \\ $$ \\ $$| $$  \\ $$| $$  \\ $$| $$  | $$  | $$      | $$| $$| $$  \\ $$| $$| $$          |\n";
    cout << " |  | $$  | $$ /$$__  $$| $$      | $$ | $$ | $$| $$  | $$| $$  | $$| $$  | $$  | $$    $$| $$| $$| $$  | $$| $$| $$          |\n";
    cout << " |  | $$  | $$|  $$$$$$$| $$      | $$ | $$ | $$|  $$$$$$/| $$  | $$|  $$$$$$$  |  $$$$$$/| $$| $$| $$  | $$| $$|  $$$$$$$    |\n";
    cout << " |  |__/  |__/ \\_______/|__/      |__/ |__/ |__/ \\______/ |__/  |__/ \\____  $$   \\______/ |__/|__/|__/  |__/|__/ \\_______/    |\n";
    cout << " |                                                                     /$$  | $$                                              |\n";
    cout << " |                                                                    |  $$$$$$/                                              |\n";
    cout << " |                                                                     \\______/                                               |\n";
    cout << " |                                                                                                                            |\n";
    cout << " +============================================================================================================================+\n";
    cout << " |  [1] Patient Management                                                                                                    |\n";
    cout << " |  [2] Appointment Schedule                                                                                                  |\n";
    cout << " |  [3] Pharmacy & Inventory                                                                                                  |\n";
    cout << " |  [4] Bill & Payment                                                                                                        |\n";
    cout << " |  [5] Exit                                                                                                                  |\n";
    cout << " +============================================================================================================================+\n";
}

int getValidChoice(int minOption, int maxOption) {
    int choice;
    while (true) {
        cout << " Enter your choice (1-5) : ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n [ERROR] Invalid input! Please enter a number.\n";
            pauseScreen();
            continue;
        }

        cin.ignore(1000, '\n');

        if (choice < minOption || choice > maxOption) {
            cout << "\n [ERROR] Invalid choice! Please enter a number between 1 and 5.\n";
            pauseScreen();
            continue;
        }

        return choice;
    }
}

int main() {
    loadPatientsFromFile();
    loadAppointmentsFromFile();
    loadBillsFromFile();

    medicineList.push_back({ "M001", "Panadol", 6.50, 20 });
    medicineList.push_back({ "M002", "Vitamin C", 10.00, 55 });

    int choice;

    do {
        displayMainMenu();
        choice = getValidChoice(1, 5);
        switch (choice) {
        case 1: {
            patientMenu();
            pauseScreen();
            break;
        }
        case 2: {
            appointmentMenu();
            pauseScreen();
            break;
        }
        case 3: {
            pharmacyMenu();
            pauseScreen();
            break;
        }
        case 4: {
            billMenu();
            pauseScreen();
            break;
        }
        case 5: {
            cout << "\n Saving all data...\n";
            savePatientsToFile();
            saveAppointmentsToFile();
            saveBillsToFile();
            cout << "\n Exiting System. Thank you for using Harmony Clinic! \n";
            break;
        }
        default: {
            cout << "\n [ERROR] Invalid choice! Please enter a number between 1 and 5.\n";
            pauseScreen();
            break;
        }

        }
    } while (choice != 5);

    return 0;
}