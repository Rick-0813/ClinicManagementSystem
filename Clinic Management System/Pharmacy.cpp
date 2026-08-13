#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Medicine{
    int medicineID;
    string medicineName;
    double price;
    int stockQty;
};

//extern mean this vector already exist in ClinicManagementSystem.cpp
extern vector<Medicine> medicineList;

void pharmacyMenu() {
    cout << " +----------------------------------------+\n";
    cout << " |        PHARMACY & INVENTORY             |\n";
    cout << " +----------------------------------------+\n";
    cout << "\n [Bryan] System in progress...\n";
}