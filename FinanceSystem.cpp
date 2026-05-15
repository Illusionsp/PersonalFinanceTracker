#include "FinanceSystem.h"
#include "Helper.h"
#include <iostream>
#include <cstdlib> 
#include <fstream> 
#include <map>     
#include <limits>

using namespace std;

//CONSTRUCTOR / DESTRUCTOR 
FinanceSystem::FinanceSystem() { 
    conn = mysql_init(0); 
    currentUserID = 0; 
}

FinanceSystem::~FinanceSystem() { 
    if (conn) mysql_close(conn); 
}

//DATABASE CORE 
bool FinanceSystem::runQuery(const string& q) {
    if (mysql_query(conn, q.c_str()) != 0) {
        cout << "\n[DB ERROR] " << mysql_error(conn) << "\n";
        return false;
    }
    return true;
}

std::map<std::string, std::string> FinanceSystem::loadConfig() {
    std::map<std::string, std::string> config;
    std::ifstream file("config.ini");
    string line;
    if (!file.is_open()) return config; 
    while (getline(file, line)) {
        if (line.empty() || line[0] == '[' || line[0] == '#') continue;
        size_t pos = line.find("=");
        if (pos != string::npos) config[line.substr(0, pos)] = line.substr(pos + 1);
    }
    return config;
}

void FinanceSystem::connectDB() {
    auto conf = loadConfig();
    string h = conf.count("host") ? conf["host"] : "127.0.0.1";
    string u = conf.count("user") ? conf["user"] : "root";
    string p = conf.count("password") ? conf["password"] : "";
    string d = conf.count("database") ? conf["database"] : "finance_tracker";
    conn = mysql_real_connect(conn, h.c_str(), u.c_str(), p.c_str(), d.c_str(), 3306, NULL, 0);
    if (!conn) { cout << "\nDB Connection Failed!\n"; exit(1); }
    cout << "\nDB Connected!\n";
}

void FinanceSystem::createTables() {
    runQuery("CREATE TABLE IF NOT EXISTS users(user_id INT AUTO_INCREMENT PRIMARY KEY, username VARCHAR(50) UNIQUE, password_hash VARCHAR(255))");
    runQuery("CREATE TABLE IF NOT EXISTS categories(category_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), type VARCHAR(20), user_id INT NULL)");
    runQuery("CREATE TABLE IF NOT EXISTS income(income_id INT AUTO_INCREMENT PRIMARY KEY, user_id INT, category_id INT, amount DECIMAL(12,2), date DATE, description TEXT)");
    runQuery("CREATE TABLE IF NOT EXISTS expenses(expense_id INT AUTO_INCREMENT PRIMARY KEY, user_id INT, category_id INT, amount DECIMAL(12,2), date DATE, description TEXT)");
    runQuery("CREATE TABLE IF NOT EXISTS budget(budget_id INT AUTO_INCREMENT PRIMARY KEY, user_id INT, category_id INT, limit_amount DECIMAL(12,2), period_start DATE, period_end DATE)");
}

//AUTHENTICATION 
bool FinanceSystem::registerUser() {
    string user, pass; clearInput();
    while(true) {
        cout << "\nUsername: "; getline(cin, user);
        if(!user.empty()) break;
        cout << "[!] Username cannot be empty.\n";
    }
    runQuery("SELECT user_id FROM users WHERE username='" + user + "'");
    MYSQL_RES* res = mysql_store_result(conn);
    if (res && mysql_num_rows(res) > 0) {
        cout << "[!] Username taken!\n"; 
        mysql_free_result(res); return false;
    }
    if(res) mysql_free_result(res);
    cout << "Password: "; getline(cin, pass);
    return runQuery("INSERT INTO users(username,password_hash) VALUES('" + user + "','" + pass + "')");
}

bool FinanceSystem::loginUser() {
    string user, pass; clearInput();
    cout << "\nUsername: "; getline(cin, user);
    cout << "Password: "; getline(cin, pass);
    runQuery("SELECT user_id FROM users WHERE username='" + user + "' AND password_hash='" + pass + "'");
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) { 
        cout << "Invalid Login!\n"; 
        if(res) mysql_free_result(res); return false; 
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    currentUserID = atoi(row[0]);
    mysql_free_result(res);
    return true;
}



// TRANSACTION MANAGEMENT 
void FinanceSystem::addIncome() {
    double amt; string date, desc, catName;
    
    
    while(true) {
        cout << "\nAmount (ETB): ";
        if(cin >> amt && amt > 0) break;
        cout << "[!] Invalid amount. Please enter a positive number.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    clearInput();

    while(true) {
        cout << "Category Name (e.g., Salary, Gift): "; getline(cin, catName);
        if(!catName.empty()) break;
    }

    int catID = 0;
    runQuery("SELECT category_id FROM categories WHERE name='" + catName + "' AND (user_id=" + to_string(currentUserID) + " OR user_id IS NULL)");
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    if (res && (row = mysql_fetch_row(res))) catID = atoi(row[0]);
    else {
        runQuery("INSERT INTO categories(name, type, user_id) VALUES('" + catName + "', 'Income', " + to_string(currentUserID) + ")");
        catID = mysql_insert_id(conn);
    }
    if(res) mysql_free_result(res);

    cout << "Date (YYYY-MM-DD) [Blank for Today]: "; getline(cin, date);
    string dSQL = (date.empty() || date.length() < 10) ? "CURDATE()" : "'" + date + "'";
    cout << "Description: "; getline(cin, desc);

    runQuery("INSERT INTO income(user_id,category_id,amount,date,description) VALUES(" + to_string(currentUserID) + "," + to_string(catID) + "," + to_string(amt) + "," + dSQL + ",'" + desc + "')");
}

void FinanceSystem::addExpense() {
    double amt; string date, desc, catName;
    
    
    while(true) {
        cout << "\nAmount (ETB): ";
        if(cin >> amt && amt > 0) break;
        cout << "[!] Invalid amount. Expense must be positive.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    clearInput();

    while(true) {
        cout << "Category Name (e.g., Food, Rent): "; getline(cin, catName);
        if(!catName.empty()) break;
    }

    int catID = 0;
    runQuery("SELECT category_id FROM categories WHERE name='" + catName + "' AND (user_id=" + to_string(currentUserID) + " OR user_id IS NULL)");
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    if (res && (row = mysql_fetch_row(res))) catID = atoi(row[0]);
    else {
        runQuery("INSERT INTO categories(name, type, user_id) VALUES('" + catName + "', 'Expense', " + to_string(currentUserID) + ")");
        catID = mysql_insert_id(conn);
    }
    if(res) mysql_free_result(res);

    cout << "Date [Blank for Today]: "; getline(cin, date);
    string dSQL = (date.empty() || date.length() < 10) ? "CURDATE()" : "'" + date + "'";
    cout << "Description: "; getline(cin, desc);

    if(runQuery("INSERT INTO expenses(user_id,category_id,amount,date,description) VALUES(" + to_string(currentUserID) + "," + to_string(catID) + "," + to_string(amt) + "," + dSQL + ",'" + desc + "')")) {
        
        double limit = 0, totalSpent = 0;
        runQuery("SELECT limit_amount FROM budget WHERE category_id=" + to_string(catID) + " AND user_id=" + to_string(currentUserID));
        MYSQL_RES* bRes = mysql_store_result(conn); MYSQL_ROW bRow;
        if(bRes && (bRow = mysql_fetch_row(bRes))) limit = atof(bRow[0]);
        if(bRes) mysql_free_result(bRes);

        if(limit > 0) {
            runQuery("SELECT SUM(amount) FROM expenses WHERE category_id=" + to_string(catID) + " AND user_id=" + to_string(currentUserID));
            MYSQL_RES* sRes = mysql_store_result(conn); MYSQL_ROW sRow;
            if(sRes && (sRow = mysql_fetch_row(sRes)) && sRow[0]) totalSpent = atof(sRow[0]);
            if(sRes) mysql_free_result(sRes);

            double usage = (totalSpent / limit) * 100;
            cout << "\n>>> BUDGET STATUS: " << usage << "% used (" << totalSpent << " / " << limit << " ETB)";
            if(usage >= 100) cout << "\n[!!!] ALERT: You have exceeded your limit!";
            else if(usage >= 75) cout << "\n[!] WARNING: Only " << (100-usage) << "% remaining!";
            cout << endl;
        }
    }
}


// VIEW & FILTERS 
void FinanceSystem::viewTransactions() {
    cout << "\n--- INCOME ---\n";
    runQuery("SELECT i.income_id, c.name, i.amount, i.date FROM income i LEFT JOIN categories c ON i.category_id=c.category_id WHERE i.user_id=" + to_string(currentUserID));
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    while (res && (row = mysql_fetch_row(res))) cout << "ID[" << row[0] << "] " << (row[1]?row[1]:"None") << " | " << row[2] << " ETB | " << row[3] << endl;
    if(res) mysql_free_result(res);

    cout << "\n--- EXPENSES ---\n";
    runQuery("SELECT e.expense_id, c.name, e.amount, e.date FROM expenses e LEFT JOIN categories c ON e.category_id=c.category_id WHERE e.user_id=" + to_string(currentUserID));
    res = mysql_store_result(conn);
    while (res && (row = mysql_fetch_row(res))) cout << "ID[" << row[0] << "] " << (row[1]?row[1]:"None") << " | " << row[2] << " ETB | " << row[3] << endl;
    if(res) mysql_free_result(res);
}

void FinanceSystem::filterByCategory() {
    string cat; clearInput();
    cout << "\nCategory Name: "; getline(cin, cat);
    runQuery("SELECT e.amount, e.date, e.description FROM expenses e JOIN categories c ON e.category_id=c.category_id WHERE c.name='" + cat + "' AND e.user_id=" + to_string(currentUserID));
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    cout << "\n--- Results for " << cat << " ---\n";
    while (res && (row = mysql_fetch_row(res))) cout << row[0] << " ETB | " << row[1] << " | " << row[2] << endl;
    if(res) mysql_free_result(res);
}

void FinanceSystem::filterByDate() {
    string d; clearInput();
    cout << "\nDate (YYYY-MM-DD): "; getline(cin, d);
    runQuery("SELECT amount, description FROM expenses WHERE date='" + d + "' AND user_id=" + to_string(currentUserID));
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    while (res && (row = mysql_fetch_row(res))) cout << row[0] << " ETB | " << row[1] << endl;
    if(res) mysql_free_result(res);
}

//  UPDATE / DELETE 
void FinanceSystem::updateTransaction() {
    int t, id; double a;
    cout << "\nUpdate [1] Inc [2] Exp: "; cin >> t;
    string tbl = (t == 1) ? "income" : "expenses";
    string col = (t == 1) ? "income_id" : "expense_id";
    runQuery("SELECT " + col + ", amount FROM " + tbl + " WHERE user_id=" + to_string(currentUserID));
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    while (res && (row = mysql_fetch_row(res))) cout << "ID[" << row[0] << "] " << row[1] << " ETB" << endl;
    if(res) mysql_free_result(res);
    cout << "Enter ID: "; cin >> id;
    
    while(true) {
        cout << "New Amt: "; 
        if(cin >> a && a > 0) break;
        cout << "[!] Must be a positive number.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    runQuery("UPDATE " + tbl + " SET amount=" + to_string(a) + " WHERE " + col + "=" + to_string(id) + " AND user_id=" + to_string(currentUserID));
}

void FinanceSystem::deleteTransaction() {
    int t, id;
    cout << "\nDelete [1] Inc [2] Exp: "; cin >> t;
    string tbl = (t == 1) ? "income" : "expenses";
    string col = (t == 1) ? "income_id" : "expense_id";
    runQuery("SELECT " + col + ", amount FROM " + tbl + " WHERE user_id=" + to_string(currentUserID));
    MYSQL_RES* res = mysql_store_result(conn); MYSQL_ROW row;
    while (res && (row = mysql_fetch_row(res))) cout << "ID[" << row[0] << "] " << row[1] << " ETB" << endl;
    if(res) mysql_free_result(res);
    cout << "Enter ID: "; cin >> id;
    runQuery("DELETE FROM " + tbl + " WHERE " + col + "=" + to_string(id) + " AND user_id=" + to_string(currentUserID));
}
