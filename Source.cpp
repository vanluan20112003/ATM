#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <conio.h>
#include <iomanip>
using namespace std;
#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"
struct Account {
    string ID;
    string PIN;
    double balance;
    bool locked;
};

const string adminFile = "Admin.txt";
const string cardFile = "TheTu.txt";
const string historyFolder = "D:\Cau truc du lieu\ATM\TransactionHistory";
const string accountFolder = "D:\Cau truc du lieu\ATM\AccountInfo";

// Function prototypes
bool loginAdmin();
bool loginUser(Account& user);
void adminMenu();
void userMenu(Account& user);
void viewAccounts();
bool addAccount();
bool removeAccount();
bool unlockAccount();
void provideFunds();
bool validateAmount(double amount, double balance);
void performTransaction(Account& sender, Account& receiver, double amount, string transactionType);
string getHiddenInput();
void printColor(const string& text, const string& color);
int main() {
    bool exitProgram = false;

    while (!exitProgram) {
        system("cls");

        printColor("CHUONG TRINH ATM\n", ANSI_BOLD);
        printColor("1. ", ANSI_GREEN);
        cout << ANSI_YELLOW<<"Dang nhap Admin" << endl;
        printColor("2. ", ANSI_GREEN);
        cout << ANSI_YELLOW << "Dang nhap User" << endl;
        printColor("3. ", ANSI_GREEN);
        cout << ANSI_YELLOW << "Thoat" << endl;
        printColor("Chon vai tro dang nhap: ", ANSI_YELLOW);

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            if (loginAdmin()) {
                adminMenu();
            }
            break;
        case 2: {
            Account user;
            if (loginUser(user)) {
                userMenu(user);
            }
            break;
        }
        case 3:
            printColor("Cam on ban da su dung chuong trinh ATM. Hen gap lai!\n", ANSI_CYAN);
            exitProgram = true;
            break;
        default:
            printColor("Lua chon khong hop le. Vui long chon lai.\n", ANSI_RED);
            break;
        }
    }
}

bool loginAdmin() {
    string adminUser;
    bool loggedIn = false;
    int attempts = 0;
    while (attempts < 3 && !loggedIn) {
        cout << ANSI_YELLOW << "* * * * * * * * * * * * * * * * * * * * *\n";
        cout << "       *" << ANSI_RESET << ANSI_GREEN << " DANG NHAP ADMIN " << ANSI_RESET << ANSI_YELLOW << "*\n";
        cout << "* * * * * * * * * * * * * * * * * * * * *\n" << ANSI_RESET;
        cout << ANSI_GREEN<<"Admin user: ";
        cin >> adminUser;
        cout << "Password: "<<ANSI_RESET;
        string adminPass = getHiddenInput();

        ifstream adminFileInput(adminFile);
        if (!adminFileInput.is_open()) {
            cout << "Sai ten dang nhap hoac mat khau!" << endl;
            return false;
        }

        string user, pass;
        while (adminFileInput >> user >> pass) {
            if (user == adminUser && pass == adminPass) {
                system("cls");
                cout << "Dang nhap thanh cong!" << endl;
                adminFileInput.close();
                return true;
            }
        }
        adminFileInput.close();

       
        system("cls");
        cout << "Sai thong tin dang nhap. Vui long kiem tra lai." << endl;
        attempts++;
    }

    if (attempts >= 3) {
        cout << "Ban da nhap sai mat khau qua nhieu lan. Vui long thu lai sau!" << endl;
    }

    return false;
}
string getHiddenInput() {
    string input = "";
    char ch = _getch(); 

    while (ch != 13) {  
        if (ch == 8) {    
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";  
            }
        }
        else {
            input += ch;
            cout << '*';  
        }
        ch = _getch();
    }

    cout << endl;
    return input;
}
bool loginUser(Account& user) {
    string userID, userPIN;
    int attempts = 0;

    while (attempts < 3) {
        cout << ANSI_YELLOW<<"* * * * * * * * * * * * * * * * * * * * *\n";
        cout << "       *"<<ANSI_RESET<<ANSI_GREEN<<" DANG NHAP USER "<<ANSI_RESET<<ANSI_YELLOW<<"*\n";
        cout << "* * * * * * * * * * * * * * * * * * * * *\n"<<ANSI_RESET;
        cout << "ID: ";
        cin >> userID;
        cout << "PIN: ";
        string userPIN = getHiddenInput();

        ifstream cardFileInput(cardFile);
        if (!cardFileInput.is_open()) {
            cout << "Khong the mo file!" << endl;
            return false;
        }

        string ID, PIN;
        while (cardFileInput >> ID >> PIN) {
            if (ID == userID && PIN == userPIN) {
                cout << "Dang nhap thanh cong!" << endl;
                user.ID = userID;
                user.PIN = userPIN;
                user.locked = false;

                string accountFile = user.ID + ".txt";
                ifstream accountFileInput(accountFile);
                if (!accountFileInput.is_open()) {
                    cout << "Khong the mo file " << accountFile << endl;
                    return false;
                }

                string line;
                while (getline(accountFileInput, line)) {
                    if (line.find("So du: ") == 0) {
                        string balanceStr = line.substr(7);
                        user.balance = stod(balanceStr);
                        break;
                    }
                }

                accountFileInput.close();
                cardFileInput.close();
                return true;
            }
        }

        cardFileInput.close();
        system("cls");
        cout << "Sai thong tin dang nhap. Vui long kiem tra lai." << endl;
        attempts++;
    }

    cout << "Sai thong tin dang nhap qua nhieu lan. Tai khoan cua ban da bi khoa." << endl;
    user.locked = true;
    return false;
}

void adminMenu() {
    while (true) {
        cout << ANSI_YELLOW << "***** MENU ADMIN *****"  << endl;
        cout << "1. Xem danh sach tai khoan" << endl;
        cout << "2. Them tai khoan" << endl;
        cout << "3. Xoa tai khoan" << endl;
        cout << "4. Mo khoa tai khoan" << endl;
        cout << "5. Cap tien cho tai khoan" << endl;
        cout << "6. Thoat" << ANSI_RESET<< endl;
        char choice;
        cin >> choice;

        switch (choice) {
        case '1':
            system("cls");
            viewAccounts();     
            break;
        case '2':
            system("cls");
            addAccount();
            break;
        case '3':
            system("cls");
            removeAccount();
            break;
        case '4':
            system("cls");
            unlockAccount();
            break;
        case '5':
            system("cls");
            provideFunds();
            break;
        case '6':
            return;
        }
    }
}

void userMenu(Account& user) {
    while (true) {
        cout <<ANSI_YELLOW<< "* * * * * * * * MENU * * * * * * * * * *\n";
        cout << "1. Xem thong tin tai khoan\n";
        cout << "2. Rut tien\n";
        cout << "3. Chuyen tien\n";
        cout << "4. Xem noi dung giao dich\n";
        cout << "5. Doi ma Pin\n";
        cout << "6. Thoat\n";
        cout << "* * * * * * * * * * *** * * * * * * * * * *\n"<<ANSI_RESET;
        char choice;
        cin >> choice;

        switch (choice) {
        case '1':
            system("cls");
            cout << "ID: " << user.ID << endl;
            cout << "So du: " << fixed << setprecision(2) << user.balance << " VND" << endl;
            break;
        case '2': {
            system("cls");
            double amount;
            cout << "Nhap so tien can rut: ";
            cin >> amount;

            if (validateAmount(amount, user.balance)) {
                user.balance -= amount;

                string accountFile = user.ID + ".txt";
                ofstream accountFileOutput(accountFile);
                if (!accountFileOutput.is_open()) {
                    cout << "Khong the mo file " << accountFile << endl;
                    return;
                }

                accountFileOutput << "ID: " << user.ID << endl;
                accountFileOutput << "Ten tai khoan: " << endl;
                accountFileOutput << "So du: " << user.balance << " VND" << endl;
                accountFileOutput << "Loai tien te: VND" << endl;
                accountFileOutput.close();

                string historyFile = historyFolder + user.ID + ".txt";
                ofstream historyFileOutput(historyFile, ios_base::app);
                time_t now = time(0);
                tm localTime;
                localtime_s(&localTime, &now);
                string transactionTime = to_string(localTime.tm_hour) + ":" + to_string(localTime.tm_min) + " " + to_string(localTime.tm_mday) + "/" + to_string(localTime.tm_mon + 1) + "/" + to_string(localTime.tm_year + 1900);
                historyFileOutput << "Rut tien" << "," << amount << "," << transactionTime << endl;
                historyFileOutput.close();

                cout << "Giao dich rut tien thanh cong." << endl;
            }
            else {
                cout << "So tien rut khong hop le. Vui long kiem tra lai." << endl;
            }
            break;
        }
        case '3': {
            system("cls");
            string receiverID;
            double amount;
            cout << "Nhap ID tai khoan muon chuyen tien den: ";
            cin >> receiverID;
            cout << "Nhap so tien can chuyen: ";
            cin >> amount;

            ifstream cardFileInput(cardFile);
            if (!cardFileInput.is_open()) {
                cout << "Khong the mo file TheTu.txt" << endl;
                return;
            }

            string ID, PIN;
            bool receiverFound = false;
            while (cardFileInput >> ID >> PIN) {
                if (ID == receiverID) {
                    receiverFound = true;
                    break;
                }
            }
            cardFileInput.close();

            if (!receiverFound) {
                cout << "Khong tim thay tai khoan co ID la " << receiverID << ". Khong the thuc hien giao dich." << endl;
            }
            else if (!validateAmount(amount, user.balance)) {
                cout << "So tien chuyen khong hop le. Vui long kiem tra lai." << endl;
            }
            else {
                // Cập nhật số dư người gửi và người nhận
                user.balance -= amount;
                string senderAccountFile = user.ID + ".txt";
                ofstream senderAccountFileOutput(senderAccountFile);
                if (!senderAccountFileOutput.is_open()) {
                    cout << "Khong the mo file " << senderAccountFile << endl;
                    return;
                }
                senderAccountFileOutput << "ID: " << user.ID << endl;
                senderAccountFileOutput << "PIN: " << user.PIN << endl;
                senderAccountFileOutput << "Ten tai khoan: " << endl;
                senderAccountFileOutput << "So du: " << fixed << setprecision(2) << user.balance << " VND" << endl;
                senderAccountFileOutput << "Loai tien te: VND" << endl;
                senderAccountFileOutput.close();

                string receiverAccountFile = receiverID + ".txt";
                ifstream receiverAccountFileInput(receiverAccountFile);
                if (!receiverAccountFileInput.is_open()) {
                    cout << "Khong the mo file " << receiverAccountFile << endl;
                    return;
                }
                double receiverBalance = 0;
                string line;
                while (getline(receiverAccountFileInput, line)) {
                    if (line.find("So du: ") == 0) {
                        string balanceStr = line.substr(7);
                        receiverBalance = stod(balanceStr);
                        break;
                    }
                }
                receiverAccountFileInput.close();
                receiverBalance += amount;
                ofstream receiverAccountFileOutput(receiverAccountFile);
                if (!receiverAccountFileOutput.is_open()) {
                    cout << "Khong the mo file " << receiverAccountFile << endl;
                    return;
                }
                receiverAccountFileOutput << "ID: " << receiverID << endl;
                receiverAccountFileOutput << "PIN: " << PIN << endl;
                receiverAccountFileOutput << "Ten tai khoan: " << endl;
                receiverAccountFileOutput << "So du: " << fixed << setprecision(2) << receiverBalance << " VND" << endl;
                receiverAccountFileOutput << "Loai tien te: VND" << endl;
                receiverAccountFileOutput.close();

                
                string senderHistoryFile = historyFolder + user.ID + ".txt";
                ofstream senderHistoryFileOutput(senderHistoryFile, ios_base::app);
                time_t now = time(0);
                tm localTime;
                localtime_s(&localTime, &now);
                string transactionTime = to_string(localTime.tm_hour) + ":" + to_string(localTime.tm_min) + " " + to_string(localTime.tm_mday) + "/" + to_string(localTime.tm_mon + 1) + "/" + to_string(localTime.tm_year + 1900);
                senderHistoryFileOutput << "Chuyen tien" << "," << receiverID << "," << amount << "," << transactionTime << endl;
                senderHistoryFileOutput.close();

                string receiverHistoryFile = historyFolder + receiverID + ".txt";
                ofstream receiverHistoryFileOutput(receiverHistoryFile, ios_base::app);
                receiverHistoryFileOutput << "Nhan tien" << "," << user.ID << "," << amount << "," << transactionTime << endl;
                receiverHistoryFileOutput.close();

                cout << "Giao dich chuyen tien thanh cong." << endl;
            }
            break;
        }
        case '4': {
            system("cls");
            string historyFile = historyFolder + user.ID + ".txt";
            ifstream historyFileInput(historyFile);
            if (!historyFileInput.is_open()) {
                cout << "Khong the mo file " << historyFile << endl;
                return;
            }

            cout << "Noi dung giao dich:\n";
            cout << "Loai giao dich, Thong tin, So tien, Thoi gian\n";
            string line;
            while (getline(historyFileInput, line)) {
                cout << line << endl;
            }
            historyFileInput.close();
            break;
        }
        case '5': {
            system("cls");
            string oldPIN, newPIN1, newPIN2;
            cout << "Nhap ma PIN hien tai: ";
            cin >> oldPIN;
            if (oldPIN != user.PIN) {
                cout << "Ma PIN hien tai khong chinh xac." << endl;
                break;
            }

            cout << "Nhap ma PIN moi: ";
            cin >> newPIN1;
            cout << "Nhap lai ma PIN moi lan 2: ";
            cin >> newPIN2;

            if (newPIN1 != newPIN2) {
                cout << "Hai lan nhap ma PIN moi khong trung khop." << endl;
                break;
            }

            if (newPIN1 == user.PIN) {
                cout << "Ma PIN moi phai khac ma PIN hien tai." << endl;
                break;
            }

            user.PIN = newPIN1;

            ofstream cardFileOutput(cardFile);
            if (!cardFileOutput.is_open()) {
                cout << "Khong the mo file TheTu.txt" << endl;
                return;
            }

            cardFileOutput << user.ID << endl << user.PIN << endl;
            cardFileOutput.close();

            cout << "Doi ma PIN thanh cong." << endl;
            break;
        }
        case '6':
            return;
        }
    }
}

void viewAccounts() {
    ifstream cardFileInput(cardFile);
    if (!cardFileInput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return;
    }

    cout << "Danh sach tai khoan:\n";
    cout << "ID va PIN\n";
    string line;
    while (getline(cardFileInput, line)) {
        cout << line << endl;
    }
    cardFileInput.close();
}
void provideFunds() {
    string receiverID;
    double amount;
    cout << "Nhap ID tai khoan muon cap tien: ";
    cin.ignore();
    getline(cin, receiverID);

    ifstream cardFileInput(cardFile);
    if (!cardFileInput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return;
    }

    string ID, PIN;
    bool receiverFound = false;
    while (cardFileInput >> ID >> PIN) {
        if (ID == receiverID) {
            receiverFound = true;
            break;
        }
    }
    cardFileInput.close();

    if (!receiverFound) {
        cout << "Khong tim thay tai khoan co ID la " << receiverID << ". Khong the thuc hien giao dich." << endl;
    }
    else {
        cout << "Nhap so tien can cap: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "So tien cap khong hop le. Vui long kiem tra lai." << endl;
            return;
        }

        string accountFile = receiverID + ".txt";
        ifstream accountFileInput(accountFile);
        if (!accountFileInput.is_open()) {
            cout << "Khong the mo file " << accountFile << endl;
            return;
        }

        string line;
        double balance = 0;
        while (getline(accountFileInput, line)) {
            if (line.find("So du: ") == 0) {
                string balanceStr = line.substr(7);
                balance = stod(balanceStr);
                break;
            }
        }
        accountFileInput.close();

        balance += amount;

        ofstream accountFileOutput(accountFile);
        if (!accountFileOutput.is_open()) {
            cout << "Khong the mo file " << accountFile << endl;
            return;
        }

        accountFileOutput << "ID: " << receiverID << endl;
        accountFileOutput << "Ten tai khoan: " << endl;
        accountFileOutput << "So du: " << balance << " VND" << endl;
        accountFileOutput << "Loai tien te: VND" << endl;
        accountFileOutput.close();

        string historyFile = historyFolder + receiverID + ".txt";
        ofstream historyFileOutput(historyFile, ios_base::app);
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);
        string transactionTime = to_string(localTime.tm_hour) + ":" + to_string(localTime.tm_min) + " " + to_string(localTime.tm_mday) + "/" + to_string(localTime.tm_mon + 1) + "/" + to_string(localTime.tm_year + 1900);
        historyFileOutput << "Cap tien" << "," << amount << "," << transactionTime << endl;
        historyFileOutput.close();

        cout << "Cap tien thanh cong." << endl;
    }
}


bool addAccount() {
    string newID, newPIN;
    cout << "Nhap ID tai khoan moi: ";
    cin.ignore();
    getline(cin, newID);

    ifstream cardFileInput(cardFile);
    if (!cardFileInput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return false;
    }

    string ID, PIN;
    while (cardFileInput >> ID >> PIN) {
        if (ID == newID) {
            cout << "ID tai khoan da ton tai. Khong the them tai khoan moi." << endl;
            cardFileInput.close();
            return false;
        }
    }

    cardFileInput.close();

    cout << "Nhap ma PIN moi cho tai khoan " << newID << ": ";
    cin >> newPIN;

    ofstream cardFileOutput(cardFile, ios_base::app);
    if (!cardFileOutput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return false;
    }

    cardFileOutput << newID << endl;
    cardFileOutput << newPIN << endl;
    cardFileOutput.close();

    string accountFile = newID + ".txt";
    ofstream accountFileOutput(accountFile);
    if (!accountFileOutput.is_open()) {
        cout << "Khong the mo file " << accountFile << endl;
        return false;
    }

    accountFileOutput << "ID: " << newID << endl;
    accountFileOutput << "PIN: " << newPIN << endl;
    accountFileOutput << "Ten tai khoan: " << endl;
    accountFileOutput << "So du: 0 VND" << endl;
    accountFileOutput << "Loai tien te: VND" << endl;
    accountFileOutput.close();

    string historyFile = historyFolder + newID + ".txt";
    ofstream historyFileOutput(historyFile);
    if (!historyFileOutput.is_open()) {
        cout << "Khong the mo file " << historyFile << endl;
        return false;
    }

    historyFileOutput << "Loai giao dich, Thong tin, So tien, Thoi gian" << endl;
    historyFileOutput.close();

    cout << "Them tai khoan moi thanh cong." << endl;
    return true;
}


bool removeAccount() {
    string IDToDelete;
    cout << "Nhap ID tai khoan muon xoa: ";
    cin.ignore();
    getline(cin, IDToDelete);

    ifstream cardFileInput(cardFile);
    if (!cardFileInput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return false;
    }

    string ID, PIN;
    vector<Account> accounts;
    while (cardFileInput >> ID >> PIN) {
        if (ID != IDToDelete) {
            Account account;
            account.ID = ID;
            account.PIN = PIN;
            accounts.push_back(account);
        }
    }

    cardFileInput.close();

    ofstream cardFileOutput(cardFile);
    if (!cardFileOutput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return false;
    }

    for (const Account& account : accounts) {
        cardFileOutput << account.ID << "," << account.PIN << endl;
    }

    cardFileOutput.close();

    string accountFile = IDToDelete + ".txt";
    if (remove(accountFile.c_str()) != 0) {
        cout << "Khong the xoa file " << accountFile << endl;
        return false;
    }

    string historyFile = historyFolder + IDToDelete + ".txt";
    if (remove(historyFile.c_str()) != 0) {
        cout << "Khong the xoa file " << historyFile << endl;
        return false;
    }

    cout << "Xoa tai khoan thanh cong." << endl;
    return true;
}

bool unlockAccount() {
    vector<Account> accounts;

    ifstream cardFileInput(cardFile);
    if (!cardFileInput.is_open()) {
        cout << "Khong the mo file TheTu.txt" << endl;
        return false;
    }

    string ID, PIN;
    while (cardFileInput >> ID >> PIN) {
        Account account;
        account.ID = ID;
        account.PIN = PIN;
        accounts.push_back(account);
    }

    cardFileInput.close();

    string IDToUnlock;
    cout << "Nhap ID tai khoan muon mo khoa: ";
    cin.ignore();
    getline(cin, IDToUnlock);

    bool found = false;
    for (Account& account : accounts) {
        if (account.ID == IDToUnlock) {
            if (account.locked) {
                account.locked = false;

                ofstream cardFileOutput(cardFile);
                if (!cardFileOutput.is_open()) {
                    cout << "Khong the mo file TheTu.txt" << endl;
                    return false;
                }

                for (const Account& acc : accounts) {
                    cardFileOutput << acc.ID << "," << acc.PIN << endl;
                }

                cardFileOutput.close();

                cout << "Mo khoa tai khoan thanh cong." << endl;
            }
            else {
                cout << "Tai khoan da o trang thai mo khoa." << endl;
            }

            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Khong tim thay tai khoan co ID la " << IDToUnlock << ". Khong the mo khoa tai khoan." << endl;
        return false;
    }

    return true;
}

bool validateAmount(double amount, double balance) {
    return (amount >= 50000 && amount <= balance - 50000 && int(amount) % 50000 == 0);
}
void printColor(const string& text, const string& color) {
    cout << color << text << ANSI_RESET;
}