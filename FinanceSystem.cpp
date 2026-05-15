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
        cout << "\nUsername:  "; getline(cin, user);
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
    cout << " Password: "; getline(cin, pass);
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