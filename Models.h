#ifndef MODELS_H
#define MODELS_H
#include <string>

struct Transaction {
    int id;
    double amount;
    std::string category;
    std::string type;
    std::string description;
    std::string date;
};

struct User {
    int id;
    std::string username;
    std::string password;
};

#endif