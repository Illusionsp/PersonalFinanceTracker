# 💰 Smart Personal Finance Tracker (C++/MySQL)

A high-performance financial management system designed for the project. It features real-time budget monitoring, transaction tracking, and automated budget threshold alerts.

---

# 🚀 Key Features

- 🔐 Secure Authentication: User registration and login system
- 📂 Relational Tracking: Maps income and expenses to categories
- 📊 Smart Budgeting: Real-time budget calculation with 75% and 100% alerts
- 📈 Analytical Reporting: Spending summaries and balance analysis
- ✅ Input Validation: Handles negative values and invalid input types

---

# 🏗️ System Architecture (Modular Design)

The project is divided into independent modules for clean Separation of Concerns.

| Member | File | Responsibility |
|---|---|---|
| Member 1 -Anael Mulugeta| `FinanceSystem.h` | Defines class structures and encapsulation |
| Member 2,3 and 4- Bethelhem Sefiw, Aschalew Getahun and Daniel Alemu | `FinanceSystem.cpp` | Business logic and MySQL queries |
| Member 5- Bontu Bekele| `main.cpp` | User interface and menu routing |
| Member 6- Bethelehem Hailu | `Helper.h` | Input validation and utility functions |
| Member 7- Dilayehu Dessalegn| `models.h` |Central Data Definition|

---

# 📊 Database Schema

The system uses a relational database model with the following tables:

- **Users** → Stores user credentials
- **Categories** → Links transactions to categories
- **Income** → Stores income transactions
- **Expenses** → Stores expense transactions
- **Budget** → Stores spending limits per category

---

# 🛠️ Installation & Setup

## MySQL Server

Ensure MySQL is running on:

```txt
127.0.0.1
