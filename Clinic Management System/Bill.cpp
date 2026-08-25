#include "bill.h"
#include<sstream>
#include<iomanip>
#include <fstream>
using namespace std;

struct Patient {
    int patientID;
    string patientName;
    int age;
    string allergy;
    vector<string> medicalHistory;
};

struct Medicine {
    int medicineID;
    string medicineName;
    double price;
    int stockQty;
};

extern vector<Patient> patientList;
extern vector<Medicine> medicineList;

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

double readDouble(const string &prompt) {

    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << " [ERROR] Please enter a valid amount! \n";
            continue;
        }
        cin.ignore(1000, '\n');
        return value;
    }
}

int getPatientAge(int pID) {
    for (int i = 0; i < (int)patientList.size(); i++) {
        if (patientList[i].patientID == pID) {
            return patientList[i].age;
        }
    }
    return 0;   
}

string formatMedicineID(int id) {
    stringstream ss;
    ss << "M" << setfill('0') << setw(3) << id;
    return ss.str();
}

void displayMedicineTable() {
    cout << " +--------------------------------------------------------------------+\n";
    cout << " | " << left << setw(12) << "Medicine ID"
        << "| " << left << setw(22) << "Medicine Name"
        << "| " << right << setw(11) << "Price (RM)"
        << " | " << right << setw(14) << "Stock" << " |\n";
    cout << " +--------------------------------------------------------------------+\n";
    for (int i = 0; i < (int)medicineList.size(); i++) {
        cout << " | " << left << setw(12) << formatMedicineID(medicineList[i].medicineID)
            << "| " << left << setw(22) << medicineList[i].medicineName
            << "| " << right << setw(11) << fixed << setprecision(2) << medicineList[i].price
            << " | " << right << setw(14) << medicineList[i].stockQty << " |\n";
    }
    cout << " +--------------------------------------------------------------------+\n";
}

void saveBillsToFile() {
    ofstream outFile("bills.txt");
    if (!outFile.is_open()) {
        cout << " [ERROR] Failed to save bill records to file!\n";
        return;
    }

    for (size_t i = 0; i < billList.size(); i++) {
        Bill b = billList[i];
        outFile << b.billID << "|"
            << b.patientID << "|"
            << b.patientName << "|"
            << b.totalAmount << "|"
            << b.taxAmount << "|"
            << b.discountAmount << "|"
            << b.finalAmount << "|"
            << b.amountPaid << "|"
            << b.change << "|"
            << (b.date.empty() ? "N/A" : b.date) << "|"
            << b.status << "|"
            << b.items.size() << "|";

        for (size_t j = 0; j < b.items.size(); j++) {
            outFile << b.items[j].medicineID << ","
                << b.items[j].medicineName << ","
                << b.items[j].quantity << ","
                << b.items[j].unitPrice << ","
                << b.items[j].subtotal;

            if (j + 1 < b.items.size()) {
                outFile << ";";
            }
        }
        outFile << "\n"; 
    }
    outFile.close();
}

void loadBillsFromFile() {
    ifstream inFile("bills.txt");
    if (!inFile.is_open()) {
        return;
    }

    billList.clear();
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string itemStr;
        Bill b;

        if (getline(ss, itemStr, '|')) b.billID = stoi(itemStr);
        if (getline(ss, itemStr, '|')) b.patientID = stoi(itemStr);
        if (getline(ss, itemStr, '|')) b.patientName = itemStr;
        if (getline(ss, itemStr, '|')) b.totalAmount = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.taxAmount = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.discountAmount = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.finalAmount = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.amountPaid = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.change = stod(itemStr);
        if (getline(ss, itemStr, '|')) b.date = itemStr;
        if (getline(ss, itemStr, '|')) b.status = itemStr;

        int itemCount = 0;
        if (getline(ss, itemStr, '|')) itemCount = stoi(itemStr);

        string allItems;
        if (getline(ss, allItems)) { 
            stringstream itemsSS(allItems);
            string singleItemStr;

            while (getline(itemsSS, singleItemStr, ';')) {
                if (!singleItemStr.empty()) {
                    stringstream singleSS(singleItemStr);
                    string field;
                    BillItem bi;

                    if (getline(singleSS, field, ',')) bi.medicineID = stoi(field);
                    if (getline(singleSS, field, ',')) bi.medicineName = field;
                    if (getline(singleSS, field, ',')) bi.quantity = stoi(field);
                    if (getline(singleSS, field, ',')) bi.unitPrice = stod(field);
                    if (getline(singleSS, field, ',')) bi.subtotal = stod(field);

                    b.items.push_back(bi);
                }
            }
        }
        billList.push_back(b);
    }
    inFile.close();
}

//-------------------------------------------------------------------------------------------

//function 1.1
void searchUnpaidBills() {
    cout << "\n +----------------------------------------+\n";
    cout << " |          SEARCH UNPAID BILLS           |\n";
    cout << " +----------------------------------------+\n";

    //get patient ID
    int pID = readInt(" Enter Patient ID (or enter 0 to cancel) : ");
    if (pID == 0) {
        cout << " [INFO] Operation cancelled.\n";
        return;
    }

    bool found = false;
    cout << "\n +-----------------------------------------------------------------+\n";
    cout << " | Bill ID | Patient Name         | Final Amount   | Status        |\n";
    cout << " +-----------------------------------------------------------------+\n";

    //find the patient and check the bill status based on patient ID
    for (int i = 0; i < (int)billList.size(); i++) {
        if (billList[i].patientID == pID && billList[i].status == "Unpaid") {
            found = true;
            stringstream ss_id;
            ss_id << "B" << setfill('0') << setw(3) << billList[i].billID;

            cout << " | " << left << setw(7) << ss_id.str()
                << " | " << left << setw(20) << billList[i].patientName
                << " | RM " << right << setw(11) << fixed << setprecision(2) << billList[i].finalAmount
                << " | " << left << setw(13) << billList[i].status << " |\n";
            cout << " +-----------------------------------------------------------------+\n";
        }
    }

    if (!found) {
        cout << " [INFO] No unpaid bills found for Patient ID " << pID << ".\n";
    }
}

//function 1.2
void createNewBill() {
    cout << "\n +----------------------------------------+\n";
    cout << " |             CREATE NEW BILL            |\n";
    cout << " +----------------------------------------+\n";

    //get patient ID
    int pID = readInt(" Enter Patient ID (or enter 0 to cancel) : ");
    cout << "\n";

    if (pID == 0) {
        cout << " [INFO] Operation cancelled. Returning to menu...\n";
        return;
    }

    //find the pateint based on ID
    int pIndex = -1;
    for (int i = 0; i < (int)patientList.size(); i++) {
        if (patientList[i].patientID == pID) {
            pIndex = i;
            break;
        }
    }

    //error message
    if (pIndex == -1) {
        cout << " [ERROR] Patient Not Found. Please check the ID and try it again. \n";
        return;
    }

    Patient p = patientList[pIndex];

    Bill newBill;
    newBill.billID = (int)billList.size() + 1;
    newBill.patientID = p.patientID;
    newBill.patientName = p.patientName;
    newBill.totalAmount = 0.0;
    newBill.status = "Unpaid";

    while (true) {
        displayMedicineTable();

        int mID = readInt(" Enter Medicine ID number to add (e.g. 1 for M001 and 0 to finish) : ");
        if (mID == 0) {
            break;
        }

        int mIndex = -1;
        for (int i = 0; i < medicineList.size(); i++) {
            if (medicineList[i].medicineID == mID) {
                mIndex = i;
                break;
            }
        }

        //error message
        if (mIndex == -1) {
            cout << " [ERROR] Medicine Not Found! \n";
            continue;
        }

        int qty = readInt(" Enter Quantity : ");

        //error message
        if (qty <= 0) {
            cout << " [ERROR] Quantity must be greater than 0. \n";
            continue;
        }
        if (qty > medicineList[mIndex].stockQty) {
            cout << " [ERROR] Not enough stock! Only " << medicineList[mIndex].stockQty << " left. \n";
            continue;
        }

        BillItem item;
        item.medicineID = medicineList[mIndex].medicineID;
        item.medicineName = medicineList[mIndex].medicineName;
        item.quantity = qty;
        item.unitPrice = medicineList[mIndex].price;
        item.subtotal = qty * medicineList[mIndex].price;

        newBill.items.push_back(item);
        newBill.totalAmount += item.subtotal;
        medicineList[mIndex].stockQty -= qty;

        cout << " Added : " << medicineList[mIndex].medicineName << " * " << qty << " = RM" << item.subtotal << "\n";
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
    billList.push_back(newBill);

    stringstream ss_id;
    ss_id << "B" << setfill('0') << setw(3) << newBill.billID;
    string billStr = ss_id.str();

    cout << "\n +-----------------------------------------------------------+\n";
    cout << " | " << left << setw(57) << ("Bill Summary (Bill ID: " + billStr + ")") << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Patient ID   : " << left << setw(42) << newBill.patientID << " |\n";
    cout << " | Patient Name : " << left << setw(42) << newBill.patientName << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Medicine Name          | Qty   | Unit Price  | Subtotal   |\n";
    cout << " +-----------------------------------------------------------+\n";

    for (int i = 0; i < (int)newBill.items.size(); i++) {
        cout << " | " << left << setw(22) << newBill.items[i].medicineName
            << " | " << right << setw(5) << newBill.items[i].quantity
            << " | RM " << right << setw(8) << fixed << setprecision(2) << newBill.items[i].unitPrice
            << " | RM " << right << setw(7) << newBill.items[i].subtotal << " |\n";
    }

    cout << " +-----------------------------------------------------------+\n";
    cout << " | Total Amount           : RM " << right << setw(29) << fixed << setprecision(2) << newBill.totalAmount << " |\n";
    cout << " | SST Tax (6%)           : RM " << right << setw(29) << newBill.taxAmount << " |\n";
    cout << " | Senior Discount        : RM " << right << setw(29) << newBill.discountAmount << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Final Amount (Total)   : RM " << right << setw(29) << newBill.finalAmount << " |\n";
    cout << " | Status                 : " << left << setw(32) << newBill.status << " |\n";
    cout << " +-----------------------------------------------------------+\n\n";

    saveBillsToFile();
}

//function 1 
void generateBIll() {
    int choice;
    do {
        cout << "\n +-----------------------------------------+\n";
        cout << " |          BILL GENERATION MENU           |\n";
        cout << " +-----------------------------------------+\n";
        cout << " | [1] Create a New Bill                   |\n";
        cout << " | [2] Check Unpaid Bills by Patient ID    |\n";
        cout << " | [0] Back to Bill Menu                   |\n";
        cout << " +-----------------------------------------+\n";

        choice = readInt(" Enter your choice (0-2): ");

        if (choice == 1) {
            createNewBill();
        }
        else if (choice == 2) {
            searchUnpaidBills();
        }
        else if (choice == 0) {
            cout << "\n Returning to Main Bill Menu...\n";
        }
        else {
            cout << "\n [ERROR] Invalid choice!\n";
        }
    } while (choice != 0);
}

//function 2
void makePayment() {
    cout << "\n +----------------------------------------+\n";
    cout << " |              MAKE PAYMENT              |\n";
    cout << " +----------------------------------------+\n";

    //check any bill or not
    if (billList.empty()) {
        cout << " [INFO] No bills available in the system.\n";
        return;
    }

    int bID = readInt(" Enter Bill ID to pay (e.g., 1 for B001, or 0 to cancel) : ");
    cout << "\n";

    //return to the menu 
    if (bID == 0) {
        cout << " [INFO] Operation cancelled. Returning to menu...\n";
        return;
    }

    //find the bill
    int bIndex = -1;
    for (int i = 0; i < (int)billList.size(); i++) {
        if (billList[i].billID == bID) {
            bIndex = i;
            break;
        }
    }

    //error message 
    if (bIndex == -1) {
        cout << " [ERROR] Bill Not Found. Please check the ID and try again.\n";
        return;
    }

    //check the bill status
    if (billList[bIndex].status != "Unpaid") {
        cout << " [ERROR] This bill cannot be paid! Current status: " << billList[bIndex].status << "\n";
        return;
    }

    stringstream ss_id;
    ss_id << "B" << setfill('0') << setw(3) << billList[bIndex].billID;
    string billStr = ss_id.str();

    cout << " +-----------------------------------------------------------+\n";
    cout << " | " << left << setw(57) << ("Bill Details (Bill ID: " + billStr + ")") << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Patient ID   : " << left << setw(42) << billList[bIndex].patientID << " |\n";
    cout << " | Patient Name : " << left << setw(42) << billList[bIndex].patientName << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Medicine Name          | Qty   | Unit Price  | Subtotal   |\n";

    for (int i = 0; i < (int)billList[bIndex].items.size(); i++) {
        cout << " | " << left << setw(22) << billList[bIndex].items[i].medicineName
            << " | " << right << setw(5) << billList[bIndex].items[i].quantity
            << " | RM " << right << setw(8) << fixed << setprecision(2) << billList[bIndex].items[i].unitPrice
            << " | RM " << right << setw(7) << billList[bIndex].items[i].subtotal << " |\n";
    }

    cout << " +-----------------------------------------------------------+\n";
    cout << " | Total Amount           : RM " << right << setw(29) << fixed << setprecision(2) << billList[bIndex].totalAmount << " |\n";
    cout << " | SST Tax (6%)           : RM " << right << setw(29) << billList[bIndex].taxAmount << " |\n";
    cout << " | Senior Discount        : RM " << right << setw(29) << billList[bIndex].discountAmount << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Final Amount Payable   : RM " << right << setw(29) << billList[bIndex].finalAmount << " |\n";
    cout << " +-----------------------------------------------------------+\n";

    //enter paid amount and check 
    double paid = 0.0;
    while (true) {
        paid = readDouble("\n Enter Payment Amount (RM) : ");
        if (paid < billList[bIndex].finalAmount) {
            cout << " [ERROR] Insufficient Payment! Amount must be at least RM " << billList[bIndex].finalAmount << "\n";
        }
        else {
            break;
        }
    }

    //update the data
    billList[bIndex].amountPaid = paid;
    billList[bIndex].change = paid - billList[bIndex].finalAmount;
    billList[bIndex].status = "Paid";

    //print receipt 
    cout << "\n +-----------------------------------------------------------+\n";
    cout << " |                      PAYMENT RECEIPT                      |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Bill ID            : " << left << setw(36) << billStr << " |\n";
    cout << " | Patient Name       : " << left << setw(36) << billList[bIndex].patientName << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " | Final Amount       : RM " << right << setw(33) << fixed << setprecision(2) << billList[bIndex].finalAmount << " |\n";
    cout << " | Total Paid         : RM " << right << setw(33) << billList[bIndex].amountPaid << " |\n";
    cout << " | Change             : RM " << right << setw(33) << billList[bIndex].change << " |\n";
    cout << " | Status             : " << left << setw(34) << billList[bIndex].status << " |\n";
    cout << " +-----------------------------------------------------------+\n";
    cout << " [SUCCESS] Payment completed successfully!\n";

    saveBillsToFile();
}

//function 3.1 
void viewAllBills() {
    cout << setfill(' ');

    cout << "\n +-----------------------------------------------------------------------------------------+\n";
    cout << " |                                   ALL BILL RECORDS                                      |\n";
    cout << " +-----------------------------------------------------------------------------------------+\n";

    //if the bill is emtpy show this message
    if (billList.empty()) {
        cout << " [INFO] No bill records found in the system.\n";
        return;
    }
    cout << " | Bill ID | Patient ID | Patient Name    | Age | Final Amount (RM) | Status               |\n";
    cout << " +-----------------------------------------------------------------------------------------+\n";

    //use for loop to access each data 
    for (int i = 0; i < (int)billList.size(); i++) {
        stringstream ss_id;
        ss_id << "B" << setfill('0') << setw(3) << billList[i].billID;

        int pAge = getPatientAge(billList[i].patientID);

        string displayName = billList[i].patientName;
        if (displayName.length() > 15) {
            displayName = displayName.substr(0, 12) + "...";
        }

        string displayStatus = billList[i].status;
        if (displayStatus.length() > 20) {
            displayStatus = displayStatus.substr(0, 17) + "...";
        }

        cout << " | " << setfill(' ') << left << setw(7) << ss_id.str()
            << " | " << left << setw(10) << billList[i].patientID
            << " | " << left << setw(15) << displayName
            << " | " << left << setw(3) << pAge
            << " | " << right << setw(17) << fixed << setprecision(2) << billList[i].finalAmount
            << " | " << left << setw(20) << displayStatus << " |\n";
    }
    cout << " +-----------------------------------------------------------------------------------------+\n";
}

//function 3.2 recalculate 
void recalculateBill(int bIndex) {
    //calculate again the bill after edit the bill
    double newTotal = 0.0;
    for (int i = 0; i < (int)billList[bIndex].items.size(); i++) {
        newTotal += billList[bIndex].items[i].subtotal;
    }

    billList[bIndex].totalAmount = newTotal;
    billList[bIndex].taxAmount = newTotal * SST_RATE;

    int pAge = getPatientAge(billList[bIndex].patientID);
    if (pAge > SENIOR_AGE) {
        billList[bIndex].discountAmount = newTotal * SENIOR_DISCOUNT_RATE;
    }
    else {
        billList[bIndex].discountAmount = .0;
    }

    billList[bIndex].finalAmount = newTotal + billList[bIndex].taxAmount - billList[bIndex].discountAmount;
}

//function 3.2
void editBill() {
        cout << "\n +----------------------------------------+\n";
        cout << " |               EDIT BILL                |\n";
        cout << " +----------------------------------------+\n";

        int bID = readInt(" Enter Bill ID to Edit (e.g., 1 for B001, or 0 to cancel) : ");

        //exit edit menu
        if (bID == 0) {
            cout << " [INFO] Operation cancelled. Returning to menu...\n";
            return;
        }

        int bIndex = -1;
        for (int i = 0; i < (int)billList.size(); i++) {
            if (billList[i].billID == bID) { 
                bIndex = i; 
                break;
            }
        }

        //error message
        if (bIndex == -1) {
            cout << " [ERROR] Bill Not Found.\n";
            return;
        }

        //only allow to edit with the bill is unpaid
        if (billList[bIndex].status != "Unpaid") {
            cout << " [ERROR] Only 'Unpaid' bills can be edited! Current status: " << billList[bIndex].status << "\n";
            return;
        }

        while (true) {
            //display the detail for the bill
            cout << "\n Current Bill Details (Bill ID: B" << setfill('0') << setw(3) << bID << "):\n";
            cout << " Assigned Patient ID   : " << billList[bIndex].patientID << "\n";
            cout << " Assigned Patient Name : " << billList[bIndex].patientName << "\n";
            cout << " Current Final Amount  : RM " << fixed << setprecision(2) << billList[bIndex].finalAmount << "\n\n";

            cout << " Items in Bill:\n";
            for (int i = 0; i < (int)billList[bIndex].items.size(); i++) {
                cout << " [" << i + 1 << "] " << billList[bIndex].items[i].medicineName
                    << " (Qty: " << billList[bIndex].items[i].quantity << ")\n";
            }

            cout << "\n Menu:\n";
            cout << " [1] Remove a Medicine\n";
            cout << " [2] Change Patient ID (Fix wrong patient)\n";
            cout << " [0] Finish Editing\n";
            int editChoice = readInt(" Enter choice: ");

            if ( editChoice == 0 ) {
                cout << " [INFO] Operation cancelled. Returning to menu...\n";
                break;
            }

            if ( editChoice == 1 ) {
                if ( billList[bIndex].items.empty() ) {
                    cout << " [ERROR] Bill is already empty!\n";
                    continue;
                }

                int removeIdx = readInt(" Enter the item number to remove: ");
                if (removeIdx < 1 || removeIdx > (int)billList[bIndex].items.size()) {
                    cout << " [ERROR] Invalid item number.\n";
                    continue;
                }

                //in the menu the medicine index is 1 but it store in the array and the index is 0 so mines 1
                int actualIdx = removeIdx - 1;
                //record the medicineID and the quantity so later can add back the quantity based on the recorded medicineID to the system 
                int mID = billList[bIndex].items[actualIdx].medicineID;
                int returnQty = billList[bIndex].items[actualIdx].quantity;

                //Find the medicine and add the quantity 
                for (int i = 0; i < (int)medicineList.size(); i++) {
                    if (medicineList[i].medicineID == mID) {
                        medicineList[i].stockQty += returnQty;
                        break;
                    }
                }

                cout << " [SUCCESS] Removed " << billList[bIndex].items[actualIdx].medicineName << " and returned to stock.\n";
                billList[bIndex].items.erase(billList[bIndex].items.begin() + actualIdx);

                recalculateBill(bIndex);
                saveBillsToFile();
            }

            else if (editChoice == 2) {
                int newPID = readInt(" Enter correct Patient ID: ");
                int newPIndex = -1;
                for (int i = 0; i < (int)patientList.size(); i++) {
                    if (patientList[i].patientID == newPID) {
                        newPIndex = i;
                        break;
                    }
                }

                //error message 
                if (newPIndex == -1) {
                    cout << " [ERROR] Patient ID " << newPID << " not found in system!\n";
                }
                //change the patient ID , Name and recalculate the bill
                else {
                    billList[bIndex].patientID = patientList[newPIndex].patientID;
                    billList[bIndex].patientName = patientList[newPIndex].patientName;

                    //recalculate because if patient is senior age then it can apply the discount so call the recalculate function)
                    recalculateBill(bIndex);
                    saveBillsToFile();
                    cout << " [SUCCESS] Patient successfully changed to " << billList[bIndex].patientName << ".\n";
                }
            }
            else {
                cout << " [ERROR] Invalid choice.\n";
            }
        }
        cout << " [INFO] Edit completed. Final Amount: RM " << fixed << setprecision(2) << billList[bIndex].finalAmount << "\n";
}

//function 3.3
void deleteBill() {
    cout << "\n +----------------------------------------+\n";
    cout << " |              CANCEL BILL               |\n";
    cout << " +----------------------------------------+\n";

    int bID = readInt(" Enter Bill ID to Cancel (e.g., 1 for B001, or 0 to go back) : ");
    if (bID == 0) {
        cout << " [INFO] Operation cancelled. Returning to menu...\n";
        return;
    }

    int bIndex = -1;
    for (int i = 0; i < (int)billList.size(); i++) {
        if (billList[i].billID == bID) { 
            bIndex = i; 
            break; }
    }

    //error message
    if (bIndex == -1) {
        cout << " [ERROR] Bill Not Found.\n";
        return;
    }

    //only allow to cancel the bill with the unpaid status
    if (billList[bIndex].status != "Unpaid") {
        cout << " [ERROR] Cannot cancel this bill. Current status: " << billList[bIndex].status << "\n";
        return;
    }

    //double confirm for cancel the bill (can not return back to unpaid or paid status)
    cout << " [WARNING] =========================================================\n";
    cout << " [WARNING] WARNING: Once a bill is cancelled, it CANNOT be undone!  \n";
    cout << " [WARNING] The status CANNOT be changed back to 'Paid' or 'Unpaid'. \n";
    cout << " [WARNING] =========================================================\n";
    cout << " Are you sure you want to cancel Bill B" << setfill('0') << setw(3) << bID << "? (Y/N): ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        cout << " Enter reason for cancellation: ";
        string reason;
        getline(cin, reason);
        if (reason.empty()) reason = "No reason provided";

        for (int i = 0; i < (int)billList[bIndex].items.size(); i++) {
            int mID = billList[bIndex].items[i].medicineID;
            int returnQty = billList[bIndex].items[i].quantity;
            for (int j = 0; j < (int)medicineList.size(); j++) {
                if (medicineList[j].medicineID == mID) {
                    medicineList[j].stockQty += returnQty;
                    break;
                }
            }
        }

        //change the status , clear the medicine items store in items and set the finalAmount as 0 because cancalled
        billList[bIndex].status = "Cancelled (" + reason + ")";
        billList[bIndex].finalAmount = 0.0;
        billList[bIndex].items.clear();

        saveBillsToFile();
        cout << " [SUCCESS] Bill successfully cancelled and stock has been returned.\n";
    }
    else {
        cout << " [INFO] Cancellation aborted.\n";
    }
}

//function 3
void billManagementRecords() {
    int choice;
    do {
        cout << "\n +-----------------------------------------+\n";
        cout << " |        BILL MANAGEMENT & RECORDS        |\n";
        cout << " +-----------------------------------------+\n";
        cout << " | [1] View All Bill Records               |\n";
        cout << " | [2] Edit Unpaid Bill                    |\n";
        cout << " | [3] Cancel/Delete Unpaid Bill           |\n";
        cout << " | [0] Back to Bill Menu                   |\n";
        cout << " +-----------------------------------------+\n";

        choice = readInt(" Enter your choice (0-3): ");

        if (choice == 1) {
            viewAllBills();
        }
        else if (choice == 2) {
            editBill();
        }
        else if (choice == 3) {
            deleteBill();
        }
        else if (choice == 0) {
            cout << "\n Returning to Main Bill Menu...\n";
        }
        else {
            cout << "\n [ERROR] Invalid choice!\n";
        }
    } while (choice != 0);
}

void billMenu() {
    loadBillsFromFile();

    int choice;
    do {
        cout << "\n +-----------------------------------------+\n";
        cout << " |           BILL & PAYMENT                |\n";
        cout << " +-----------------------------------------+\n";
        cout << " | [1] Generate Bill                       |\n";
        cout << " | [2] Make Payment                        |\n";
        cout << " | [3] Bill Records                        |\n";
        cout << " | [4] Back to Main Menu                   |\n";
        cout << " +-----------------------------------------+\n";

        choice = readInt(" Enter your choice (1-4): ");
        cout << "\n";

        switch (choice) {
        case 1 : 
            generateBIll(); 
            break;
        case 2: 
            makePayment();
            break;
        case 3 : 
            billManagementRecords();
            break;
        case 4 : 
            cout << "\n Returning to Main Menu...\n";
            break;
        default: cout << "\n [ERROR] Invalid choice!\n";
            break;
        }
    } while (choice != 4);
}