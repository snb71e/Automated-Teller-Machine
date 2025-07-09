#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sstream>
#include <exception>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

class Account;
class Bank;
class ATM;
class Interface;
class card;
class singleATM;
class multiATM;
class record;

int num_of_ATM = 0;
int num_of_banks = 0;

std::vector<Bank*> bank_list;
std::vector<ATM*> atm_list;
void display_history(bool exit, const vector<record*>& transaction_records, const Interface& ui);
int getIntegerInput(const string& prompt, Interface* ui);
string globalinput_string(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui);
double globalinput_double(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui);
int globalinput_int(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui);

// ================================== ACCOUNT ================================== \\

class Account {
private:
    string account_number;
    string password;
    string owner_name;
    string card_number;
    long long available_fund = 0;
    Bank* account_bank;
    Bank* accountNumber;

public:
    Account(string account_num, string owner, string card_num, double init_balance, Bank* bank, string pwd) : account_number(account_num), owner_name(owner), card_number(card_num), available_fund(init_balance), account_bank(bank), password(pwd) {
        account_number = account_num;
        owner_name = owner;
        card_num = card_number;
        available_fund = init_balance;
        account_bank = bank;
        password = pwd;
    }
    ~Account() {
    }
    double increase_account_balance(double amount) { return available_fund += amount; }
    double decrease_account_balance(double amount) { return available_fund -= amount; }
    string getAccountNumber() { return account_number; }
    string getOwnerName() { return owner_name; };
    double getAvailableFund() { return available_fund; }
    string getCardNumber() { return card_number; }
    string getPassword() { return password; }
    string getBankName();
};

// ================================== BANK ================================== \\

class Bank {
private:
    string bank_name;
    string bank_number;
    string history[100];
    int num_of_transactions = 0;
    Account* accounts[100];
    int num_of_accounts = 0;
    Interface* ui;

public:
    Bank(string bankname, string banknumber, Interface* interface)
        : bank_name(bankname), bank_number(banknumber), ui(interface) {
        num_of_banks += 1;
        for (int i = 0; i < 100; ++i) {
            accounts[i] = nullptr;
        }
    }
    ~Bank() {
        for (int i = 0; i < num_of_accounts; ++i) {
            delete accounts[i];
        }
    }
    bool isPositive(int input);
    string getBankName() const { return bank_name; }
    string getBankNumber() const { return bank_number; }
    Account* createAccount(const string& owner, double balance, const string& password);
    bool deleteAccount(const string& account_number);
    void listAccounts() const;
    Account* make_account(string new_owner_name, string bankname, long long int initial_balance, string password);
    Account* getAccount(int index) const;
    int getNumOfAccounts() const;
    Account* access_to_account(int i);
    Account* access_to_account_by_num(string num);
    int calculateFee(string transaction_type, string cardBank, string target_bank);
    bool hasAccount(string accountnumber);
    void increase_receiver_balance(string receiver_account_number, double amount);
    bool show_authorize(string account_num, string input_password);
};

// ================================== INTERFACE ================================== \\

class Interface {
private:
    bool isEng;
    void printSeparator(int width = 70) const {
        std::cout << std::string(width, '=') << std::endl;
    }
    void printCentered(const std::string& text, int width = 70) const {
        int padding = (width - static_cast<int>(text.length())) / 2;
        std::cout << std::string(padding, ' ') << text << std::endl;

    }
    std::string createBox(const std::string& text, int boxWidth = 30, int boxHeight = 5) const {
        std::ostringstream boxStream;
        boxStream << "=" << std::string(boxWidth - 2, '=') << "=" << std::endl;
        int emptyLines = (boxHeight - 1) / 2;
        for (int i = 0; i < emptyLines; ++i) {
            boxStream << "=" << std::string(boxWidth - 2, ' ') << "=" << std::endl;
        }
        int textPadding = (boxWidth - 2 - static_cast<int>(text.length())) / 2;
        boxStream << "=" << std::string(textPadding, ' ') << text
            << std::string(boxWidth - 2 - textPadding - text.length(), ' ') << "=" << std::endl;
        for (int i = 0; i < emptyLines; ++i) {
            boxStream << "=" << std::string(boxWidth - 2, ' ') << "=" << std::endl;
        }
        boxStream << "=" << std::string(boxWidth - 2, '=') << "=" << std::endl;
        return boxStream.str();
    }

    void printTwoBoxes(const std::string& leftText, const std::string& rightText, int boxWidth = 30, int totalWidth = 70) const {
        std::string leftBox = createBox(leftText, boxWidth);
        std::string rightBox = createBox(rightText, boxWidth);

        std::istringstream leftStream(leftBox), rightStream(rightBox);
        std::string leftLine, rightLine;

        while (std::getline(leftStream, leftLine) && std::getline(rightStream, rightLine)) {
            int padding = (totalWidth - (boxWidth * 2)) / 3;
            std::cout << std::string(padding, ' ') << leftLine
                << std::string(padding, ' ') << rightLine << std::endl;
        }
    }
    void printFourBoxes(const std::vector<std::string>& options, int boxWidth = 30, int totalWidth = 70) const {
        std::vector<std::string> filledOptions = options;
        while (filledOptions.size() < 4) {
            filledOptions.push_back("");
        }
        std::string leftBox = createBox(filledOptions[0], boxWidth);
        std::string rightBox = createBox(filledOptions[1], boxWidth);
        std::istringstream leftStream(leftBox), rightStream(rightBox);
        std::string leftLine, rightLine;
        while (std::getline(leftStream, leftLine) && std::getline(rightStream, rightLine)) {
            int padding = (totalWidth - (boxWidth * 2)) / 3;
            std::cout << std::string(padding, ' ') << leftLine
                << std::string(padding, ' ') << rightLine << std::endl;
        }
        leftBox = createBox(filledOptions[2], boxWidth);
        rightBox = createBox(filledOptions[3], boxWidth);
        leftStream = std::istringstream(leftBox);
        rightStream = std::istringstream(rightBox);
        while (std::getline(leftStream, leftLine) && std::getline(rightStream, rightLine)) {
            int padding = (totalWidth - (boxWidth * 2)) / 3;
            std::cout << std::string(padding, ' ') << leftLine
                << std::string(padding, ' ') << rightLine << std::endl;
        }
    }

    void printMenu(const std::string& title, const std::vector<std::string>& options) const {
        clearScreen();
        printSeparator();
        printCentered(title, 70);
        printSeparator();
        printFourBoxes(options, 30, 70);
        printSeparator();
    }

public:
    Interface(bool lang = true) : isEng(lang) {}
    void clearScreen() const {

#ifdef _WIN32
        system("cls");
#else
        std::cout << "\033[H\033[J";
#endif
    }
    void showTransitionMessage(const std::string& message) const {
        clearScreen();
        printSeparator();
        printCentered(message, 70);
        printSeparator();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void setLanguage(bool lang) {
        isEng = lang;
    }

    bool getLanguage() const {
        return isEng;
    }

    void showStartScreen() const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "Welcome to the System" : "�ý��ۿ� ���� ���� ȯ���մϴ�", 70);
        printSeparator();
        printTwoBoxes(isEng ? "1. Bank Management" : "1. ���� ����", isEng ? "2. ATM Management" : "2. ATM ����");
        printTwoBoxes(isEng ? "3. Language Settings" : "3. ��� ����", isEng ? "4. Insert Card" : "4. ī�� ����");
        printTwoBoxes(isEng ? "5. Transaction History" : "5. �ŷ� ����", isEng ? "6. Exit" : "6. ����");
        printSeparator();
    }
    void showBankManagementMenu() const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "Bank Management" : "���� ����", 70);
        printSeparator();
        printTwoBoxes(isEng ? "1. Add Bank" : "1. ���� �߰�", isEng ? "2. Remove Bank" : "2. ���� ����");
        printTwoBoxes(isEng ? "3. Create Account" : "3. ���� ����", isEng ? "4. Delete Account" : "4. ���� ����");
        printTwoBoxes(isEng ? "5. View Accounts" : "5. ���� ���", isEng ? "6. Return to Main Menu" : "6. ���� �޴�");
        printSeparator();
    }

    void showAMTManagementMenu() const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "ATM Management" : "ATM ����", 70);
        printSeparator();
        printTwoBoxes(isEng ? "1. Add ATM" : "1. ATM �߰�", isEng ? "2. Remove ATM" : "2. ATM ����");
        printTwoBoxes(isEng ? "3. View ATM" : "3. ATM ���", isEng ? "4. Return to Main Menu" : "4. ���� �޴�");
        printSeparator();
    }

    void showLanguageSettingsMenu() const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "Language Settings" : "��� ����", 70);
        printSeparator();
        printTwoBoxes(isEng ? "1. English" : "1. ����", isEng ? "2. Korean" : "2. �ѱ���");
        printSeparator();
        cout << (isEng ? "Please select your preferred language: " : "���Ͻô� �� ������ �ּ���: ");
    }

    void showDepositMenu() const {
        printMenu(isEng ? "Deposit Menu" : "�Ա� �޴�", { isEng ? "1. Cash Deposit" : "1. ���� �Ա�" ,isEng ? "2. Check Deposit" : "2. ��ǥ �Ա�",isEng ? "3. Exit" : "3. �ŷ� ���" });
    }

    void showTransferCashUI(int transferAmount) const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "Insert cash." : "������ �����ϼ���.", 70);
        printSeparator();
        printCentered((isEng ? "Transfer amount: " : "�۱� �ݾ�: ") + std::to_string(transferAmount) + (isEng ? " won." : "��"), 70);
        printSeparator();
    }

    void showTransferSuccessUI(int accountBalance) const {
        printSeparator();
        printCentered(isEng ? "Transfer Successful!" : "�۱� ����!", 70);
        printSeparator();
        printCentered((isEng ? "Current Balance: " : "���� ���� �ܾ�: ") + std::to_string(accountBalance) + (isEng ? " won." : "��"), 70);
        printSeparator();
    }

    void showErrorUI(const std::string& message) const {
        clearScreen();
        printSeparator();
        printCentered(isEng ? "Error" : "����", 70);
        printSeparator();
        printCentered(message, 70);
        printSeparator();
    }

    void showWithdrawOptions() const {
        clearScreen();
        cout << "======================================================================" << endl;
        cout << "                              " << (isEng ? "Withdraw" : "���") << endl;
        cout << "======================================================================" << endl;
    }

    void showTransferOptions() const {
        printMenu(isEng ? "Transfer Menu" : "�۱� �޴�", { isEng ? "1. Cash Transfer" : "1. ���� �۱�", isEng ? "2. Account Transfer" : "2. ���� �۱�", isEng ? "3. Cancel" : "3. ���" });
    }

    void showErrorMessage() const {
        printMenu(isEng ? "Error" : "����", { isEng ? "Invalid input." : "�߸��� �Է��Դϴ�.", isEng ? "Try again." : "�ٽ� �õ����ּ���." });
    }

    void transactionCancelled() const {
        printMenu(isEng ? "Cancelled" : "��ҵ�", { isEng ? "Transaction" : "�ŷ���", isEng ? "cancelled." : "��ҵǾ����ϴ�" });
    }

    void showDepositAmount(int amount) const {
        printMenu(isEng ? "Depost completed" : "�Ա� �Ϸ�", { (isEng ? "Depost amount" : "�� �Ա� �ݾ�: "), std::to_string(amount) + (isEng ? "won" : "��") });
    }

    void displayBalance(int balance) const {
        printMenu(isEng ? "Balance Inquiry" : "�ܾ� ��ȸ", { (isEng ? "Current Balance: " : "���� �ܾ�: ") + std::to_string(balance) + (isEng ? " won." : "��") });
    }

    void promptForCheckDeposit() const {
        printMenu(isEng ? "Check Deposit" : "��ǥ �Ա�", { isEng ? "Insert check." : "��ǥ ����" ,isEng ? "0 to Exit" : "��� : 0" });
    }

    void requestFeePayment(int fee) const {
        printMenu(isEng ? "Fee Process" : "������ �ȳ�", { std::to_string(fee) + (isEng ? "won" : "��"), isEng ? "Pay to proceed." : "�����Ϸ��� ����" });
    }

    void showUserMenu() const {
        printMenu(isEng ? "User Menu" : "����� �޴�", { isEng ? "1. Deposit" : "1. �Ա�", isEng ? "2. Withdraw" : "2. ���", isEng ? "3. Transfer" : "3. �۱�", isEng ? "4. Exit" : "4. ����" });
    }

    void showAdminMenu() const {
        printMenu(isEng ? "Admin Menu" : "������ �޴�", { isEng ? "1. View Transaction History" : "1. �ŷ� ���� ����", isEng ? "2. Exit" : "2. ����" });
    }
};

// ================================== RECORD ================================== \\

class record {
private:
    string transactionID;
    string cardNumber;
    string transactionType;
    int amount;
    string atm_bank;
    string deposit_type;
    string SN;
    string owner;
    string owner_bank;
    string owner_account;
    string receiver_bank;
    string receiver_account;
    string transfer_type;

public:
    record(const string& parameter1, const string& parameter2,
        const string& parameter3, const int& parameter4,
        const string& parameter5, const string& parameter6,
        const string& parameter7, const string& parameter8,
        const string& parameter9, const string& parameter10 = "") {
        transactionID = parameter1;
        cardNumber = parameter2;
        transactionType = parameter3;
        amount = parameter4;
        if (parameter3 == "Deposit") {
            deposit_type = parameter5;
            atm_bank = parameter6;
            SN = parameter7;
            owner = parameter8;
            owner_bank = parameter9;
            owner_account = parameter10;
        }
        else if (parameter3 == "Withdraw") {
            atm_bank = parameter5;
            SN = parameter6;
            owner = parameter7;
            owner_bank = parameter8;
            owner_account = parameter9;
        }
        else if (parameter3 == "Cash Transfer") {
            atm_bank = parameter5;
            SN = parameter6;
            receiver_bank = parameter7;
            receiver_account = parameter8;
            transfer_type = parameter9;
        }
        else if (parameter3 == "Account Transfer") {
            atm_bank = parameter5;
            SN = parameter6;
            owner = parameter7;
            owner_bank = parameter8;
            owner_account = parameter9;
            transfer_type = parameter10;
        }
        else {
            throw invalid_argument("Invalid transaction type");
        }
    }
    string getTransactionID() { return transactionID; }
    string getCardNumber() { return cardNumber; }
    string getTransactionType() { return transactionType; }
    int getAmount() { return amount; }
    string getAtmBank() { return atm_bank; }
    string getOwner() { return owner; }
    string write_one_transaction(const Interface* ui, const std::string& transaction_type) const {
        stringstream output;
        output << (ui->getLanguage() ? "Transaction ID: " : "�ŷ� ID: ") << transactionID << "\n";
        output << (ui->getLanguage() ? "Card Number: " : "ī�� ��ȣ: ") << cardNumber << "\n";
        output << (ui->getLanguage() ? "Transaction Type: " : "�ŷ� ����: ") << transactionType << "\n";
        output << (ui->getLanguage() ? "Amount: " : "�ݾ�: ") << amount << "\n";
        if (transaction_type == "Deposit") {
            output << (ui->getLanguage() ? "Deposit Type: " : "�Ա� ����: ") << deposit_type << "\n";
            output << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << "\n";
            output << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << "\n";
            output << (ui->getLanguage() ? "Owner Name: " : "�����ָ�: ") << owner << "\n";
            output << (ui->getLanguage() ? "Owner Bank: " : "���� ����: ") << owner_bank << "\n";
            output << (ui->getLanguage() ? "Owner Account: " : "���� ��ȣ: ") << owner_account << "\n";
        }
        else if (transaction_type == "Withdraw") {
            output << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << "\n";
            output << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << "\n";
            output << (ui->getLanguage() ? "Owner Name: " : "�����ָ�: ") << owner << "\n";
            output << (ui->getLanguage() ? "Owner Bank: " : "���� ����: ") << owner_bank << "\n";
            output << (ui->getLanguage() ? "Owner Account: " : "���� ��ȣ: ") << owner_account << "\n";
        }
        else if (transaction_type == "Cash Transfer") {
            output << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << "\n";
            output << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << "\n";
            output << (ui->getLanguage() ? "Receiver Bank: " : "�޴� ���� ����: ") << receiver_bank << "\n";
            output << (ui->getLanguage() ? "Receiver Account: " : "�޴� ���� ��ȣ: ") << receiver_account << "\n";
            output << (ui->getLanguage() ? "Transfer Type: " : "��ü ����: ") << transfer_type << "\n";
        }
        else if (transaction_type == "Account Transfer") {
            output << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << "\n";
            output << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << "\n";
            output << (ui->getLanguage() ? "Owner Name: " : "������ �����ָ�: ") << owner << "\n";
            output << (ui->getLanguage() ? "Owner Bank: " : "������ ���� ����: ") << owner_bank << "\n";
            output << (ui->getLanguage() ? "Owner Account: " : "������ ���� ��ȣ: ") << owner_account << "\n";
            output << (ui->getLanguage() ? "Transfer Type: " : "��ü ����: ") << transfer_type << "\n";
        }
        else {
            output << (ui->getLanguage() ? "Unknown Transaction Type" : "�� �� ���� �ŷ� ����") << "\n";
        }
        output << "-------------------------------------------\n";
        return output.str();
    }
    // ���� 2455 ���� 2300 �Կ� 2500 ���� 2000
    void display_one_transaction(const Interface* ui, string transaction_type) const {
        cout << (ui->getLanguage() ? "Transaction ID: " : "�ŷ� ID: ") << transactionID << endl;
        cout << (ui->getLanguage() ? "Card Number: " : "ī�� ��ȣ: ") << cardNumber << endl;
        cout << (ui->getLanguage() ? "Transaction Type: " : "�ŷ� ����: ") << transactionType << endl;
        cout << (ui->getLanguage() ? "Amount: " : "�ݾ�: ") << amount << endl;
        if (transaction_type == "Deposit") {
            cout << (ui->getLanguage() ? "Deposit Type: " : "�Ա� ����: ") << deposit_type << endl;
            cout << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << endl;
            cout << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << endl;
            cout << (ui->getLanguage() ? "Owner Name: " : "�����ָ�: ") << owner << endl;
            cout << (ui->getLanguage() ? "Owner Bank: " : "���� ����: ") << owner_bank << endl;
            cout << (ui->getLanguage() ? "Owner Account: " : "���� ��ȣ: ") << owner_account << endl;
        }
        else if (transaction_type == "Withdraw") {
            cout << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << endl;
            cout << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << endl;
            cout << (ui->getLanguage() ? "Owner Name: " : "�����ָ�: ") << owner << endl;
            cout << (ui->getLanguage() ? "Owner Bank: " : "���� ����: ") << owner_bank << endl;
            cout << (ui->getLanguage() ? "Owner Account: " : "���� ��ȣ: ") << owner_account << endl;
        }
        else if (transaction_type == "Cash Transfer") {
            cout << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << endl;
            cout << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << endl;
            cout << (ui->getLanguage() ? "Receiver Bank: " : "�޴� ���� ����: ") << receiver_bank << endl;
            cout << (ui->getLanguage() ? "Receiver Account: " : "�޴� ���� ��ȣ: ") << receiver_account << endl;
            cout << (ui->getLanguage() ? "Transfer Type: " : "��ü ����: ") << transfer_type << endl;
        }
        else if (transaction_type == "Account Transfer") {
            cout << (ui->getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_bank << endl;
            cout << (ui->getLanguage() ? "ATM Serial Number: " : "ATM SN ��ȣ: ") << SN << endl;
            cout << (ui->getLanguage() ? "Owner Name: " : "������ �����ָ�: ") << owner << endl;
            cout << (ui->getLanguage() ? "Owner Bank: " : "������ ���� ����: ") << owner_bank << endl;
            cout << (ui->getLanguage() ? "Owner Account: " : "������ ���� ��ȣ: ") << owner_account << endl;
            cout << (ui->getLanguage() ? "Transfer Type: " : "��ü ����: ") << transfer_type << endl;
        }
        else {
            cout << (ui->getLanguage() ? "Unknown Transaction Type" : "�� �� ���� �ŷ� ����") << endl;
        }
        cout << "-------------------------------------------" << endl;
    }
};

// ================================== CARD ================================== \\

class Card {
public:
    string cardNumber;
    string password;
    bool isAdmin;
    Card() : cardNumber(""), password(""), isAdmin(false) {}
    Card(string number, string pwd, bool admin = false)
        : cardNumber(number), password(pwd), isAdmin(admin) {}
};

// ================================== ATM ================================== \\

class ATM {
private:
    string atmBank;
    string atmBankName;
    Account* account;
    Bank* bank;
    string atmNumber;
    string atmID;
    Interface* ui;
    int cash[4]{ 0, 0, 0, 0 };
    int transaction_order = 0;
    int num_of_transaction = 0;
    vector<record*> transaction_records;
    map<string, string> userPasswords;
    map<string, Card> cards;
    map<string, string> userInfo;
    int loginAttempts = 0;
    Bank* primaryBank;
    bool isSingleBankMode;
    bool isBilingual;
    Interface* atmInterface;

public:
    ATM(Bank* atmBank, bool issingle, bool isBilingual, int arr[4], const string& atmId, Interface* uiinterface)
        : bank(atmBank), isSingleBankMode(issingle), isBilingual(isBilingual), atmID(atmId), ui(uiinterface) {
        for (int i = 0; i < 4; ++i) {
            cash[i] = arr[i];
        }
        if (bank) {
            atmBankName = bank->getBankName();
        }
        atmInterface = new Interface(ui->getLanguage());
    }
    string getatmNumber() { return atmNumber; }
    string getremainingcash(int i) { return to_string(cash[i]); }
    void transactionHistory(bool exit) const {
        display_history(exit, transaction_records, *ui);
    }
    void singleBankMode(bool SingleBankMode);
    void addCard(const string& cardNumber, const string& password, const string& userID, bool isAdmin);
    string getBankNumber();
    string getatmID() { return atmID; }
    string getatmbank() const { return atmBankName; }
    bool issinglemode() { return isSingleBankMode; }
    int cashinatm() const { return cash[0] * 1000 + cash[1] * 5000 + cash[2] * 10000 + cash[3] * 50000; }
    void adminMenu();
    void userMenu(ATM* selectedATM);
    bool payFee(int fee);
    bool isInteger(const std::string& input);
    bool isPositive(int input);
    bool hasSufficientCash(int amount);
    void dispenseCash(int amount);
    void addCashToATM(int m1, int m2, int m3, int m4, int fee = 0);
    int getValidInput(const string& prompt, Interface* ui);
    bool depositCash(Interface* ui, int& m1, int& m2, int& m3, int& m4, int& totalBills);
    bool deposit();
    bool fee_cash_calculator(int fee);
    bool change_ATM_dec(int change);
    bool fee_account_calculator(int fee);
    bool withdraw();
    bool transfer();
    void processTransaction(int depositAmount, const string& cardBank);
    string transactionid();
    void transaction_recording(const string& parameter1, const string& parameter2,
        const string& parameter3, const int& parameter4,
        const string& parameter5, const string& parameter6,
        const string& parameter7, const string& parameter8,
        const string& parameter9, const string& parameter10 = "");
    void setAccount(Account* acc) {
        account = acc;
    }
    const vector<record*>& getTransactionRecords() const {
        return transaction_records;
    }
    bool getIsBilingual() const { return isBilingual; }
    string getLanMode() {
        if (isBilingual) { return "Bilingual"; }
        else { return"Uinbilingual"; }
    }
    Interface* getAtmInterface() { return atmInterface; }
    void setAtmLanguage(bool isEng) {
        atmInterface->setLanguage(isEng);
    }
};

class singleATM : public ATM {
public:
    singleATM(Bank* bank, bool issingle, bool isBilingual, int arr[4], const string& id, Interface* ui)
        : ATM(bank, issingle, isBilingual, arr, id, ui) {
        num_of_ATM += 1;
    }
};

class multiATM : public ATM {
public:
    multiATM(Bank* bank, bool issingle, bool isBilingual, int arr[4], const string& id, Interface* ui)
        : ATM(bank, issingle, isBilingual, arr, id, ui) {
        num_of_ATM += 1;
    }
};

void display_atm(vector<ATM*> atm_list, Interface ui) {
    if (atm_list.empty()) {
        cout << (ui.getLanguage() ? "**********There are no ATMs created**********" : "**********������ ATM�� �����ϴ�**********") << endl;
    }
    else {
        cout << "==============================ATM==============================\n";
        for (int i = 0; i < atm_list.size(); i++) {
            cout << "ATM [SN:" << atm_list[i]->getatmID() << "]\nremaining cash : {KRW 50000 : " << atm_list[i]->getremainingcash(3) << ", KRW 10000 : " << atm_list[i]->getremainingcash(2) << ", KRW 5000 : " << atm_list[i]->getremainingcash(1)
                << ", KRW 1000 : " << atm_list[i]->getremainingcash(0) << "} " << endl;
            cout << "---------------------------------------------------------------\n";
        }
        cout << "" << endl;
    }
}

void display_account(vector<Bank*> bank_list, Interface ui) {
    if (bank_list.empty()) {
        cout << (ui.getLanguage() ? "**********There are no Banks created**********"
            : "**********������ ������ �����ϴ�.**********") << endl;
    }
    else {
        bool account_exist = false;
        for (int i = 0; i < bank_list.size(); i++) {
            if (bank_list[i]->getNumOfAccounts() != 0) {
                account_exist = true;
            }
        }
        if (!account_exist) {
            cout << (ui.getLanguage() ? "**********There are no Accounts created**********"
                : "**********������ ���°� �����ϴ�**********") << endl;
        }
        else {
            cout << (ui.getLanguage() ? "============================ACCOUNT============================\n"
                : "============================���� ����============================\n");
            for (int i = 0; i < bank_list.size(); i++) {
                for (int j = 0; j < bank_list[i]->getNumOfAccounts(); j++) {
                    cout << (ui.getLanguage() ? "Account [Bank:" : "���� [����:")
                        << bank_list[i]->getBankName()
                        << (ui.getLanguage() ? ", No : " : ", ���� ��ȣ: ")
                        << bank_list[i]->access_to_account(j)->getAccountNumber()
                        << (ui.getLanguage() ? ", Owner : " : ", ������: ")
                        << bank_list[i]->access_to_account(j)->getOwnerName()
                        << "]\n";

                    cout << (ui.getLanguage() ? " balance: " : " �ܾ�: ")
                        << std::fixed << std::setprecision(0)
                        << bank_list[i]->access_to_account(j)->getAvailableFund()
                        << (ui.getLanguage() ? " won" : " ��") << endl;

                    cout << (ui.getLanguage() ? "---------------------------------------------------------------\n"
                        : "---------------------------------------------------------------\n");
                }
            }
        }
    }
}

// ================================== BANK ================================== \\

bool Bank::isPositive(int input) {
    if (input < 0) {
        cout << (ui->getLanguage() ? "Invalid Number\nTry Again" : "�ùٸ� ���ڰ� �ƴմϴ�.\n�ٽ� �õ��� �ּ���.") << endl;
        return false;
    }
    return true;
}

bool Bank::deleteAccount(const string& accountNumber) {
    for (int i = 0; i < 100; ++i) {
        if (accounts[i] != nullptr && accounts[i]->getAccountNumber() == accountNumber) {
            accounts[i] = accounts[num_of_accounts - 1];
            delete accounts[i];
            accounts[num_of_accounts] == nullptr;
            num_of_accounts--;
            return true;
        }
    }
    return false;
}

void Bank::listAccounts() const {
    if (num_of_accounts == 0) {
        cout << this->getBankName() << (ui->getLanguage() ? ": No registered accounts." : ": ��ϵ� ���°� �����ϴ�.") << endl;
        return;
    }
    cout << (ui->getLanguage() ? "Bank " : "���� ") << bank_name
        << (ui->getLanguage() ? "'s account list" : "�� ���� ���:") << endl;
    for (int i = 0; i < num_of_accounts; ++i) {
        Account* account = accounts[i];
        cout << std::fixed << std::setprecision(0);
        cout << "======================================================================" << endl;
        cout << (ui->getLanguage() ? "Account information\n" : "���� ����\n");
        cout << (ui->getLanguage() ? "- Owner: " : "- ������: ") << account->getOwnerName() << "\n";
        cout << (ui->getLanguage() ? "- Bank Name: " : " - ���� ��: ") << bank_name << "\n";
        cout << (ui->getLanguage() ? "- Account Number: " : " - ���� ��ȣ: ") << account->getAccountNumber() << "\n";
        cout << (ui->getLanguage() ? "- Card Number: " : " - ī�� ��ȣ: ") << account->getCardNumber() << "\n";
        cout << (ui->getLanguage() ? "- Balance: " : " - �ܾ�: ") << account->getAvailableFund() << (ui->getLanguage() ? " won\n" : " ��\n");
        cout << "======================================================================" << endl;
    }
}

Account* Bank::make_account(string new_owner_name, string bankname, long long int initial_balance, string password) {
    if (num_of_accounts >= 100) {
        cout << "���� ���� ����: �ִ� ���� ���� �ʰ��߽��ϴ�.\n";
        return nullptr;
    }
    string account_number = bank_number + to_string(num_of_accounts + 1);
    string card_number = bank_number;
    for (int i = 0; i < 8; ++i) {
        card_number += to_string(rand() % 10);
    }
    Account* new_account = new Account(account_number, new_owner_name, card_number, initial_balance, this, password);
    accounts[num_of_accounts++] = new_account;
    cout << (ui->getLanguage() ? "Account is initialized.\n" : "���°� �����Ǿ����ϴ�.\n");
    cout << "======================================================================" << endl;
    cout << (ui->getLanguage() ? "<Account information>\n" : "<���� ����>\n");
    cout << (ui->getLanguage() ? "- Owner: " : "- ������: ") << new_owner_name << "\n";
    cout << (ui->getLanguage() ? "- Bank Name: " : " - ���� ��: ") << bankname << "\n";
    cout << (ui->getLanguage() ? "- Account Number: " : " - ���� ��ȣ: ") << account_number << "\n";
    cout << (ui->getLanguage() ? "- Card Number: " : " - ī�� ��ȣ: ") << card_number << "\n";
    cout << (ui->getLanguage() ? "- Initial Balance: " : " - �ʱ� �ܾ�: ") << initial_balance << (ui->getLanguage() ? " won\n" : " ��\n");
    cout << "======================================================================" << endl;
    return new_account;
}

Account* Bank::getAccount(int index) const {
    if (index >= 0 && index < num_of_accounts) {
        return accounts[index];
    }
    return nullptr;
}

int Bank::getNumOfAccounts() const {
    return num_of_accounts;
}
Account* Bank::access_to_account(int i) { return accounts[i]; }
Account* Bank::access_to_account_by_num(string num) {
    for (int i = 0; i < this->getNumOfAccounts(); i++) {
        if (num == accounts[i]->getAccountNumber()) {
            return accounts[i];
        }
    }
    return nullptr;
}

int Bank::calculateFee(string transaction_type, string cardBank = "", string target_bank = "") {
    if (transaction_type == "transfer") {
        if (!cardBank.empty() && !target_bank.empty()) {
            if (cardBank == bank_number && target_bank == bank_number) {
                return 2000;
            }
            else if (cardBank != bank_number && target_bank != bank_number) {
                return 4000;
            }
            else if (cardBank == bank_number && target_bank != bank_number) {
                return 3000;
            }
        }
        else {
            return 1000;
        }
    }
    else {
        return (cardBank == bank_name) ? 1000 : 2000;
    }
}

bool Bank::hasAccount(string accountnumber) {
    for (int i = 0; i < num_of_accounts; i++) {
        if (accounts[i]->getAccountNumber() == accountnumber) {
            return true;
        }
    }
    return false;
}

void Bank::increase_receiver_balance(string receiver_account_number, double amount) {
    for (int i = 0; i < num_of_accounts; i++) {
        if (accounts[i]->getAccountNumber() == receiver_account_number) {
            accounts[i]->increase_account_balance(amount);
        }
    }

}

bool Bank::show_authorize(string account_num, string input_password) {
    for (int i = 0; i < num_of_accounts; i++) {
        if (accounts[i]->getAccountNumber() == account_num) {
            if (accounts[i]->getPassword() == input_password) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

// ================================== ACCOUNT ================================== \\

string Account::getBankName() { return account_bank->getBankName(); }

// ================================== ATM ================================== \\

void ATM::singleBankMode(bool SingleBankMode) {
    SingleBankMode = isSingleBankMode;
}

void ATM::addCard(const string& cardNumber, const string& password, const string& userID, bool isAdmin) {
    cards[cardNumber] = Card(cardNumber, password, isAdmin);
}

string ATM::getBankNumber() {
    return bank->getBankNumber();
}

void ATM::adminMenu() {
    while (true) {
        ui->showAdminMenu();
        string selection;
        cin >> selection;
        if (selection == "1") {
            cout << (ui->getLanguage() ? "Viewing transaction history..." : "�ŷ� ���� ��ȸ...") << endl;
            display_history("admin", transaction_records, ui);
        }
        else if (selection == "2") {
            cout << (ui->getLanguage() ? "Exiting admin menu. Returning to main menu.\n" : "������ �޴��� �����մϴ�. ���� �޴��� ���ư��ϴ�.\n");
            break;
        }
        else {
            cout << (ui->getLanguage() ? "Invalid option. Try again.\n" : "�߸��� �����Դϴ�. �ٽ� �õ��� �ּ���.\n");
        }
    }
}

void ATM::userMenu(ATM* selectedATM) {
    if (ui == nullptr) {
        cout << "Error: UI interface is not initialized.\n";
        return;
    }

    // Bilingual ���ο� ���� ��� ����
    if (isBilingual) {
        int languageSelection = 0;
        cout << "Select Language:\n1. English\n2. �ѱ���\n";
        while (true) {
            cin >> languageSelection;
            if (languageSelection == 1) {
                ui->setLanguage(true); // ����� ����
                break;
            }
            else if (languageSelection == 2) {
                ui->setLanguage(false); // �ѱ���� ����
                break;
            }
            else {
                cout << "Invalid option. Please select again: ";
            }
        }
    }

    cout << "Entering userMenu..." << endl;
    while (true) {
        ui->showUserMenu();
        cout << (ui->getLanguage() ? "Select an option: " : "�ɼ��� �����ϼ���: ");
        string selection;
        cin >> selection;

        if (selection == "1") {
            cout << (ui->getLanguage() ? "Selected Deposit menu..." : "�Ա� �޴��� �����߽��ϴ�...") << endl;
            if (!deposit()) {
                cout << (ui->getLanguage() ? "Deposit failed. Returning to user menu..." : "�Ա� ����. ����� �޴��� ���ư��ϴ�...") << endl;
            }
        }
        else if (selection == "2") {
            cout << (ui->getLanguage() ? "Selected Withdraw menu..." : "��� �޴��� �����߽��ϴ�...") << endl;
            if (!withdraw()) {
                cout << (ui->getLanguage() ? "Withdraw failed. Returning to user menu..." : "��� ����. ����� �޴��� ���ư��ϴ�...") << endl;
            }
        }
        else if (selection == "3") {
            cout << (ui->getLanguage() ? "Selected Transfer menu..." : "�۱� �޴��� �����߽��ϴ�...") << endl;
            if (!transfer()) {
                cout << (ui->getLanguage() ? "Transfer failed. Returning to user menu..." : "�۱� ����. ����� �޴��� ���ư��ϴ�...") << endl;
            }
        }
        else if (selection == "4") {
            cout << (ui->getLanguage() ? "Exiting ATM. Please take your card.\n" : "ATM�� �����մϴ�. ī�带 ����������.\n");
            break;
        }
        else {
            cout << (ui->getLanguage() ? "Invalid option. Please try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��� �ּ���.\n");
        }
    }
}

bool ATM::isInteger(const std::string& input) {
    if (input.empty()) {
        cout << (ui->getLanguage() ? "Please enter a valid numeric value." : "�ùٸ� ���� ���� �Է��� �ּ���.") << endl;
        return false;
    }
    size_t start = (input[0] == '-') ? 1 : 0;
    for (size_t i = start; i < input.size(); ++i) {
        if (!isdigit(input[i])) {
            cout << (ui->getLanguage() ? "Please enter a valid numeric value." : "�ùٸ� ���� ���� �Է��� �ּ���.") << endl;
            return false;
        }
    }
    return true;
}

bool ATM::isPositive(int input) {
    if (input < 0) {
        cout << (ui->getLanguage() ? "Negative numbers cannot be entered.\nPlease try again." : "������ �Է��� �� �����ϴ�.\n�ٽ� �õ��� �ּ���.") << endl;
        return false;
    }
    return true;
}

bool ATM::hasSufficientCash(int amount) {
    int totalCash = cash[0] * 1000 + cash[1] * 5000 + cash[2] * 10000 + cash[3] * 50000;
    return totalCash >= amount;
}

void ATM::dispenseCash(int amount) {
    int denominations[4] = { 50000, 10000, 5000, 1000 };
    int numNotes[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++) {
        numNotes[i] = min(amount / denominations[i], cash[3 - i]);
        amount -= numNotes[i] * denominations[i];
        cash[3 - i] -= numNotes[i];
    }
    cout << (ui->getLanguage() ? "Dispensed cash as follows:\n" : "������ ���� ������ ���޵Ǿ����ϴ�:\n");
    for (int i = 0; i < 4; i++) {
        cout << denominations[i] << (ui->getLanguage() ? " won: " : "��: ") << numNotes[i] << endl;
    }
    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
    cin.ignore();
    cin.get();
}

void ATM::addCashToATM(int m1, int m2, int m3, int m4, int fee) {
    int denominations[4] = { 1000, 5000, 10000, 50000 };
    int numNotes[4] = { m1, m2, m3, m4 };
    int totalBills = 0;
    for (int i = 0; i < 4; ++i) {
        cash[i] += numNotes[i];
        totalBills += numNotes[i] * denominations[i];
    }
    if (fee > 0) {
        cash[0] += fee / 1000;
    }
    cout << (ui->getLanguage() ? "Cash added to ATM: " : "ATM�� �߰��� ����: ") << totalBills << " won\n";
    cout << (ui->getLanguage() ? "Adding fee to ATM: " : "ATM�� �߰��� ������: ") << fee << " won\n";
}

int ATM::getValidInput(const string& prompt, Interface* ui) {
    int value;
    while (true) {
        cout << prompt;
        value = globalinput_int(atm_list, bank_list, ui);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << (ui->getLanguage() ? "Wrong Input. Please enter a valid integer." : "�߸��� �Է��Դϴ�. ��ȿ�� ������ �Է��ϼ���.") << endl;
            continue;
        }
        if (value == -1) {
            return -1;
        }
        if (value >= 0 && value <= numeric_limits<int>::max()) {
            return value;
        }
        else {
            if (ui->getLanguage()) {
                cout << "Invalid input. Please enter a number between 0 and " + to_string(numeric_limits<int>::max()) + "." << endl;
            }
            else {
                cout << "�߸��� �Է��Դϴ�. 0���� " + to_string(numeric_limits<int>::max()) + " ������ ���ڸ� �Է��ϼ���." << endl;
            }
        }
    }
}

bool ATM::depositCash(Interface* ui, int& m1, int& m2, int& m3, int& m4, int& totalBills) {
    totalBills = 0;
    int total = 0;
    cout << (ui->getLanguage() ? "Enter the number of bills for each denomination (or enter '0' for all to cancel):\n"
        : "�� ������ ������ �Է��ϼ��� (�Ǵ� ���� '0'�� �Է��� ���):\n");
    while (total <= 50) {
        cout << (ui->getLanguage() ? "Number of 1,000 bills: " : "1,000�� ���� ����: ");
        m1 = getValidInput("", ui);
        total += m1;
        if (total > 50) {
            cout << (ui->getLanguage() ? "Too many bills. Maximum total is 50.\n"
                : "������ ���� 50���� �ʰ��� �� �����ϴ�.\n");
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        cout << (ui->getLanguage() ? "Number of 5,000 bills: " : "5,000�� ���� ����: ");
        m2 = getValidInput("", ui);
        total += m2;
        if (total > 50) {
            cout << (ui->getLanguage() ? "Too many bills. Maximum total is 50.\n"
                : "������ ���� 50���� �ʰ��� �� �����ϴ�.\n");
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        cout << (ui->getLanguage() ? "Number of 10,000 bills: " : "10,000�� ���� ����: ");
        m3 = getValidInput("", ui);
        total += m3;
        if (total > 50) {
            cout << (ui->getLanguage() ? "Too many bills. Maximum total is 50.\n"
                : "������ ���� 50���� �ʰ��� �� �����ϴ�.\n");
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        cout << (ui->getLanguage() ? "Number of 50,000 bills: " : "50,000�� ���� ����: ");
        m4 = getValidInput("", ui);
        total += m4;
        if (total > 50) {
            cout << (ui->getLanguage() ? "Too many bills. Maximum total is 50.\n"
                : "������ ���� 50���� �ʰ��� �� �����ϴ�.\n");
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        break;
    }
    totalBills = (m1 * 1000) + (m2 * 5000) + (m3 * 10000) + (m4 * 50000);
    if (totalBills == 0) {
        cout << (ui->getLanguage() ? "No money entered. Cancelling...\n" : "�Ա� �ݾ��� �����ϴ�. ��� ��...\n");
        return false;
    }
    cout << (ui->getLanguage() ? "Total deposit amount: " : "�� �Ա� �ݾ�: ") << totalBills << " won\n";
    return true;
}

bool ATM::deposit() {
    cout << "Starting deposit process...\n";
    if (account == nullptr) {
        cout << (ui->getLanguage() ? "Error: Account is not initialized.\n" : "���°� �����Ǿ� ���� �ʾҽ��ϴ�.\n");
        return false;
    }
    string cardBank = account->getBankName();
    string atmBank = getatmbank();
    while (true) {
        ui->showDepositMenu();
        int input = getValidInput((ui->getLanguage() ? "Select an option: " : "�ɼ��� �����ϼ���: "), ui);
        if (input == 1) {
            int m1, m2, m3, m4, totalBills = 0;
            if (!depositCash(ui, m1, m2, m3, m4, totalBills)) {
                return false;
            }
            int fee = (cardBank == atmBank) ? 1000 : 2000;
            cout << (ui->getLanguage() ? "Please pay the deposit fee: " : "�Ա� �����Ḧ �������ּ���: ") << fee << " won\n";
            int feePaid = 0;
            if (!payFee(fee)) {
                return false;
            }
            addCashToATM(m1, m2, m3, m4, feePaid);
            transaction_recording(transactionid(), account->getCardNumber(), "Deposit", totalBills, "(Cash Deposit)", atmBank, getatmNumber(), account->getOwnerName(), account->getBankName(), account->getAccountNumber());
            processTransaction(totalBills, cardBank);
            return true;
        }
        else if (input == 2) {
            int checkAmount;
            while (true) {
                ui->promptForCheckDeposit();
                cout << (ui->getLanguage() ? "Please input check (Minimum : 100,000 won): " : "��ǥ�� �Ա����ּ��� (�ּ� �ݾ� : 100,000��): ") << endl;
                checkAmount = globalinput_int(atm_list, bank_list, ui);
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << (ui->getLanguage() ? "Wrong Input. Try Again." : "�߸��� �Է�. �ٽ� �õ�.") << endl;
                    continue;
                }
                if (checkAmount == 0) {
                    ui->transactionCancelled();
                    return false;
                }
                if (checkAmount < 100000) {
                    cout << (ui->getLanguage() ? "Check deposit requires at least 100,000. Please try again." : "��ǥ ������ �ּ� 100,000�� �̻� �����մϴ�. �ٽ� �õ����ּ���.") << endl;
                }
                else {
                    break;
                }
            }
            int fee = (cardBank == atmBank) ? 1000 : 2000;
            cout << (ui->getLanguage() ? "Please pay the deposit fee: " : "�Ա� �����Ḧ �������ּ���: ") << fee << " won" << endl;
            int feePaid = 0;
            if (!payFee(fee)) {
                cout << (ui->getLanguage() ? "Fee payment failed. Cancelling transaction.\n" : "������ ���� ����. �ŷ��� ����մϴ�.\n");
                return false;
            }
            transaction_recording(transactionid(), account->getCardNumber(), "Deposit", checkAmount, "(Check Deposit)", getatmbank(), getatmNumber(), account->getOwnerName(), account->getBankName(), account->getAccountNumber());
            processTransaction(checkAmount, cardBank);
            return true;
        }
        else if (input == 3) {
            ui->transactionCancelled();
            return false;
        }
        else {
            ui->showErrorMessage();
        }
    }
}

bool ATM::fee_cash_calculator(int fee) {
    int denominations[4] = { 50000, 10000, 5000, 1000 };
    int cashAdded[4] = { 0, 0, 0, 0 };
    int total = 0;
    cout << (ui->getLanguage() ? "Please pay the fee: " : "�����Ḧ �������ּ���: ") << fee << " won\n";
    for (int i = 3; i >= 0; i--) {
        while (total < fee) {
            cout << denominations[i] << (ui->getLanguage() ? " won bills: " : "�� ���� ��: ");
            int input = globalinput_int(atm_list, bank_list, ui);
            if (input < 0) {
                cout << (ui->getLanguage() ? "Invalid input. Please enter a positive number of bills." : "�߸��� �Է��Դϴ�. ����� ���� ������ �Է��� �ּ���.") << endl;
                return false;
            }
            total += input * denominations[i];
            cashAdded[i] += input;
            if (total >= fee) break;
        }
    }
    if (total < fee) {
        cout << (ui->getLanguage() ? "Failure to pay fees: Insufficient cash provided." : "������ ���� ����: ���� ����.") << endl;
        return false;
    }
    if (total > fee) {
        int change = total - fee;
        cout << (ui->getLanguage() ? "Change: " : "�Ž�����: ") << change << " won\n";

        if (!change_ATM_dec(change)) {
            cout << (ui->getLanguage() ? "Failed to dispense change due to insufficient ATM cash." : "ATM ���� �������� �Ž����� ���� ����.") << endl;
            return false;
        }
    }
    for (int i = 0; i < 4; i++) {
        cash[i] += cashAdded[3-i];
    }
    cout << (ui->getLanguage() ? "Fee paid successfully. ATM cash updated." : "������ ���� ����. ATM ������ ������Ʈ�Ǿ����ϴ�.") << endl;
    return true;
}

bool ATM::change_ATM_dec(int change) {
    int num_cash[4]{ 0,0,0,0 };
    int real_cash[4]{ 1000, 5000, 10000, 50000 };
    for (int i = 3; i >= 0; i--) {
        while (change >= real_cash[i]) {
            num_cash[i] += 1;
            change -= real_cash[i];
        }
    }
    for (int j = 0; j < 4; j++) {
        if ((cash[j] - num_cash[j]) >= 0) {
        }
        else {
            cout << (ui->getLanguage() ? "Transaction failed: Insufficient cash in ATM" : "�ŷ� ����: ATM �� ���� ����");
            ui->transactionCancelled();
            cout << "======================================================================" << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
    }
    for (int k = 0; k < 4; k++) {
        cash[k] -= num_cash[k];
    }
    return true;
}

bool ATM::payFee(int fee) {
    int m1, m2, m3, m4, totalPaid = 0;
    cout << (ui->getLanguage() ? "Enter bills to pay the fee:\n" : "�����Ḧ ������ ���� ������ �Է��ϼ���:\n");
    m1 = getValidInput((ui->getLanguage() ? "Number of 1,000 bills: " : "1,000�� ���� ����: "), ui);
    m2 = getValidInput((ui->getLanguage() ? "Number of 5,000 bills: " : "5,000�� ���� ����: "), ui);
    m3 = getValidInput((ui->getLanguage() ? "Number of 10,000 bills: " : "10,000�� ���� ����: "), ui);
    m4 = getValidInput((ui->getLanguage() ? "Number of 50,000 bills: " : "50,000�� ���� ����: "), ui);
    totalPaid = (m1 * 1000) + (m2 * 5000) + (m3 * 10000) + (m4 * 50000);
    if (totalPaid < fee) {
        cout << (ui->getLanguage() ? "Insufficient fee payment. Please try again.\n" : "������ ���� �ݾ��� �����մϴ�. �ٽ� �õ��ϼ���.\n");
        cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
        cin.ignore();
        cin.get();
        return false;
    }
    int change = totalPaid - fee;
    if (change > 0) {
        cout << (ui->getLanguage() ? "Change: " : "�Ž�����: ") << change << " won\n";
        int previousCash[4];
        for (int i = 0; i < 4; i++) {
            previousCash[i] = cash[i];
        }
        dispenseCash(change);
        int remainingChange = change;
        for (int i = 0; i < 4; i++) {
            int dispensed = previousCash[i] - cash[i];
            remainingChange -= dispensed * (50000 / (1 << i));
        }
        if (remainingChange > 0) {
            cout << (ui->getLanguage() ? "Error: Unable to dispense change. Transaction cancelled.\n"
                : "����: �Ž����� ���� ����. �ŷ��� ����մϴ�.\n");
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
    }
    addCashToATM(m1, m2, m3, m4);
    cout << (ui->getLanguage() ? "Fee paid successfully.\n" : "������ ���ΰ� �Ϸ�Ǿ����ϴ�.\n");
    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
    cin.ignore();
    cin.get();
    return true;
}

bool ATM::fee_account_calculator(int fee) {
    ui->requestFeePayment(fee);
    cout << (ui->getLanguage() ? "Fee: " : "������: ") << fee << endl;
    cout << (ui->getLanguage() ? "Paying a fee from your account..." : "���¿��� �����Ḧ ���� ��...") << endl;
    double balance = account->getAvailableFund();
    if (balance >= fee) {
        account->decrease_account_balance(fee);
        cout << (ui->getLanguage() ? "Successfully Fee paid!" : "������ ���� ����!") << endl;
        cout << "======================================================================" << endl;
        return true;
    }
    else {
        cout << (ui->getLanguage() ? "Failed to pay fee: Insufficient account balance" : "������ ���� ����: ���� �ܾ� ����") << endl;
        cout << "======================================================================" << endl;

        return false;
    }
}

bool ATM::withdraw() {
    if (ui == nullptr || account == nullptr) {
        cout << (ui->getLanguage() ? "Error: Account is not initialized." : "���°� �����Ǿ� ���� �ʽ��ϴ�.") << endl;
        cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
        cin.ignore();
        cin.get();
        return false;
    }
    ui->showWithdrawOptions();
    string cardBank = account->getAccountNumber().substr(0, 4);
    int withdrawalCount = 0;
    while (withdrawalCount < 3) {
        long long int withdrawAmount = 0;
        cout << (ui->getLanguage() ? "\nAccount Balance: " : "\n���� �ܾ�: ")
            << account->getAvailableFund() << " won\n";
        cout << (ui->getLanguage() ? "The Amount to withdraw: " : "����� �ݾ�: ") << endl;
        withdrawAmount = globalinput_int(atm_list, bank_list, ui);
        if (withdrawAmount == 0) {
            cout << (ui->getLanguage() ? "Returning to Menu..." : "�޴��� ���ư��ϴ�...") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...") << endl;
            cin.ignore();
            cin.get();
            return false;
        }
        if (withdrawAmount > 500000) {
            cout << (ui->getLanguage() ? "Error: Transaction limit is 500,000 won per withdrawal." : "����: �ŷ� �ѵ��� ��� �� 500,000���Դϴ�.") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...") << endl;
            cin.ignore();
            cin.get();
            continue;
        }
        if (withdrawAmount > account->getAvailableFund()) {
            cout << (ui->getLanguage() ? "Error: Insufficient account balance." : "����: ���� �ܾ� ����.") << endl;
            cout << (ui->getLanguage() ? "Returning to Menu..." : "�޴��� ���ư��ϴ�...") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...") << endl;
            cin.ignore();
            cin.get();
            return false;
        }
        if (!hasSufficientCash(withdrawAmount)) {
            cout << (ui->getLanguage() ? "Error: ATM has insufficient cash." : "����: ATM�� ����� ������ �����ϴ�.") << endl;
            cout << (ui->getLanguage() ? "Returning to Menu..." : "�޴��� ���ư��ϴ�...") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...") << endl;
            cin.ignore();
            cin.get();
            return false;
        }
        int fee = (account->getBankName() == bank->getBankName()) ? 1000 : 2000;
        if (account->getAvailableFund() < withdrawAmount + fee) {
            cout << (ui->getLanguage() ? "Error: Insufficient balance to cover the fee." :
                "����: �����Ḧ ������ �ܾ��� �����մϴ�.") << endl;
            cout << (ui->getLanguage() ? "Returning to Menu..." : "�޴��� ���ư��ϴ�...") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...") << endl;
            cin.ignore();
            cin.get();
            return false;
        }
        cout << (ui->getLanguage() ? "Fee paid successfully. ATM cash updated." :
            "������ ���� ����. ATM ������ ������Ʈ�Ǿ����ϴ�.") << endl;
        account->decrease_account_balance(withdrawAmount + fee);
        dispenseCash(withdrawAmount);
        transaction_recording(transactionid(), account->getCardNumber(), "Withdraw", withdrawAmount, getatmbank(), getatmNumber(), account->getOwnerName(), account->getBankName(), account->getAccountNumber());
        cout << (ui->getLanguage() ? "Withdrawal Successful!" : "��� ����!") << endl;
        cout << (ui->getLanguage() ? "Current Balance: " : "���� �ܾ�: ") << account->getAvailableFund() << " won\n";
        cout << (ui->getLanguage() ? "Would you like to make another withdrawal? (1: Yes, 0: No): " : "�߰� ������ ���Ͻø� 1��, �����Ͻ÷��� 0�� �Է��ϼ���: ");
        int continueWithdrawal = globalinput_int(atm_list, bank_list, ui);
        if (continueWithdrawal == 0) {
            cout << (ui->getLanguage() ? "Returning to User Menu..." : "����� �޴��� ���ư��ϴ�...") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        else if (continueWithdrawal == 1) {
            withdrawalCount++;
            continue;
        }
        else if (continueWithdrawal != 1) {
            cout << (ui->getLanguage() ? "Error: Invalid input. Ending withdrawal session." : "����: �߸��� �Է��Դϴ�. ��� ������ �����մϴ�.") << endl;
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        continue;
    }
    cout << (ui->getLanguage() ? "Error: Withdrawal limit exceeded. Returning to User Menu..."
        : "����: �ִ� ���� Ƚ���� �ʰ��߽��ϴ�. ����� �޴��� ���ư��ϴ�...") << endl;
    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
    cin.ignore();
    cin.get();
    return false;
}

bool ATM::transfer() {
    while (true) {
        ui->showTransferOptions();
        cout << (ui->getLanguage() ? "Select an option: " : "�ɼ��� �����ϼ���: ");
        int input = globalinput_int(atm_list, bank_list, ui);
        if (input == 1) {
            cout << (ui->getLanguage() ? "Please enter the account to transfer or '0' to cancel." : "�۱��� ���¸� �Է��ϰų� '0'�� ���� ����ϼ���.") << endl;
            string account_num = globalinput_string(atm_list, bank_list, ui);
            if (account_num == "0") {
                ui->transactionCancelled();
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            if (!isInteger(account_num)) {
                ui->showErrorMessage();
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            bool validAccount = false;
            for (int i = 0; i < bank_list.size(); i++) {
                for (int j = 0; j < bank_list[i]->getNumOfAccounts(); j++) {
                    if (bank_list[i]->access_to_account(j)->getAccountNumber() == account_num) {
                        validAccount = true;
                        break;
                    }
                }
            }
            if (validAccount) {
                cout << (ui->getLanguage() ? "<The amount of transfer>" : "<�۱��� �ݾ�>") << endl;
                int m1 = 0;
                int m2 = 0;
                int m3 = 0;
                int m4 = 0;
                long long transferAmount;
                cout << (ui->getLanguage() ? "Enter the number of bills for each denomination (or enter '0' to cancel):\n"
                    : "�� ������ ������ �Է��ϼ��� (�Ǵ� '0'�� �Է��� ���):\n");
                while (m1 + m2 + m3 + m4 <= 50) {
                    m1 = getValidInput((ui->getLanguage() ? "Number of 1,000 bills: " : "1,000�� ���� ����: "), ui);
                    m2 = getValidInput((ui->getLanguage() ? "Number of 5,000 bills: " : "5,000�� ���� ����: "), ui);
                    m3 = getValidInput((ui->getLanguage() ? "Number of 10,000 bills: " : "10,000�� ���� ����: "), ui);
                    m4 = getValidInput((ui->getLanguage() ? "Number of 50,000 bills: " : "50,000�� ���� ����: "), ui);
                    break;
                }
                if (m1 + m2 + m3 + m4 > 50) {
                    cout << (ui->getLanguage() ? "Do not insert more than number of 50 bills" : "������ 50�� �̻� ���� �� �����ϴ�.");
                    for (size_t i = 0; i < atm_list.size(); ++i) {
                        cout << (ui->getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                        atm_list[i]->transactionHistory(true);
                        continue;
                    }
                    exit(0);
                }
                if (m1 + m2 + m3 + m4 == 0) {
                    cout << (ui->getLanguage() ? "Cancelling...\n" : "��� ��...\n");
                    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                    return false;
                }
                transferAmount = m1 * 1000 + m2 * 5000 + m3 * 10000 + m4 * 50000;
                if (fee_cash_calculator(1000)) {
                    transaction_recording(transactionid(), account->getCardNumber(), "Cash Transfer", transferAmount, getatmbank(), getatmID(), account->getOwnerName(), account->getBankName(), account->getAccountNumber(), "(Cash transfer)");
                    cash[0] += m1;
                    cash[1] += m2;
                    cash[2] += m3;
                    cash[3] += m4;
                    ui->showTransferSuccessUI(account->getAvailableFund());
                    bank->increase_receiver_balance(account_num, transferAmount);
                    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                    return true;
                }
                else {
                    ui->showErrorMessage();
                    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                    return false;
                }
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
            }
            else {
                cout << (ui->getLanguage() ? "Transaction failed: Non-existent account." : "�ŷ� ����: �������� �ʴ� ����.") << endl;
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
        }
        else if (input == 2) {
            cout << (ui->getLanguage() ? "Enter account number to transfer or '0' to cancel." : "�۱��� ���� ��ȣ�� �Է��ϰų� '0'�� ���� ����ϼ���: ") << endl;
            string account_num = globalinput_string(atm_list, bank_list, ui);
            if (account_num == account->getAccountNumber()) {
                cout << (ui->getLanguage() ? "Error: Cannot transfer to your own account." : "����: �ڽ��� ���·� �۱��� �� �����ϴ�.") << endl;
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            if (account_num == "0") {
                ui->transactionCancelled();
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            bool validAccount = false;
            for (int i = 0; i < bank_list.size(); i++) {
                for (int j = 0; j < bank_list[i]->getNumOfAccounts(); j++) {
                    if (bank_list[i]->access_to_account(j)->getAccountNumber() == account_num) {
                        validAccount = true;
                        break;
                    }
                }
            }
            if (!validAccount) {
                cout << (ui->getLanguage() ? "Transaction failed: Non-existent account." : "�ŷ� ����: �������� �ʴ� ����.") << endl;
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            double transferAmount;
            cout << (ui->getLanguage() ? "Enter amount to transfer or '0' to cancel." : "�۱��� �ݾ��� �Է��ϰų� '0'�� ���� ����ϼ���.") << endl;
            while (true) {
                transferAmount = globalinput_double(atm_list, bank_list, ui);
                if (transferAmount == 0) {
                    ui->transactionCancelled();
                    cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                    return false;
                }
                if (isPositive(transferAmount)) break;
                ui->showErrorMessage();
            }
            if (account->getAvailableFund() < transferAmount) {
                cout << (ui->getLanguage() ? "Transaction failed: Insufficient balance." : "�ŷ� ����: �ܾ� ����.") << endl;
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
            account->decrease_account_balance(transferAmount);
            int real_fee;
            real_fee = bank->calculateFee("transfer", account->getCardNumber().substr(0, 4), account_num.substr(0, 4));
            if (fee_account_calculator(real_fee)) {
                transaction_recording(transactionid(), account->getCardNumber(), "Account Transfer", transferAmount, getatmbank(), getatmID(), account->getOwnerName(), account->getBankName(), account->getAccountNumber(), "(Account transfer)");
                bank->increase_receiver_balance(account_num, transferAmount);
                ui->showTransferSuccessUI(account->getAvailableFund());
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return true;
            }
            else {
                account->increase_account_balance(transferAmount);
                ui->showErrorMessage();
                cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                cin.ignore();
                cin.get();
                return false;
            }
        }
        else if (input == 3 or input == 0) {
            ui->transactionCancelled();
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
        else {
            ui->showErrorMessage();
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            return false;
        }
    }
}

void ATM::processTransaction(int depositAmount, const string& cardBank) {
    int fee = bank->calculateFee(cardBank);
    account->increase_account_balance(depositAmount);
}

string ATM::transactionid() {
    return account->getAccountNumber().substr(0, 4) + to_string(++transaction_order);
}

void ATM::transaction_recording(const string& parameter1, const string& parameter2,
    const string& parameter3, const int& parameter4,
    const string& parameter5, const string& parameter6,
    const string& parameter7, const string& parameter8,
    const string& parameter9, const string& parameter10) {
    if (num_of_transaction >= 100) {
        cout << (ui->getLanguage() ? "Transaction history is full. Cannot record more transactions."
            : "�ŷ� ����� ���� á���ϴ�. �� �̻� ����� �� �����ϴ�.") << endl;
        return;
    }
    record* new_record = nullptr;
    if (parameter3 == "Deposit") {
        new_record = new record(parameter1, parameter2,
            parameter3, parameter4,
            parameter5, parameter6,
            parameter7, parameter8,
            parameter9, parameter10);
    }
    else if (parameter3 == "Withdraw") {
        new_record = new record(parameter1, parameter2,
            parameter3, parameter4,
            parameter5, parameter6,
            parameter7, parameter8,
            parameter9);
    }
    else if (parameter3 == "Cash Transfer") {
        new_record = new record(parameter1, parameter2,
            parameter3, parameter4,
            parameter5, parameter6,
            parameter7, parameter8,
            parameter9);
    }
    else if (parameter3 == "Account Transfer") {
        new_record = new record(parameter1, parameter2,
            parameter3, parameter4,
            parameter5, parameter6,
            parameter7, parameter8,
            parameter9, parameter10);
    }
    transaction_records.push_back(new_record);
    cout << (ui->getLanguage() ? "Transaction recorded successfully." : "�ŷ��� ���������� ��ϵǾ����ϴ�.") << endl;
}

void display_history(bool exit, const vector<record*>& transaction_records, const Interface& ui) {
    if (transaction_records.empty()) {
        //////////////
        ui.showErrorUI(ui.getLanguage() ? "There is no transaction history." : "�ŷ� ������ �����ϴ�.");
        ////////////////////
        return;
    }
    if (!exit) {
        ui.clearScreen();
        ui.showTransitionMessage(ui.getLanguage() ? "All Transaction History" : "��ü �ŷ� ����");
    }

    vector<string> transaction_types = { "Deposit", "Withdraw", "Cash Transfer", "Account Transfer" };
    for (const auto& record : transaction_records) {
        for (const auto& type : transaction_types) {
            if (type == record->getTransactionType()) {
                record->display_one_transaction(const_cast<Interface*>(&ui), type);
            }
        }
    }
    if (exit) {
        cout << (ui.getLanguage() ? "Session End" : "���� ����") << endl;
        return;
    }
    cout << (ui.getLanguage() ? "Press any key to continue..." : "����Ϸ��� �ƹ� Ű�� ��������...") << endl;
    cin.ignore();
    cin.get();
    ui.showTransitionMessage(ui.getLanguage() ? "Save to file?" : "���Ϸ� �����Ͻðڽ��ϱ�?");
    cout << (ui.getLanguage() ? "1. Yes\n2. No" : "1. ��\n2. �ƴϿ�") << endl;
    string saveOption;
    cin >> saveOption;
    if (saveOption == "1") {
        ofstream file("AllTransactionHistory.txt");
        if (!file) {
            ui.showErrorUI(ui.getLanguage() ? "Error: Unable to create file." : "����: ������ ������ �� �����ϴ�.");
            return;
        }
        string write;
        for (const auto& record : transaction_records) {
            for (const auto& type : transaction_types) {
                if (type == record->getTransactionType()) {
                    write = record->write_one_transaction(const_cast<Interface*>(&ui), type);
                    record->display_one_transaction(const_cast<Interface*>(&ui), type);
                }
            }
        }
        file << "************Transaction History************" << endl;
        for (const auto& record : transaction_records) {
            file << write;
        }
        file.close();
        ui.showTransitionMessage(ui.getLanguage() ? "Saved to AllTransactionHistory.txt" : "AllTransactionHistory.txt�� ����Ǿ����ϴ�.");
    }
    else {
        ui.showTransitionMessage(ui.getLanguage() ? "Returning to menu." : "�޴��� ���ư��ϴ�.");
    }
}

string generateAtmID(const string& bankNumber, int atmCount) {
    ostringstream oss;
    oss << bankNumber << setfill('0') << setw(2) << atmCount + 1;
    return oss.str();
}

string globalinput_string(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui) {
    string input;
    while (true) {
        cin >> input;
        if (input == "-1") {
            cout << (ui.getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
            for (size_t i = 0; i < atm_list.size(); ++i) {
                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                atm_list[i]->transactionHistory(true);
                continue;
            }
            exit(0);
        }
        else if (input == "/") {
            display_atm(atm_list, ui);
            display_account(bank_list, ui);
            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            continue;
        }
        return input;
    }
}

double globalinput_double(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui) {
    double input;
    while (true) {
        cin >> input;
        if (input == -1) {
            cout << (ui.getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
            for (size_t i = 0; i < atm_list.size(); ++i) {
                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                atm_list[i]->transactionHistory(true);
                continue;
            }
            exit(0);
        }
        else if (to_string(input) == "/") {
            display_atm(atm_list, ui);
            display_account(bank_list, ui);
            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            continue;
        }
        return input;
    }
}

int globalinput_int(vector<ATM*> atm_list, vector<Bank*> bank_list, Interface ui) {
    string raw_input;
    int input;
    while (true) {
        cin >> raw_input;
        if (raw_input == "-1") {
            cout << (ui.getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
            for (size_t i = 0; i < atm_list.size(); ++i) {
                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                atm_list[i]->transactionHistory(true);
                continue;
            }
            exit(0);
        }
        else if (raw_input == "/") {
            display_atm(atm_list, ui);
            display_account(bank_list, ui);
            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            continue;
        }
        else {
            input = stoi(raw_input);
            return input;
        }
    }
}

int getIntegerInput(const string& prompt, Interface* ui) {
    string raw_input;
    int value;
    while (true) {
        cout << prompt;
        cin >> raw_input;
        if (raw_input == "-1") {
            cout << (ui->getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
            for (size_t i = 0; i < atm_list.size(); ++i) {
                cout << (ui->getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                atm_list[i]->transactionHistory(true);
                continue;
            }
            exit(0);
        }
        else if (raw_input == "/") {
            display_atm(atm_list, ui);
            display_account(bank_list, ui);
            cout << (ui->getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
            cin.ignore();
            cin.get();
            continue;
        }
        else if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Only integers are allowed. Please try again." << endl;
        }
        else {
            int input;
            input = stoi(raw_input);
            return input;
        }
    }
}

// ================================== MAIN ================================== \\

int main() {
    Interface ui;
    int current_atm_num;
    ATM* atm;

    while (true) {
        ui.showStartScreen();
        cout << (ui.getLanguage() ? "Please select an option: " : "�ɼ� ����: ") << endl;
        string startSelection;
        startSelection = globalinput_string(atm_list, bank_list, ui);
        if (startSelection == "1") {
            ui.showTransitionMessage(ui.getLanguage() ? "Moving to Bank Management Menu..." : "���� ���� �޴��� �̵� ��...");
            while (true) {
                ui.showBankManagementMenu();
                cout << (ui.getLanguage() ? "Please select an option: " : "�ɼ� ����: ") << endl;
                string bankSelection;
                bankSelection = globalinput_string(atm_list, bank_list, ui);
                if (bankSelection == "1") {
                    string bankName, bankCode;
                    bool isSame = false;
                    while (true) {
                        cout << (ui.getLanguage() ? "Enter the bank name: " : "���� �̸��� �Է��ϼ���: ");
                        bankName = globalinput_string(atm_list, bank_list, ui);
                        for (int i = 0; i < bank_list.size(); i++) {
                            if (bank_list[i]->getBankName() == bankName) {
                                cout << (ui.getLanguage() ? "The bank already exists." : "�̹� �����ϴ� �����Դϴ�.") << endl;
                                isSame = true;
                                break;
                            }
                        }
                        if (isSame) {
                            continue;
                        }
                        break;
                    }

                    cout << (ui.getLanguage() ? "Bank code is 4 digits." : "���� �ڵ�� 4�ڸ��Դϴ�.") << endl;
                    while (true) {
                        cout << (ui.getLanguage() ? "Enter the bank code: " : "���� �ڵ带 �Է��ϼ���: ");
                        bankCode = globalinput_string(atm_list, bank_list, ui);
                        if (bankCode.length() != 4) {
                            cout << (ui.getLanguage()
                                ? "Error: Bank code must be 4 digits.\nPlease Try again."
                                : "����: ���� �ڵ�� 4�ڸ����� �մϴ�.\n�ٽ� �õ��� �ּ���.")
                                << endl;
                            continue;
                        }
                        bool isDuplicate = false;
                        for (int i = 0; i < bank_list.size(); i++) {
                            if (bank_list[i]->getBankNumber() == bankCode) {
                                cout << (ui.getLanguage() ? "Bank codes cannot be duplicated." : "���� �ڵ�� �ߺ��� �� �����ϴ�.") << endl;
                                isDuplicate = true;
                                break;
                            }
                        }
                        if (isDuplicate) {
                            continue;
                        }
                        break;
                    }
                    bank_list.push_back(new Bank(bankName, bankCode, &ui));
                    cout << (ui.getLanguage() ? "Bank added successfully.\n" : "������ ���������� �߰��Ǿ����ϴ�.\n");
                    cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                }
                else if (bankSelection == "2") {
                    for (int i = 0; i < bank_list.size(); i++) {
                        cout << "======================================================================" << endl;
                        cout << i + 1 << "." << endl;
                        cout << (ui.getLanguage() ? "Bank Name: " : "���� �̸�: ") << bank_list[i]->getBankName() << endl;
                        cout << (ui.getLanguage() ? "Bank Code: " : "���� �ڵ�: ") << bank_list[i]->getBankNumber() << endl << endl;
                    }
                    string bankCode;
                    cout << (ui.getLanguage() ? "Enter the bank code to delete: " : "������ ���� �ڵ带 �Է��ϼ���: ");
                    bankCode = globalinput_string(atm_list, bank_list, ui);
                    bool found = false;
                    for (auto it = bank_list.begin(); it != bank_list.end(); ++it) {
                        if ((*it)->getBankNumber() == bankCode) {
                            cout << "Deleting bank: " << (*it)->getBankNumber() << endl;
                            delete* it;
                            bank_list.erase(it);
                            found = true;
                            cout << (ui.getLanguage() ? "Bank is deleted successfully." : "������ ���������� �����Ǿ����ϴ�.") << endl;
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                            cin.ignore();
                            cin.get();
                            break;
                        }
                    }
                    if (!found) {
                        cout << (ui.getLanguage() ? "Error: Bank not found.\n" : "����: �ش� ������ ã�� �� �����ϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                    }
                }
                else if (bankSelection == "3") {
                    if (bank_list.empty()) {
                        cout << (ui.getLanguage() ? "No banks available. Please add a bank first.\n" : "��ϵ� ������ �����ϴ�. ���� ������ �߰����ּ���.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    cout << (ui.getLanguage() ? "Select a bank to create an account:\n" : "���¸� ������ ������ �����ϼ���:\n");
                    for (size_t i = 0; i < bank_list.size(); ++i) {
                        cout << i + 1 << ". " << bank_list[i]->getBankName() << " (" << bank_list[i]->getBankNumber() << ")\n";
                    }
                    int bankChoice = getIntegerInput(
                        string(ui.getLanguage() ? "Enter the bank number: " : "���� ��ȣ�� �Է��ϼ���: "),
                        &ui
                    );
                    
                    if (bankChoice < 1 || bankChoice > static_cast<int>(bank_list.size())) {
                        cout << (ui.getLanguage() ? "Invalid bank selection.\n" : "�߸��� ���� �����Դϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    Bank* selectedBank = bank_list[bankChoice - 1];
                    string ownerName, password;
                    double initialBalance;
                    cout << (ui.getLanguage() ? "Enter the owner name: " : "���� �������� �̸��� �Է��ϼ���: ");
                    ownerName = globalinput_string(atm_list, bank_list, ui);
                    initialBalance = getIntegerInput(ui.getLanguage() ? "Enter the initial balance: " : "�ʱ� �ܾ��� �Է��ϼ���: ", &ui);
                    cout << (ui.getLanguage() ? "Enter the password: " : "��й�ȣ�� �Է��ϼ���: ");
                    password = globalinput_string(atm_list, bank_list, ui);
                    selectedBank->make_account(ownerName, selectedBank->getBankName(), initialBalance, password);
                    cout << (ui.getLanguage() ? "Account created successfully.\n" : "���°� ���������� �����Ǿ����ϴ�.\n") << endl;
                    cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                }
                else if (bankSelection == "4") {
                    if (bank_list.empty()) {
                        cout << (ui.getLanguage() ? "No banks available.\n" : "��ϵ� ������ �����ϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    cout << (ui.getLanguage() ? "Select a bank to delete an account:\n" : "���¸� ������ ������ �����ϼ���:\n");
                    for (int i = 0; i < bank_list.size(); i++) {
                        cout << "======================================================================" << endl;
                        cout << i + 1 << ". " << endl;
                        bank_list[i]->listAccounts();
                    }
                    int bankChoice = getIntegerInput(ui.getLanguage() ? "Enter the bank number: " : "���� ��ȣ�� �Է��ϼ���: ", &ui);
                    if (bankChoice < 1 || bankChoice > static_cast<int>(bank_list.size())) {
                        cout << (ui.getLanguage() ? "Invalid bank selection.\n" : "�߸��� ���� �����Դϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    Bank* selectedBank = bank_list[bankChoice - 1];
                    if (!selectedBank) {
                        cout << (ui.getLanguage() ? "Error: Invalid bank selected.\n" : "����: �߸��� ������ ���õǾ����ϴ�.\n");
                        continue;
                    }
                    cout << (ui.getLanguage() ? "Enter the account number to delete: " : "������ ���� ��ȣ�� �Է��ϼ���: ");
                    string accountNumber;
                    cin >> accountNumber;
                    if (selectedBank->deleteAccount(accountNumber)) {
                        cout << (ui.getLanguage() ? "Account removed successfully.\n" : "���°� ���������� �����Ǿ����ϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                    }
                    else {
                        cout << (ui.getLanguage() ? "Error: Account number not found.\n" : "����: �ش� ���� ��ȣ�� ã�� �� �����ϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                    }
                }
                else if (bankSelection == "5") {
                    if (bank_list.empty()) {
                        cout << (ui.getLanguage() ? "No Accounts available.\n" : "��ϵ� ���°� �����ϴ�.\n");
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    ui.clearScreen();
                    for (int i = 0; i < bank_list.size(); i++) {
                        cout << "======================================================================" << endl;
                        cout << i + 1 << ". " << endl;
                        bank_list[i]->listAccounts();
                    }
                    cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                    cin.ignore();
                    cin.get();
                }
                else if (bankSelection == "6") {
                    ui.showTransitionMessage(ui.getLanguage() ? "Returning to Main Menu..." : "���� �޴��� ���ư��ϴ�...");
                    break;
                }
                else {
                    cout << (ui.getLanguage() ? "Invalid option. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
                }
            }
        }
        else if (startSelection == "2") {
            ui.showTransitionMessage(ui.getLanguage() ? "Moving to ATM Management Menu..." : "ATM ���� �޴��� �̵� ��...");
            while (true) {
                ui.showAMTManagementMenu();
                cout << (ui.getLanguage() ? "Please select an option: " : "�ɼ� ����: ") << endl;
                string ATMSelection;
                ATMSelection = globalinput_string(atm_list, bank_list, ui);
                if (ATMSelection == "1") {
                    string bankname;
                    int bank_index;
                    if (bank_list.empty()) {
                        cout << (ui.getLanguage() ? "No bank to create ATM. " : "ATM�� ������ ������ �������� �ʽ��ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    cout << (ui.getLanguage() ? "Please select a Main Bank" : "�� ������ �Է��� �ּ���.") << endl;
                    cout << (ui.getLanguage() ? "If you wish to cancle, select '0'." : "����ϰ� ������ '0'�� ���� �ּ���.") << endl;
                    cout << (ui.getLanguage() ? "List of Banks" : "���� ���") << endl;
                    for (int i = 0; i < bank_list.size(); i++) {
                        cout << i + 1 << ". ";
                        cout << bank_list[i]->getBankName() << endl;
                    }
                    bank_index = globalinput_int(atm_list, bank_list, ui);
                    if (bank_index == 0) {
                        cout << (ui.getLanguage() ? "Creating ATM is canceled." : "ATM ������ ��ҵǾ����ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    bank_index--;
                    if (not (bank_index >= 0) and (bank_index < bank_list.size())) {
                        cout << (ui.getLanguage() ? "Invalid input value." : "�߸��� �Է°��Դϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Creating ATM is canceled." : "ATM ������ ��ҵǾ����ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    while (true) {
                        cout << bank_list[bank_index]->getBankName() << (ui.getLanguage() ? " Generating ATM." : " ���࿡�� ATM ������ �����մϴ�.") << endl;
                        bool issingle;
                        string singleormulti;
                        cout << (ui.getLanguage() ? "1. Single Bank ATM" : "1. ���� ���� ATM") << endl;
                        cout << (ui.getLanguage() ? "2. Multi Bank ATM" : "2. ���� ���� ATM") << endl;
                        singleormulti = globalinput_string(atm_list, bank_list, ui);
                        if (singleormulti == "1") {
                            cout << (ui.getLanguage() ? "Generating Single Bank ATM." : "���� ���� ATM�� ����ϴ�.") << endl;
                            issingle = true;
                        }
                        else if (singleormulti == "2") {
                            cout << (ui.getLanguage() ? "Generating Multi Bank ATM." : "���� ���� ATM�� ����ϴ�.") << endl;
                            issingle = false;
                        }
                        else {
                            cout << (ui.getLanguage() ? "Invalid option. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                            cin.ignore();
                            cin.get();
                            continue;
                        }
                        int cashes[4]{ 0, 0, 0, 0 };
                        int cash1, cash2, cash3, cash4;
                        cout << (ui.getLanguage() ? "Enter the initial cash amount for the ATM." : "ATM�� �ʱ� �ݾ��� �Է��� �ּ���.") << endl;
                        cout << (ui.getLanguage() ? "Enter the number of bills for each denomination." : "�� ������ �ش��ϴ� ������ ���� �Է��� �ּ���.") << endl;
                        cout << (ui.getLanguage() ? "1,000 bills: " : "1,000: ");
                        cash1 = globalinput_int(atm_list, bank_list, ui);
                        cout << endl << (ui.getLanguage() ? "5,000 bills: " : "5,000: ");
                        cash2 = globalinput_int(atm_list, bank_list, ui);
                        cout << endl << (ui.getLanguage() ? "10,000 bills: " : "10,000: ");
                        cash3 = globalinput_int(atm_list, bank_list, ui);
                        cout << endl << (ui.getLanguage() ? "50,000 bills: " : "50,000: ");
                        cash4 = globalinput_int(atm_list, bank_list, ui);
                        cashes[0] = cash1;
                        cashes[1] = cash2;
                        cashes[2] = cash3;
                        cashes[3] = cash4;
                        cout << (ui.getLanguage() ? "Total initial cash in the ATM: " : "ATM�� �ʱ� �ݾ�: ")
                            << cashes[0] * 1000 + cashes[1] * 5000 + cashes[2] * 10000 + cashes[3] * 50000 << endl;
                        cout << (ui.getLanguage() ? "Creating ATM..." : "ATM ���� �� ...") << endl;
                        string atmId = generateAtmID(bank_list[bank_index]->getBankNumber(), atm_list.size());
                        cout << (ui.getLanguage() ? "Choose ATM language mode:\n1. Unilingual\n2. Bilingual\n"
                            : "ATM ��� ��带 �����ϼ���:\n1. ���� ���\n2. ���� ���\n");
                        int langMode = globalinput_int(atm_list, bank_list, ui);
                        bool isBilingual = (langMode == 2);
                        if (langMode != 1 && langMode != 2) {
                            cout << (ui.getLanguage() ? "Invalid selection. Defaulting to Unilingual mode.\n"
                                : "�߸��� �����Դϴ�. ���� ��� ���� �����˴ϴ�.\n");
                            isBilingual = false;
                        }
                        if (issingle) {
                            atm_list.push_back(new singleATM(bank_list[bank_index], issingle, isBilingual, cashes, atmId, &ui));
                            cout << (ui.getLanguage() ? "ATM is initialized successfully." : "ATM�� ���������� �����Ǿ����ϴ�.") << endl;
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                            cin.ignore();
                            cin.get();
                            break;
                        }
                        else {
                            atm_list.push_back(new multiATM(bank_list[bank_index], issingle, isBilingual, cashes, atmId, &ui));
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                            cin.ignore();
                            cin.get();
                            break;
                        }
                    }
                    current_atm_num = bank_index;
                }
                else if (ATMSelection == "2") {
                    if (atm_list.empty()) {
                        cout << (ui.getLanguage() ? "No ATM to delete." : "������ ATM�� �������� �ʽ��ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    while (true) {
                        cout << "======================================================================" << endl;
                        for (int i = 0; i < atm_list.size(); i++) {
                            cout << i + 1 << "." << endl;
                            cout << (ui.getLanguage() ? "ATM ID: " : "ATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                            cout << (ui.getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_list[i]->getatmbank() << endl;
                            cout << (ui.getLanguage() ? "ATM Mode: " : "ATM ���: ");
                            if (atm_list[i]->issinglemode()) {
                                cout << (ui.getLanguage() ? "Single" : "����") << endl;
                            }
                            else {
                                cout << (ui.getLanguage() ? "Multi" : "����") << endl;
                            }
                            cout << (ui.getLanguage() ? "Cash on ATM: " : "ATM �� ����: ") << atm_list[i]->cashinatm() << endl;
                        }
                        cout << "======================================================================" << endl;
                        cout << (ui.getLanguage() ? "Enter the number of ATM to delete or press 0 to go back: "
                            : "������ ATM ��ȣ�� �Է��ϰų�, 0�� ���� ���� ȭ������ ���ư�����: ") << endl;
                        int num;
                        num = globalinput_int(atm_list, bank_list, ui);
                        if (num == 0) {
                            cout << (ui.getLanguage() ? "Returning to previous menu..." : "���� ȭ������ ���ư��ϴ�...") << endl;
                            break;
                        }
                        if (num > 0 && num <= atm_list.size()) {
                            num--;
                            auto iterator = atm_list.begin() + num;
                            ATM* atm = *iterator;
                            atm_list.erase(iterator);
                            delete atm;
                            cout << (ui.getLanguage() ? "ATM deletion complete." : "ATM�� �����Ǿ����ϴ�.") << endl;
                            break;
                        }
                        else {
                            cout << (ui.getLanguage() ? "Invalid input. Try again." : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.") << endl;
                        }
                    }
                }
                else if (ATMSelection == "3") {
                    if (atm_list.empty()) {
                        cout << (ui.getLanguage() ? "No ATM Exist" : "ATM�� �������� �ʽ��ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    while (true) {
                        cout << "======================================================================" << endl;
                        for (int i = 0; i < atm_list.size(); i++) {
                            cout << i + 1 << "." << endl;
                            cout << (ui.getLanguage() ? "ATM ID: " : "ATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                            cout << (ui.getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_list[i]->getatmbank() << endl;
                            cout << (ui.getLanguage() ? "ATM Mode: " : "ATM ���: ");
                            cout << (ui.getLanguage() ? "Language Mode: " : "��� ���: ") << atm_list[i]->getLanMode() << endl;
                            if (atm_list[i]->issinglemode()) {
                                cout << (ui.getLanguage() ? "Single" : "����") << endl;
                            }
                            else {
                                cout << (ui.getLanguage() ? "Multi" : "����") << endl;
                            }
                            cout << (ui.getLanguage() ? "Cash on ATM: " : "ATM �� ����: ") << atm_list[i]->cashinatm() << endl;
                        }
                        cout << "======================================================================" << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                }
                else if (ATMSelection == "4") {
                    ui.showTransitionMessage(ui.getLanguage() ? "Returning to Main Menu..." : "���� �޴��� ���ư��ϴ�...");
                    break;
                }
                else {
                    cout << (ui.getLanguage() ? "Invalid option. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
                }
            }
        }

        else if (startSelection == "3") {
            while (true) {
                ui.showLanguageSettingsMenu();
                int languageSelection = getIntegerInput(ui.getLanguage() ? "Please select a language: " : "�� �����ϼ���: ", &ui);
                if (languageSelection == 1) {
                    ui.setLanguage(true);
                    ui.clearScreen();
                    cout << "Language changed to English.\n";
                    break;
                }
                else if (languageSelection == 2) {
                    ui.setLanguage(false);
                    ui.clearScreen();
                    cout << "�� �ѱ���� ����Ǿ����ϴ�.\n";
                    break;
                }
                else {
                    cout << (ui.getLanguage() ? "Invalid input. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
                }
            }
        }
        else if (startSelection == "4") {
            ui.clearScreen();
            if (atm_list.empty()) {
                cout << (ui.getLanguage() ? "Please create an ATM first." : "���� ATM�� ������ �ּ���.") << endl;
                cout << (ui.getLanguage() ? "Press Enter to return to the previous menu..." : "���� �޴��� ���ư����� ���� Ű�� ��������...") << endl;
                cin.ignore();
                cin.get();
                continue;
            }
            bool exitToMainMenu = false;
            while (true) {
                cout << (ui.getLanguage() ? "Select ATM for transaction." : "�ŷ��� ������ ATM�� ������ �ּ���.") << endl;
                for (int i = 0; i < atm_list.size(); i++) {
                    cout << i + 1 << "." << endl;
                    cout << (ui.getLanguage() ? "ATM ID: " : "ATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                    cout << (ui.getLanguage() ? "ATM Bank: " : "ATM ����: ") << atm_list[i]->getatmbank() << endl;
                    cout << (ui.getLanguage() ? "ATM Mode: " : "ATM ���: ")
                        << (atm_list[i]->issinglemode() ? (ui.getLanguage() ? "Single" : "����") : (ui.getLanguage() ? "Multi" : "����")) << endl;
                    cout << (ui.getLanguage() ? "Language Mode: " : "��� ���: ") << atm_list[i]->getLanMode() << endl;
                    cout << (ui.getLanguage() ? "Cash on ATM: " : "ATM �� ����: ") << atm_list[i]->cashinatm() << endl;
                }
                string raw_input;
                int atmChoice;
                while (true) {
                    cout << (ui.getLanguage() ? "Enter the number of ATM." : "ATM ��ȣ�� �Է��ϼ���.") << endl;
                    cin >> raw_input;
                    if (raw_input == "-1") {
                        cout << (ui.getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
                        for (size_t i = 0; i < atm_list.size(); ++i) {
                            cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                            atm_list[i]->transactionHistory(true);
                            continue;
                        }
                        exit(0);
                    }
                    else if (raw_input == "/") {
                        display_atm(atm_list, ui);
                        display_account(bank_list, ui);
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    else if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << (ui.getLanguage() ? "Invalid input. Please enter a valid number." : "��ȿ���� ���� �Է��Դϴ�. �ùٸ� ���ڸ� �Է��ϼ���.") << endl;
                        continue;
                    }
                    else {
                        atmChoice = stoi(raw_input);
                    }
                    if (atmChoice == -1) {
                        cout << (ui.getLanguage() ? "Returning to main menu..." : "���� �޴��� ���ư��ϴ�...") << endl;
                        exitToMainMenu = true;
                        break;
                    }
                    if (atmChoice > 0 && atmChoice <= static_cast<int>(atm_list.size())) {
                        break;
                    }
                    else {
                        cout << (ui.getLanguage() ? "Invalid choice. Please select again." : "�߸��� �����Դϴ�. �ٽ� �����ϼ���.") << endl;
                    }
                }   if (exitToMainMenu) break;
                ATM* selectedATM = atm_list[atmChoice - 1];
                while (true) {
                    ui.clearScreen();
                    cout << (ui.getLanguage() ? "Insert your card." : "ī�带 �����ϼ���.") << endl;
                    string cardNumber;
                    cout << (ui.getLanguage() ? "Enter your card number (or '0' to return): " : "ī�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� ���ư��ϴ�): ");
                    cardNumber = globalinput_string(atm_list, bank_list, ui);
                    if (cardNumber == "0") {
                        cout << (ui.getLanguage() ? "Returning to main menu..." : "���� �޴��� ���ư��ϴ�...") << endl;
                        exitToMainMenu = true;
                        break;
                    }
                    else if (cardNumber == "admin" || cardNumber == "ADMIN" || cardNumber == "Admin") {
                        cout << (ui.getLanguage() ? "[Administrator Authentication] Please enter the password.\n"
                            : "[������ ����] ��й�ȣ�� �Է����ּ���.\n");
                        string adminpw;
                        cin >> adminpw;
                        if (adminpw == "admin") {
                            cout << (ui.getLanguage() ? "Authentication successful. Accessing transaction history...\n"
                                : "���� ����. �ŷ� ������ Ȯ���մϴ�...\n");
                            if (atm_list.empty()) {
                                cout << (ui.getLanguage() ? "No ATMs available to display transaction history."
                                    : "�ŷ� ������ ǥ���� ATM�� �����ϴ�.") << endl;
                            }
                            else {
                                for (size_t i = 0; i < atm_list.size(); ++i) {
                                    cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                                    atm_list[i]->transactionHistory(false);
                                    continue;
                                }
                            }
                        }
                        else {
                            cout << (ui.getLanguage() ? "Authentication Failed.\n"
                                : "���� ����\n");
                            continue;
                        }
                    }
                    if (cardNumber.empty() || cardNumber.length() != 12 || !std::all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
                        cout << (ui.getLanguage() ? "Invalid card number. Please try again." : "��ȿ���� ���� ī�� ��ȣ�Դϴ�. �ٽ� �õ��ϼ���.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    if (selectedATM->issinglemode()) {
                        string atmBankNumber = selectedATM->getBankNumber();
                        string cardBankNumber = cardNumber.substr(0, 4);
                        if (atmBankNumber != cardBankNumber) {
                            cout << (ui.getLanguage() ? "Invalid card. This ATM only supports the bank it belongs to." : "��ȿ���� ���� ī���Դϴ�. �� ATM�� �ش� ������ ī�常 �����մϴ�.") << endl;
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                            cin.ignore();
                            cin.get();
                            continue;
                        }
                    }
                    Bank* targetBank = nullptr;
                    Account* targetAccount = nullptr;
                    for (auto bank : bank_list) {
                        if (bank->getBankNumber() == cardNumber.substr(0, 4)) {
                            targetBank = bank;
                            break;
                        }
                    }
                    if (!targetBank) {
                        cout << (ui.getLanguage() ? "Bank associated with card not found. Please try again." : "ī��� ����� ������ ã�� �� �����ϴ�. �ٽ� �õ��ϼ���.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    for (int i = 0; i < targetBank->getNumOfAccounts(); ++i) {
                        Account* account = targetBank->getAccount(i);
                        if (account && account->getCardNumber() == cardNumber) {
                            targetAccount = account;
                            break;
                        }
                    }
                    if (!targetAccount) {
                        cout << (ui.getLanguage() ? "Account associated with card not found. Please try again." : "ī��� ����� ���¸� ã�� �� �����ϴ�. �ٽ� �õ��ϼ���.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    selectedATM->setAccount(targetAccount);
                    int retryCount = 0;
                    const int maxRetries = 3;
                    while (retryCount < maxRetries) {
                        string password;
                        cout << (ui.getLanguage() ? "Enter your password: " : "��й�ȣ�� �Է��ϼ���: ");
                        password = globalinput_string(atm_list, bank_list, ui);
                        string correctPassword = targetAccount->getPassword();
                        if (correctPassword == password) {
                            cout << (ui.getLanguage() ? "Card authentication successful." : "ī�� ���� ����.") << endl;
                            ///////////
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin.get();
                            ///////////////////
                            break;
                        }
                        retryCount++;
                        if (retryCount < maxRetries) {
                            int chancesLeft = maxRetries - retryCount;
                            cout << (ui.getLanguage() ? "Chance Left: " + std::to_string(chancesLeft)
                                : "���� ��ȸ: " + std::to_string(chancesLeft)) << endl;
                            cout << (ui.getLanguage() ? "Incorrect password. Please try again."
                                : "��й�ȣ�� Ʋ�Ƚ��ϴ�. �ٽ� �õ��ϼ���.") << endl;
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin.get();
                        }
                    }
                    if (retryCount >= maxRetries) {
                        /////
                        cout << (ui.getLanguage() ? "Error: Maximum retries exceeded." : "����: �ִ� �õ� Ƚ���� �ʰ��߽��ϴ�.") << endl;
                        cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� ���� Ű�� ��������...");
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin.get();
                        //////
                        for (size_t i = 0; i < atm_list.size(); ++i) {
                            cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                            atm_list[i]->transactionHistory(true);
                            continue;
                        }
                        exit(0);
                    }
                    ui.clearScreen();
                    cout << (ui.getLanguage() ? "Card authentication successful." : "ī�� ���� ����.") << endl;
                    if (selectedATM->getIsBilingual()) {
                        while (true) {
                            ui.showLanguageSettingsMenu();
                            int languageSelection = getIntegerInput(ui.getLanguage() ? "Please select a language: " : "�� �����ϼ���: ", &ui);
                            if (languageSelection == 1) {
                                ui.setLanguage(true);
                                ui.clearScreen();
                                cout << "Language changed to English.\n";
                                break;
                            }
                            else if (languageSelection == 2) {
                                ui.setLanguage(false);
                                ui.clearScreen();
                                cout << "�� �ѱ���� ����Ǿ����ϴ�.\n";
                                break;
                            }
                            else {
                                cout << (ui.getLanguage() ? "Invalid input. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
                            }
                        }
                    }
                    string userOption;
                    while (true) {
                        ui.showUserMenu();
                        cout << (ui.getLanguage() ? "Select an option: " : "�ɼ��� �����ϼ���: ");
                        cin >> userOption;
                        if (userOption == "-1") {
                            cout << (ui.getLanguage() ? "Exiting the program..." : "���α׷��� �����մϴ�...") << endl;
                            for (size_t i = 0; i < atm_list.size(); ++i) {
                                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                                atm_list[i]->transactionHistory(true);
                                continue;
                            }
                            exit(0);
                        }
                        else if (userOption == "/") {
                            display_atm(atm_list, ui);
                            display_account(bank_list, ui);
                            cout << (ui.getLanguage() ? "Press Enter to continue..." : "����Ϸ��� Enter�� ��������...");
                            cin.ignore();
                            cin.get();
                            continue;
                        }
                        else if (cin.fail()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            ui.showErrorMessage();
                            continue;
                        }
                        if (userOption == "1") {
                            if (!selectedATM->deposit()) continue;
                        }
                        else if (userOption == "2") {
                            if (!selectedATM->withdraw()) continue;
                        }
                        else if (userOption == "3") {
                            if (!selectedATM->transfer()) continue;
                        }
                        else if (userOption == "4") {
                            cout << (ui.getLanguage() ? "Exiting to main menu." : "���� �޴��� ���ư��ϴ�.") << endl;
                            exitToMainMenu = true;
                            break;
                        }
                        else {
                            cout << (ui.getLanguage() ? "Invalid option. Try again." : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.") << endl;
                        }
                        break;
                    }
                    if (exitToMainMenu) break;
                    break;
                }
                if (exitToMainMenu) break;
            }
        }
        else if (startSelection == "5") {
            while (true) {
                ui.clearScreen();
                cout << (ui.getLanguage() ? "[This menu is available only for administrators]" : "[�����ڿ��Ը� �����Ǵ� �޴��Դϴ�]") << endl;
                cout << (ui.getLanguage() ? "1. Authenticate as an administrator\n2. Return to Main Menu\n"
                    : "1. ������ �����ϱ�\n2. ���� �޴��� ���ư���\n");
                string administrator;
                administrator = globalinput_string(atm_list, bank_list, ui);
                if (administrator == "1") {
                    cout << (ui.getLanguage() ? "Please authenticate as an administrator." : "������ ������ ���ּ���.") << endl;
                    string auth;
                    auth = globalinput_string(atm_list, bank_list, ui);
                    if (auth == "admin" || auth == "Admin" || auth == "ADMIN") {
                        cout << (ui.getLanguage() ? "Authentication successful. Accessing transaction history...\n"
                            : "���� ����. �ŷ� ������ Ȯ���մϴ�...\n");
                        if (atm_list.empty()) {
                            cout << (ui.getLanguage() ? "No ATMs available to display transaction history."
                                : "�ŷ� ������ ǥ���� ATM�� �����ϴ�.") << endl;
                        }
                        else {
                            for (size_t i = 0; i < atm_list.size(); ++i) {
                                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                                atm_list[i]->transactionHistory(false);
                            }
                        }
                        cout << (ui.getLanguage() ? "Press Enter to return to the menu..." : "�޴��� ���ư����� Enter Ű�� ��������...");
                        cin.ignore();
                        cin.get();
                        break;
                    }
                    else {
                        cout << (ui.getLanguage() ? "[Authentication failed] Returning to menu...\n"
                            : "[���� ����] �޴��� ���ư��ϴ�...\n");
                        continue;
                    }
                }
                else if (administrator == "2") {
                    cout << (ui.getLanguage() ? "Returning to Main Menu..." : "���� �޴��� ���ư��ϴ�...") << endl;
                    break;
                }
                else {
                    cout << (ui.getLanguage() ? "Invalid input. Please try again." : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.") << endl;
                }
            }
        }
        else if (startSelection == "6") {
            ui.showTransitionMessage(ui.getLanguage() ? "Exiting system. Goodbye!" : "�ý����� �����մϴ�. �ȳ��� ������!\n");
            cout << endl;
            for (size_t i = 0; i < atm_list.size(); ++i) {
                cout << (ui.getLanguage() ? "\nATM ID: " : "\nATM ���� ��ȣ: ") << atm_list[i]->getatmID() << endl;
                atm_list[i]->transactionHistory(true);
                continue;
            }
            exit(0);
            break;
        }
        else {
            cout << (ui.getLanguage() ? "Invalid input. Try again.\n" : "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n");
        }
    }
    return 0;
}