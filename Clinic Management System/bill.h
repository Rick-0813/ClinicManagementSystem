#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
    string status;
};

extern vector<Bill> billList;

void billMenu();
void loadBillsFromFile();
void saveBillsToFile();