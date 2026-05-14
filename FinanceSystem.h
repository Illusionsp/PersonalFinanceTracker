#ifndef FINANCESYSTEM_H
#define FINANCESYSTEM_H

#include <mysql/mysql.h>
#include <string>
#include "Models.h"

class FinanceSystem {
private:
    MYSQL* db_connection; 
    User active_user;     
    bool is_offline;      

public:
    FinanceSystem();
    ~FinanceSystem();
    

    bool login(std::string username, std::string password);
    bool registerUser(std::string username, std::string password);
    

    void addTransaction(double amount, std::string category, std::string desc, std::string type);
    void updateTransaction(int transaction_id, double new_amt, std::string new_cat, std::string new_desc, std::string new_type);
    void deleteTransaction(int transaction_id);
    void searchByCategory(std::string category);
    void showBalance();
    void exportStatement(); 
};

#endif