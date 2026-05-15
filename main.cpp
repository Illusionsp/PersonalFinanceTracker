#include <iostream>
#include "FinanceSystem.h"
#include "Helper.h"

using namespace std;

int main() {
    FinanceSystem fs;
    fs.connectDB();
    fs.createTables();

    int choice;

    while (true) {
        cout << "\n========== WELCOME ==========\n";
        cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
        if (!(cin >> choice)) { clearInput(); continue; }

        if (choice == 3) break;
        if (choice == 1) { fs.registerUser(); continue; }

        if (choice == 2) {
            if (!fs.loginUser()) continue;

            bool loggedIn = true;
            while (loggedIn) {
                cout << "\n========== MAIN MENU ==========\n";
                cout << "1. Manage Transactions (Add/Update/Delete Transaction)\n";
                cout << "2. View & Filter History\n"; 
                cout << "3. Reports & Analytics\n";
                cout << "4. Budget Management\n";
                cout << "5. Log Out\n";
                cout << "Enter Choice: ";

                int mChoice;
                if (!(cin >> mChoice)) { clearInput(); continue; }

                switch (mChoice) {
                    case 1: { 
                        bool sub = true;
                        while(sub) {
                            cout << "\n--- MANAGE TRANSACTIONS ---\n";
                            cout << "1. Add Income\n2. Add Expense\n3. Update Record\n4. Delete Record\n5. Back\nChoice: ";
                            int sChoice;
                            if (!(cin >> sChoice)) { clearInput(); continue; }
                            switch(sChoice) {
                                case 1: fs.addIncome(); break;
                                case 2: fs.addExpense(); break;
                                case 3: fs.updateTransaction(); break; 
                                case 4: fs.deleteTransaction(); break; 
                                case 5: sub = false; break;
                            }
                        }
                        break;
                    }
                    case 2: { 
                        bool sub = true;
                        while(sub) {
                            cout << "\n--- VIEW & FILTER ---\n";
                            cout << "1. Full History\n2. Filter data by Category\n3. Filter data by Date\n4. Back\nChoice: ";
                            int sChoice;
                            if (!(cin >> sChoice)) { clearInput(); continue; }
                            
                            switch(sChoice) {
                                case 1: fs.viewTransactions(); break;
                                case 2: fs.filterByCategory(); break;
                                case 3: fs.filterByDate(); break;
                                case 4: sub = false; break;
                            }
                        }
                        break;
                    }
                    case 3: { 
                         bool sub = true;
                         while(sub) {
                             cout << "\n--- REPORTS & ANALYTICS ---\n";
                             cout << "1. Balance\n2. Category Report\n3. Monthly Report\n4. Back\nChoice: ";
                             int sChoice;
                             if (!(cin >> sChoice)) { clearInput(); continue; }
                             switch(sChoice) {
                                 case 1: fs.calculateBalance(); break;
                                 case 2: fs.categoryReport(); break;
                                 case 3: fs.monthlyReport(); break;
                                 case 4: sub = false; break;
                             }
                         }
                         break;
                    }
                    case 4: fs.setCategoryBudget(); break;
                    case 5: loggedIn = false; break;
                }
            }
        }
    }
    return 0;
}
