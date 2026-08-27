#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Medicine {
    string medicineID;
    string medicineName;
    double price;
    int stockQty;
};

extern vector<Medicine> medicineList;

void pharmacyMenu();