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
| Member 1 | `FinanceSystem.h` | Defines class structures and encapsulation |
| Member 2 | `FinanceSystem.cpp` | Business logic and MySQL queries |
| Member 3 | `main.cpp` | User interface and menu routing |
| Member 4 | `schema.sql` | Database schema and relationships |
| Member 5 | `Helper.h` | Input validation and utility functions |

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
