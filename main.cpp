#include <iostream>
#include <limits>
#include "FinanceSystem.h"
#include "Helper.h"

int main() {
    FinanceSystem app;
    
    while (true) {
        Helper::clearScreen();
        Helper::printHeader("AASTU FINANCE TRACKER");
        std::cout << "1. Log In\n2. Register New Account\n3. Exit Application\nSelect Option: ";
        
        int auth_choice;
        if (!(std::cin >> auth_choice)) {
            std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        if (auth_choice == 3) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        }
        
        std::string username, password;
        
        if (auth_choice == 2) {
            Helper::printHeader("REGISTER NEW ACCOUNT");
            std::cout << "Choose a Username: "; std::cin >> username;
            std::cout << "Choose a Password: "; std::cin >> password;
            app.registerUser(username, password);
            
            std::cout << "\nPress Enter to return to main menu...";
            std::cin.ignore(); std::cin.get();
            continue; 
        }
        
        if (auth_choice == 1) {
            Helper::printHeader("ACCOUNT LOGIN");
            std::cout << "Username: "; std::cin >> username;
            std::cout << "Password: "; std::cin >> password;

            if (app.login(username, password)) {
                while (true) {
                    Helper::printHeader("MAIN MENU - Logged in as: " + username);
                    std::cout << "1. Manage Transactions (Add/Edit/Delete)\n2. View Balance\n3. Search by Category\n4. Export Statement\n5. Log Out\nSelect Option: ";
                    
                    int choice;
                    if (!(std::cin >> choice)) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (choice == 5) break; 
                    
                    if (choice == 1) {
                        Helper::printHeader("MANAGE TRANSACTIONS");
                        std::cout << "1. Add New\n2. Edit Existing\n3. Delete\n4. Back\nSelect Option: ";
                        int sub_choice;
                        if (!(std::cin >> sub_choice)) {
                            std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                        
                        if (sub_choice == 1) {
                            double amt; std::string cat, desc, type;
                            std::cout << "Amount (ETB): "; std::cin >> amt;
                            std::cout << "Type (Income/Expense): "; std::cin >> type;
                            std::cout << "Category (e.g., Food, Rent): "; std::cin >> cat;
                            std::cout << "Description: "; std::cin.ignore(); std::getline(std::cin, desc);
                            app.addTransaction(amt, cat, desc, type);
                        }
                        else if (sub_choice == 2) {
                            int id; double amt; std::string cat, desc, type;
                            std::cout << "(Hint: Use Option 3 from the Main Menu to search and find the ID first!)\n";
                            std::cout << "Enter Transaction ID to edit: "; std::cin >> id;
                            std::cout << "New Amount (ETB): "; std::cin >> amt;
                            std::cout << "New Type (Income/Expense): "; std::cin >> type;
                            std::cout << "New Category: "; std::cin >> cat;
                            std::cout << "New Description: "; std::cin.ignore(); std::getline(std::cin, desc);
                            app.updateTransaction(id, amt, cat, desc, type);
                        }
                        else if (sub_choice == 3) {
                            int id; 
                            std::cout << "(Hint: Use Option 3 from the Main Menu to search and find the ID first!)\n";
                            std::cout << "Enter Transaction ID to delete: "; std::cin >> id;
                            app.deleteTransaction(id);
                        }
                    }
                    else if (choice == 2) { app.showBalance(); }
                    else if (choice == 3) {
                        std::string cat; std::cout << "Enter Category: "; std::cin >> cat;
                        app.searchByCategory(cat);
                    }
                    else if (choice == 4) { app.exportStatement(); }
                    
                    std::cout << "\nPress Enter to continue...";
                    std::cin.ignore(); std::cin.get();
                }
            } else {
                std::cout << "[-] Login Failed! Incorrect credentials or system is offline.\n";
                std::cout << "\nPress Enter to try again...";
                std::cin.ignore(); std::cin.get();
            }
        }
    }
    return 0;
}