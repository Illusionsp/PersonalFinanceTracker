💰 Personal Finance Tracker (PFT)

A modular C++ console application that replaces traditional file handling with a MySQL Relational Database to provide secure, efficient, and reliable financial management.

🏗️ Academic Framework (Units 1–3)

This project demonstrates mastery of the SwEg2102 curriculum at Addis Ababa Science and Technology University.

Unit 1 — Modular Programming
Functional decomposition using independent modules (main, FinanceSystem, Helper) for maintainability and code reusability.
Unit 2 — Data Structures & OOP
Uses struct for heterogeneous data mapping and class encapsulation for secure system behavior and data abstraction.
Unit 3 — Persistence & File Management
Implements dual-layer persistence using:
MySQL for relational data storage
C++ fstream for generating sequential text-based bank statements (BankStatement.txt)
📂 Project Architecture
PersonalFinanceTracker/
│
├── main.cpp
├── Models.h
├── FinanceSystem.h
├── FinanceSystem.cpp
├── Helper.h
├── config.ini
├── Makefile
└── README.md
Core Components
main.cpp → Controls menu navigation and application flow
Models.h → Defines User and Transaction structures
FinanceSystem.* → Handles MySQL connectivity, CRUD operations, balance calculation, and file export
Helper.h → Input validation and utility functions
config.ini → Stores database credentials securely
Makefile → Automates compilation and links libmysqlclient
📋 Key Features
🔐 Secure Authentication
User login system with SQL-based credential validation
User-specific financial data isolation
💳 Transaction Management (CRUD)
Add, update, and delete transactions
Real-time MySQL synchronization
Formatted console table view for transaction history
📊 Financial Reporting
Dynamic balance calculation
Export financial reports using:
std::ofstream
Generates readable BankStatement.txt files
🛠️ Technical Stack
Component	Technology
Language	C++17
Database	MySQL 8.0+
Library	libmysqlclient
File Handling	C++ fstream
Build Tool	GNU Make
⚙️ Execution Guide
# Navigate to project directory
cd ~/PersonalFinanceTracker

# Build the application
make

# Run the application
./pft_app
🎯 Learning Outcomes

This project demonstrates practical understanding of:

Modular Programming
Object-Oriented Programming
MySQL Integration with C++
CRUD Operations
File Persistence using fstream
Build Automation with Makefiles