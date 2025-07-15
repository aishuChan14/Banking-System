
#include <iostream>
#include <fstream>
using namespace std;

class Account {
    int accNo;
    char name[50];
    float balance;

public:
    void createAccount();
    void showAccount() const;
    void deposit(float);
    void withdraw(float);
    int getAccNo() const;
    float getBalance() const;
};

void Account::createAccount() {
    cout << "Enter Account No: ";
    cin >> accNo;
    cin.ignore();
    cout << "Enter Account Holder Name: ";
    cin.getline(name, 50);
    cout << "Enter Initial Balance: ";
    cin >> balance;
}

void Account::showAccount() const {
    cout << "\nAccount No: " << accNo;
    cout << "\nName: " << name;
    cout << "\nBalance: " << balance << endl;
}

void Account::deposit(float amount) {
    balance += amount;
}

void Account::withdraw(float amount) {
    if (amount <= balance)
        balance -= amount;
    else
        cout << "Insufficient balance!\n";
}

int Account::getAccNo() const {
    return accNo;
}

float Account::getBalance() const {
    return balance;
}

void writeAccount() {
    Account acc;
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
    cout << "\nAccount created successfully!\n";
}

void displayAccount(int num) {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == num) {
            acc.showAccount();
            found = true;
        }
    }
    inFile.close();

    if (!found)
        cout << "\nAccount not found!\n";
}

void updateAccount(int num, bool isDeposit) {
    Account acc;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && !found) {
        int pos = file.tellg();
        file.read(reinterpret_cast<char*>(&acc), sizeof(Account));

        if (acc.getAccNo() == num) {
            acc.showAccount();
            float amount;
            cout << (isDeposit ? "\nEnter amount to deposit: " : "\nEnter amount to withdraw: ");
            cin >> amount;

            if (isDeposit)
                acc.deposit(amount);
            else
                acc.withdraw(amount);

            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            found = true;
        }
    }
    file.close();

    if (!found)
        cout << "\nAccount not found!\n";
}

void deleteAccount(int num) {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() != num)
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    }

    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    cout << "\nAccount deleted successfully!\n";
}

int main() {
    int choice, accNum;

    do {
        cout << "\n\n\U0001F3E6 BANKING SYSTEM MENU";
        cout << "\n1. Create Account";
        cout << "\n2. View Account";
        cout << "\n3. Deposit";
        cout << "\n4. Withdraw";
        cout << "\n5. Delete Account";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeAccount();
                break;
            case 2:
                cout << "Enter Account No: ";
                cin >> accNum;
                displayAccount(accNum);
                break;
            case 3:
                cout << "Enter Account No: ";
                cin >> accNum;
                updateAccount(accNum, true);
                break;
            case 4:
                cout << "Enter Account No: ";
                cin >> accNum;
                updateAccount(accNum, false);
                break;
            case 5:
                cout << "Enter Account No: ";
                cin >> accNum;
                deleteAccount(accNum);
                break;
            case 6:
                cout << "\nThank you for using the Banking System.\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
