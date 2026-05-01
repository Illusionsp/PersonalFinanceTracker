# 💰 Personal Finance Tracker (PFT)

A modular C++ console application that replaces traditional file-handling with a **MySQL Relational Database** to provide secure, efficient, and reliable financial management.

---

## 🏗️ Academic Framework (Units 1-3)
This project demonstrates mastery of the **SwEg2102** curriculum:
- **Unit 1 (Modular Programming):** Functional decomposition across independent modules (`main`, `helper`, `FinanceSystem`).
- **Unit 2 (Data Structures & OOP):** Use of `struct` for heterogeneous data mapping and `class` encapsulation for system behavior.
- **Unit 3 (Persistence):** SQL-based permanent storage, fulfilling the requirement for secondary storage management.

---

## 📋 Proposal-Based Features & Objectives
According to our project proposal, the following features have been implemented to solve the challenges of manual financial tracking:

### 1. User Account Management (Secure Access)
- **Login System:** Restricted access requiring a username and password to enter the tracker.
- **Data Privacy:** User data is isolated, ensuring individuals only see their own financial history.

### 2. Transaction Management (Core Logic)
- **Add Transactions:** Record income and expenses with detailed descriptions and dates.
- **Categorization:** Assign every transaction to a specific category (e.g., Food, Rent, Salary) for better organization.
- **Update & Delete:** Modify or remove past entries to maintain accurate records.

### 3. Real-Time Financial Analysis
- **Current Balance:** Dynamic calculation of total funds based on all recorded transactions.
- **Expense Tracking:** View total spending within specific categories or timeframes.
- **Search & Filter:** Quickly find past transactions without "wasting time" scanning manual logs.

---

## 👥 Team Roles & Responsibilities
| Member | Role | Key Contribution |
| :--- | :--- | :--- |
| **Member 1** | Project Manager | Main Menu System & UI Flow (Unit 1) |
| **Member 2** | Data Architect | Struct Definitions & Table Mapping (Unit 2) |
| **Member 3** | Database Lead | MySQL Connection & Config Management (Unit 3) |
| **Member 4** | Logic Developer | Transaction CRUD Operations (Add/Delete/Update) |
| **Member 5** | Analysis Expert | SQL-based Financial Reporting & Balance Math |
| **Member 6** | QA & Security | Secure Login & Input Sanitization |

---

## 🛠️ Technical Stack
- **Language:** C++17
- **Database:** MySQL 8.0+
- **API:** MariaDB C++ Connector
- **Persistence Logic:** Relational SQL Queries replacing Chapter 3 `fstream`.