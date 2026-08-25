#include <iostream>
#include <string>
#include <vector>
#include<sstream>
#include<iomanip>
using namespace std;

struct BillItem{
    int medicineID;
    string medicineName;
    int quantity;
    double subtotal;
};

struct Patient {
    int patinetID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

struct Medicine {
    int medicineId;
    string medicineName;
    double price;
    int stockQty;
};

struct Bill{
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
    string paymentMethod;
    string status;
};

extern vector<Patient> patientList;
extern vector<Medicine> medicineList;
extern vector<Bill> billList;

const double SST_RATE = 0.06;
const double SENIOR_DISCOUNT_RATE = 0.10;
const int SENIOR_AGE = 65;

//utilize function
int readInt(const string &prompt) { 
    
    // using prompt to get a message to display and use const to avoid the prompt message change 
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << " [ERROR] Please enter a valid number  ! \n";
            continue;
        }
        cin.ignore(1000, '\n');
        return value;
    }
}

string formatMedicineID(int id) {
    stringstream ss;
    ss << "M" << setfill('0') << setw(3) << id;
    return ss.str();
}

void displayMedicineTable() {
    cout << " +------------------------------------------------------------------+\n";
    cout << " | Medicine ID | Medicine Name        | Price (RM) | Stock          |\n";
    cout << " +------------------------------------------------------------------+\n";
    for (int i = 0; i < (int)medicineList.size(); i++) {
        cout << " | " << left << setw(12) << formatMedicineID(medicineList[i].medicineId)
            << " | " << left << setw(22) << medicineList[i].medicineName
            << " | " << right << setw(11) << fixed << setprecision(2) << medicineList[i].price
            << " | " << right << setw(15) << medicineList[i].stockQty;
    }
    cout << " +------------------------------------------------------------------+\n";
}


//function 1 
void generateBIll() {
    cout << " +----------------------------------------+\n";
    cout << " |             GENERATE BILL              |\n";
    cout << " +----------------------------------------+\n";

    int pID = readInt("Enter Patient ID :");

    //check pateintID
    int pIndex = -1;
    for (int i = 0; i < (int)patientList.size(); i++) {
        if (patientList[i].patinetID = pID) {
            pIndex = i;
            break;
        }
    }

    //error message 
    if (pIndex == -1) {
        cout << " [ERROR] Pateint Not Found. Please check the ID and try it again. \n";
        return;
    }
    
    Patient p = patientList[pIndex];

    Bill newBill;
    newBill.billID = (int)billList.size() + 1;
    newBill.patientID = p.patinetID;
    newBill.patientName = p.patientName;
    newBill.totalAmount = 0.0;
    newBill.status = "Unpaid";

    while (true) {
        displayMedicineTable();

        int mID = readInt("Ener Medicine ID number to add (e.g. 1 for M001 and 0 to finish) :");
        if (mID == 0) {
            break;
        }

        //Check Medicine 
        int mIndex = -1;
        for (int i = 0; i < medicineList.size(); i++) {
            if (medicineList[i].medicineId == mIndex) {
                mIndex = i;
                break;
            }
        }

        //Error message
        if (mIndex == -1) {
            cout << " [ERROR] Medicine Not Found! \n";
            continue;
        }
        
        int qty = readInt("Enter Quantity :");

        //Error message 
        if (qty <= 0) {
            cout << " [ERROR] Quantity must be greater than 0. \n";
            continue;
        }
        //Error message
        if (qty > medicineList[mIndex].stockQty) {
            cout << " [ERROR] Not enough stock! Only" << medicineList[mIndex].stockQty << "left. \n";
            continue;
        }

        //store each medicine to item and item add to bill.items (store all of the medicine)
        BillItem item;
        item.medicineID = medicineList[mIndex].medicineId;
        item.medicineName = medicineList[mIndex].medicineName;
        item.quantity = qty;
        item.subtotal = qty * medicineList[mIndex].price;

        //add the entire item to items in newBill
        newBill.items.push_back(item);
        //add the subtotal to totalAmount 
        newBill.totalAmount += item.subtotal;

        //deduct stock 
        medicineList[mIndex].stockQty -= qty;

        cout << " Added :" << medicineList[mIndex].medicineName << "*" << qty << " = RM" << item.subtotal << "\n";
    }

    if (newBill.items.empty()) {
        cout << "\n No items added. Bill cancelled. \n";
        return;
    }

    newBill.taxAmount = newBill.totalAmount * SST_RATE;
    if (p.age > SENIOR_AGE) {
        newBill.discountAmount = newBill.totalAmount * SENIOR_DISCOUNT_RATE;
    }
    else {
        newBill.discountAmount = 0.0;
    }

    newBill.finalAmount = newBill.totalAmount + newBill.taxAmount - newBill.discountAmount;
    //add the newBill to billList 
    billList.push_back(newBill);

    stringstream ss_id;
    ss_id << "B" << setfill('0') << setw(3) << newBill.billID;
    string billStr = ss_id.str();

    cout << " +----------------------------------------+\n";
    cout << " |        Bill Summary (Bill ID: " << left << setw(12) << billStr << "|\n";
    cout << " +----------------------------------------+\n";
    cout << " | Patient Name : " << newBill.patientName << "\n";
    cout << " Patient ID : " << newBill.patientID << "\n";
}


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