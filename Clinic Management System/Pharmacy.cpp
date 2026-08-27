#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Pharmacy.h"

using namespace std;

void pauseScreen();

vector<Medicine> medicineList;

string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

string autoGenerateID() {
    int maxID = 0;
    if (medicineList.empty()) return "M001";

    for (int i = 0; i < medicineList.size(); i++) {
        string idStr = medicineList[i].medicineID.substr(1);
        int currentID = stoi(idStr);
        if (currentID > maxID) maxID = currentID;
    }

    int newID = maxID + 1;
    string numStr = to_string(newID);
    while (numStr.length() < 3) numStr = "0" + numStr;

    return "M" + numStr;
}

bool findMedicineByID(string searchID, Medicine& foundResult) {
    string targetID = toLowerCase(searchID);
    for (int i = 0; i < medicineList.size(); i++) {
        if (toLowerCase(medicineList[i].medicineID) == targetID) {
            foundResult = medicineList[i];
            return true;
        }
    }
    return false;
}

bool createMedicine(int qty, string medicineName, double price) {
    string targetName = toLowerCase(medicineName);
    for (int i = 0; i < medicineList.size(); i++) {
        if (toLowerCase(medicineList[i].medicineName) == targetName) {
            return false;
        }
    }
    Medicine newChar = { autoGenerateID(), medicineName, price, qty };
    medicineList.push_back(newChar);
    saveMedicinesToFile();
    return true;
}

bool updateMedicineByID(string searchID, int newQty, double newPrice) {
    string targetID = toLowerCase(searchID);
    for (int i = 0; i < medicineList.size(); i++) {
        if (toLowerCase(medicineList[i].medicineID) == targetID) {
            medicineList[i].stockQty = newQty;
            medicineList[i].price = newPrice;
            saveMedicinesToFile();
            return true;
        }
    }
    return false;
}

bool deleteMedicineByID(string searchID) {
    string targetID = toLowerCase(searchID);
    for (int i = 0; i < medicineList.size(); i++) {
        if (toLowerCase(medicineList[i].medicineID) == targetID) {
            medicineList.erase(medicineList.begin() + i);
            saveMedicinesToFile();
            return true;
        }
    }
    return false;
}

void addMedicineUI() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |             ADD NEW MEDICINE            |\n";
    cout << " +-----------------------------------------+\n";

    string name;
    int qty;
    double price;

    cout << " Enter Medicine Name (or 'exit' to cancel): ";
    cin.ignore(1000, '\n');
    getline(cin, name);
    if (name == "exit") return;

    if (name.empty()) {
        cout << " [ERROR] Medicine name cannot be empty!\n";
        return;
    }

    cout << " Enter Initial Stock Quantity: ";
    while (!(cin >> qty)) {
        cout << " [ERROR] Invalid quantity! Please enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (qty < 0) {
        cout << " [ERROR] Quantity and price cannot be negative!\n";
        return;
    }

    cout << " Enter Price (RM): ";
    while (!(cin >> price)) {
        cout << " [ERROR] Invalid price! Please enter a valid amount: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (price < 0) {
        cout << " [ERROR] Quantity and price cannot be negative!\n";
        return;
    }

    if (createMedicine(qty, name, price)) {
        cout << "\n [SUCCESS] Medicine '" << name << "' added to inventory successfully!\n";
    }
    else {
        cout << "\n [ERROR] Medicine name already exists in the system!\n";
    }
}

void viewMedicineUI() {
    cout << "\n +============================================================================================================================+\n";
    cout << " |                                                PHARMACY INVENTORY LIST                                                     |\n";
    cout << " +============================================================================================================================+\n";

    if (medicineList.empty()) {
        cout << " | [INFO] Inventory is currently empty.                                                                                       |\n";
        cout << " +============================================================================================================================+\n";
        return;
    }

    cout << " | " << left << setw(15) << "Medicine ID"
        << " | " << left << setw(45) << "Medicine Name"
        << " | " << right << setw(25) << "Stock Quantity"
        << " | " << right << setw(28) << "Unit Price (RM)" << " |\n";
    cout << " +----------------------------------------------------------------------------------------------------------------------------+\n";

    for (int i = 0; i < medicineList.size(); i++) {
        cout << " | " << left << setw(15) << medicineList[i].medicineID
            << " | " << left << setw(45) << medicineList[i].medicineName
            << " | " << right << setw(25) << medicineList[i].stockQty
            << " | " << right << setw(28) << fixed << setprecision(2) << medicineList[i].price << " |\n";
    }
    cout << " +============================================================================================================================+\n";
}

void searchMedicineByID() {
    string searchID;
    cout << " Enter Medicine ID to search (e.g. M001): ";
    cin >> searchID;

    Medicine result;
    if (findMedicineByID(searchID, result)) {
        cout << "\n [SUCCESS] Medicine Found!\n"
            << " ID    : " << result.medicineID << "\n"
            << " Name  : " << result.medicineName << "\n"
            << " Qty   : " << result.stockQty << "\n"
            << " Price : RM " << fixed << setprecision(2) << result.price << "\n";
    }
    else {
        cout << "\n [ERROR] Medicine ID [" << searchID << "] not found in system.\n";
    }
}

void updateMedicineUI() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |             UPDATE MEDICINE             |\n";
    cout << " +-----------------------------------------+\n";

    string searchID;
    cout << " Enter Medicine ID to update (e.g. M001): ";
    cin >> searchID;

    Medicine temp;
    if (!findMedicineByID(searchID, temp)) {
        cout << " [ERROR] Medicine ID [" << searchID << "] not found!\n";
        return;
    }

    cout << "\n Current Info -> Name: " << temp.medicineName
        << " | Qty: " << temp.stockQty
        << " | Price: RM " << fixed << setprecision(2) << temp.price << "\n\n";

    int newQty;
    double newPrice;

    cout << " Enter NEW Stock Quantity: ";
    while (!(cin >> newQty)) {
        cout << " [ERROR] Invalid quantity! Try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (newQty < 0) {
        cout << " [ERROR] Quantity and price cannot be negative!\n";
        return;
    }

    cout << " Enter NEW Price (RM): ";
    while (!(cin >> newPrice)) {
        cout << " [ERROR] Invalid price! Try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (newPrice < 0) {
        cout << " [ERROR] Quantity and price cannot be negative!\n";
        return;
    }

    if (updateMedicineByID(searchID, newQty, newPrice)) {
        cout << "\n [SUCCESS] Medicine [" << temp.medicineName << "] updated successfully!\n";
    }
}

void deleteMedicineUI() {
    cout << "\n +-----------------------------------------+\n";
    cout << " |             DELETE MEDICINE             |\n";
    cout << " +-----------------------------------------+\n";

    string searchID;
    cout << " Enter Medicine ID to delete (e.g. M001): ";
    cin >> searchID;

    Medicine temp;
    if (!findMedicineByID(searchID, temp)) {
        cout << " [ERROR] Medicine ID [" << searchID << "] not found!\n";
        return;
    }

    cout << "\n Target Info -> Name: " << temp.medicineName
        << " | Qty: " << temp.stockQty
        << " | Price: RM " << fixed << setprecision(2) << temp.price << "\n\n";

    char confirm;
    cout << " Are you SURE you want to delete this medicine? (Y/N): ";
    cin >> confirm;

    if (tolower(confirm) == 'y') {
        if (deleteMedicineByID(searchID)) {
            cout << "\n [SUCCESS] Medicine [" << temp.medicineName << "] has been removed!\n";
        }
    }
    else {
        cout << "\n [INFO] Deletion cancelled.\n";
    }
}

void pharmacyMenu() {
    int choice = 0;

    do {
        bool hasLowStock = false;
        for (int i = 0; i < medicineList.size(); i++) {
            if (medicineList[i].stockQty < 50) {
                hasLowStock = true;
                break;
            }
        }

        if (hasLowStock) {
            cout << "\n +----------------------------------------------------------------------------------------------------------------------------+\n";
            cout << " | [!] LOW STOCK ALERT                                                                                                        |\n";
            cout << " +----------------------------------------------------------------------------------------------------------------------------+\n";
            for (int i = 0; i < medicineList.size(); i++) {
                if (medicineList[i].stockQty < 50) {
                    cout << " | -> " << left << setw(20) << medicineList[i].medicineID
                        << " | " << left << setw(74) << medicineList[i].medicineName
                        << " Current Stock: " << right << setw(6) << medicineList[i].stockQty << " |\n";
                }
            }
            cout << " +----------------------------------------------------------------------------------------------------------------------------+\n";
        }

        cout << "\n +============================================================================================================================+\n";
        cout << " |                                                PHARMACY & INVENTORY                                                        |\n";
        cout << " +============================================================================================================================+\n";
        cout << " |  [1] Add New Medicine                                                                                                      |\n";
        cout << " |  [2] View All Medicines                                                                                                    |\n";
        cout << " |  [3] Update Medicine Stock/Price                                                                                           |\n";
        cout << " |  [4] Delete Medicine                                                                                                       |\n";
        cout << " |  [5] Return to Main Menu                                                                                                   |\n";
        cout << " +============================================================================================================================+\n";

        cout << " Enter your choice (1-5): ";
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << " [ERROR] Invalid choice! Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch (choice) {
        case 1:
            addMedicineUI();
            pauseScreen();
            break;
        case 2:
            viewMedicineUI();
            pauseScreen();
            break;
        case 3:
            updateMedicineUI();
            pauseScreen();
            break;
        case 4:
            deleteMedicineUI();
            pauseScreen();
            break;
        case 5:
            break;
        }
    } while (choice != 5);
}

void saveMedicinesToFile() {
    ofstream outFile("medicines.txt");
    if (!outFile.is_open()) {
        cout << " [ERROR] Failed to save medicine records to file!\n";
        return;
    }
    for (int i = 0; i < medicineList.size(); i++) {
        outFile << medicineList[i].medicineID << "|"
            << medicineList[i].medicineName << "|"
            << medicineList[i].price << "|"
            << medicineList[i].stockQty << "\n";
    }
    outFile.close();
}

void loadMedicinesFromFile() {
    ifstream inFile("medicines.txt");
    if (!inFile.is_open()) return;

    medicineList.clear();
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string itemStr;
        Medicine m;

        if (getline(ss, itemStr, '|')) m.medicineID = itemStr;
        if (getline(ss, itemStr, '|')) m.medicineName = itemStr;
        if (getline(ss, itemStr, '|')) m.price = stod(itemStr);
        if (getline(ss, itemStr, '|')) m.stockQty = stoi(itemStr);

        medicineList.push_back(m);
    }
    inFile.close();
}
