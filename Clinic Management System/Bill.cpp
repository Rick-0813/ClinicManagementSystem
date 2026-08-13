#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct BillItem{
    int medicineID;
    string medicineName;
    int quantity;
    double subtotal;
};

struct Bill{
    int billID;
    int patientID;
    string patientName;
    vector<BillItem> items;
    double totalAmount;
    double taxAmount;
    double discountAmount;
    double amountPaid;
    double change;
    string date;
    string paymentMethod;
};

extern vector<Bill> billList;

void billMenu() {
    cout << " +----------------------------------------+\n";
    cout << " |           BILL & PAYMENT                |\n";
    cout << " +----------------------------------------+\n";
    cout << " | [1]                                     |\n";
    cout << " | [2]                                     |\n";
    cout << " | [3]                                     |\n";
    cout << " | [4]                                     |\n";
    cout << " +----------------------------------------+\n";

}