#include "FinanceSystem.h"
#include <iostream>
#include <fstream> 

FinanceSystem::FinanceSystem() {
    db_connection = mysql_init(NULL);
    if (mysql_real_connect(db_connection, "127.0.0.1", "admin", "", "pft_db", 3306, NULL, 0)) {
        is_offline = false;
    } else {
        is_offline = true;
        std::cout << "[!] Warning: Database connection failed. System running in Offline Mode.\n";
    }
}

FinanceSystem::~FinanceSystem() {
    if (!is_offline) mysql_close(db_connection);
}



bool FinanceSystem::login(std::string username, std::string password) {
    if (is_offline) return false;
    
    std::string query = "SELECT id FROM users WHERE username='" + username + "' AND password='" + password + "'";
    mysql_query(db_connection, query.c_str());
    MYSQL_RES* result = mysql_store_result(db_connection);
    
    if (result && mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        active_user.id = std::stoi(row[0]);
        active_user.username = username;
        mysql_free_result(result);
        return true;
    }
    return false;
}

bool FinanceSystem::registerUser(std::string username, std::string password) {
    if (is_offline) {
        std::cout << "[-] Cannot register: System is offline.\n";
        return false;
    }
    
    std::string check_query = "SELECT id FROM users WHERE username='" + username + "'";
    mysql_query(db_connection, check_query.c_str());
    MYSQL_RES* result = mysql_store_result(db_connection);
    
    if (result && mysql_num_rows(result) > 0) {
        std::cout << "[-] Username already exists. Please choose a different one.\n";
        mysql_free_result(result);
        return false;
    }
    if (result) mysql_free_result(result);

    std::string insert_query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
    
    if (mysql_query(db_connection, insert_query.c_str()) == 0) {
        std::cout << "[+] Registration successful! You can now log in.\n";
        return true;
    } else {
        
        std::cout << "[-] Error registering user: " << mysql_error(db_connection) << "\n";
        return false;
    }
}


void FinanceSystem::addTransaction(double amount, std::string category, std::string desc, std::string type) {
    if (is_offline) { std::cout << "Cannot add: Offline Mode.\n"; return; }
    
    std::string query = "INSERT INTO transactions (user_id, amount, category, description, type, date) VALUES (" 
                        + std::to_string(active_user.id) + ", " + std::to_string(amount) + ", '" 
                        + category + "', '" + desc + "', '" + type + "', CURDATE())";
    
    if (mysql_query(db_connection, query.c_str()) == 0) {
        std::cout << "[+] Transaction recorded successfully!\n";
    } else {
        std::cout << "[-] Error recording transaction.\n";
    }
}

void FinanceSystem::updateTransaction(int transaction_id, double new_amt, std::string new_cat, std::string new_desc, std::string new_type) {
    if (is_offline) { std::cout << "Cannot update: Offline Mode.\n"; return; }
    

    std::string query = "UPDATE transactions SET amount=" + std::to_string(new_amt) + 
                        ", category='" + new_cat + "', description='" + new_desc + 
                        "', type='" + new_type + "' WHERE id=" + std::to_string(transaction_id) + 
                        " AND user_id=" + std::to_string(active_user.id);
    
    mysql_query(db_connection, query.c_str());
    
    if (mysql_affected_rows(db_connection) > 0) {
        std::cout << "[+] Transaction updated successfully!\n";
    } else {
        std::cout << "[-] Update failed. Transaction ID not found or doesn't belong to you.\n";
    }
}

void FinanceSystem::deleteTransaction(int transaction_id) {
    if (is_offline) return;
    std::string query = "DELETE FROM transactions WHERE id=" + std::to_string(transaction_id) + 
                        " AND user_id=" + std::to_string(active_user.id);
    mysql_query(db_connection, query.c_str());
    std::cout << "[+] Transaction removed (if it existed).\n";
}

void FinanceSystem::searchByCategory(std::string category) {
    if (is_offline) return;
    std::cout << "\n--- Searching for: " << category << " ---\n";
    std::string query = "SELECT id, date, description, type, amount FROM transactions WHERE category='" + category + 
                        "' AND user_id=" + std::to_string(active_user.id);
    
    mysql_query(db_connection, query.c_str());
    MYSQL_RES* result = mysql_store_result(db_connection);
    MYSQL_ROW row;
    
    while ((row = mysql_fetch_row(result))) {
        std::cout << "ID[" << row[0] << "] " << row[1] << " | " << row[2] << " (" << row[3] << "): " << row[4] << " ETB\n";
    }
    mysql_free_result(result);
}

void FinanceSystem::showBalance() {
    if (is_offline) return;
    std::string query = "SELECT SUM(CASE WHEN type='Income' THEN amount ELSE -amount END) FROM transactions WHERE user_id=" + std::to_string(active_user.id);
    mysql_query(db_connection, query.c_str());
    MYSQL_RES* result = mysql_store_result(db_connection);
    MYSQL_ROW row = mysql_fetch_row(result);
    
    std::cout << "\n>>> Total Available Balance: " << (row[0] ? row[0] : "0.00") << " ETB <<<\n";
    mysql_free_result(result);
}

void FinanceSystem::exportStatement() {
    std::ofstream file("BankStatement.txt");
    if (!file.is_open()) {
        std::cout << "[-] Error opening file.\n";
        return;
    }
    file << "====================================\n";
    file << "    PERSONAL FINANCE STATEMENT      \n";
    file << "====================================\n";
    file << "Account Holder: " << active_user.username << "\n\n";
    
    if (!is_offline) {
        std::string query = "SELECT date, type, category, amount FROM transactions WHERE user_id=" + std::to_string(active_user.id);
        mysql_query(db_connection, query.c_str());
        MYSQL_RES* result = mysql_store_result(db_connection);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            file << row[0] << " | " << row[1] << " | " << row[2] << " | " << row[3] << " ETB\n";
        }
        mysql_free_result(result);
    }
    file.close();
    std::cout << "[+] Statement saved to BankStatement.txt\n";
}