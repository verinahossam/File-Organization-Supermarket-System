# 🛒 File Organization Supermarket System

A C++ console-based Supermarket Management System that demonstrates file organization, indexing, record management, and free-space management using binary files.

The system manages two types of records:

- Customers
- Products

---

## 📌 Table of Contents

- [Overview](#-overview)
- [Main Menu](#-main-menu)
- [Customer System](#-customer-system)
- [Product System](#-product-system)
- [File Storage](#-file-storage)
- [Record Structure](#-record-structure)
- [Primary Index](#-primary-index)
- [Secondary Index](#-secondary-index)
- [Searching](#-searching)
- [CRUD Operations](#-crud-operations)
- [Free Space Management](#-free-space-management)
- [First Fit](#-first-fit)
- [Best Fit](#-best-fit)
- [Current Strategy](#-current-strategy)
- [File Initialization](#-file-initialization)
- [Index Persistence](#-index-persistence)
- [Example](#-example)
- [How to Run](#-how-to-run)
- [Files Used](#-files-used)
- [Technologies](#-technologies)
- [Learning Outcomes](#-learning-outcomes)

---

## 📖 Overview

The **File Organization Supermarket System** is a C++ console application for managing customer and product records.

The project applies file organization concepts to a practical supermarket scenario. Records are stored in binary files, while Primary and Secondary Indexes are used to locate records efficiently.

The system also manages deleted spaces inside the data files and supports two free-space allocation strategies:

- First Fit
- Best Fit

---

## 🏠 Main Menu

When the program starts, the user can choose between the two available systems:

```text
1. Customer System.
2. Product System.
0. Exit.
Enter Choice:
```

---

# 👤 Customer System

The Customer System manages customer records.

### Customer Data

Each customer contains:

```text
Customer ID
Full Name
Phone Number
Age
```

### Customer Menu

```text
1. Add New Customer To File.
2. Display All Customers.
3. Find Customer By Id.
4. Update Customer By Id.
5. Delete Customer By Id.
6. Find Customer By Name.
7. Update Customer By Name.
8. Delete Customer By Name.
9. Display Five Top Customers.
0. Exit.
```

### Customer Operations

- Add a new customer.
- Display all customers.
- Find a customer by ID.
- Update a customer by ID.
- Delete a customer by ID.
- Find customers by name.
- Update a customer by name.
- Delete a customer by name.
- Display up to five customers.

The customer ID must be unique when adding a new customer.

The customer name must contain at least 5 characters.

---

# 📦 Product System

The Product System manages product records.

### Product Data

Each product contains:

```text
Product ID
Product Name
Product Price
```

### Product Menu

```text
1. Add New Product To File.
2. Display All Products.
3. Find Product By Id.
4. Update Product By Id.
5. Delete Product By Id.
6. Find Product By Name.
7. Update Product By Name.
8. Delete Product By Name.
9. Display Top Five.
0. Exit.
```

### Product Operations

- Add a new product.
- Display all products.
- Find a product by ID.
- Update a product by ID.
- Delete a product by ID.
- Find products by name.
- Update a product by name.
- Delete a product by name.
- Display up to five products.

The product ID must be unique when adding a new product.

The product name must contain at least 5 characters.

---

# 💾 File Storage

The system uses binary file operations for storing records.

The main data files are:

```text
Customer.txt
Product.txt
```

Even though these files have a `.txt` extension, the program opens them using binary mode.

The records are stored using file read/write operations, and file offsets are used to locate records.

---

# 📏 Record Structure

The Customer and Product records contain variable-length fields.

## Customer

The customer record contains:

```text
ID
Name Length
Name
Phone Length
Phone
Age
```

The name and phone number are stored together with their length indicators.

The `calcLength()` function calculates the record size used by the free-space management logic.

## Product

The product record contains:

```text
ID
Name Length
Name
Price
```

The product name is stored together with its length indicator.

The `calcLength()` function calculates the record size used by the free-space management logic.

---

# 🔑 Primary Index

The system uses a Primary Index based on the record ID.

The `PrimaryIndex` structure contains:

```text
id
offset
```

The ID identifies the record, while the offset points to the record's location in the data file.

Conceptually:

```text
ID
 ↓
Primary Index
 ↓
File Offset
 ↓
Record in Data File
```

The Primary Index list is kept sorted by ID.

When searching for an ID, the system uses **binary search** on the Primary Index.

---

# 🔤 Secondary Index

The system also uses a Secondary Index for searching by name.

The `SecondaryIndex` structure contains:

```text
id
name
```

The Secondary Index list is sorted by name, and by ID when two records have the same name.

Conceptually:

```text
Name
 ↓
Secondary Index
 ↓
ID
 ↓
Primary Index
 ↓
File Offset
 ↓
Record
```

This also allows multiple records with the same name to be found.

---

# 🔍 Searching

## Search By ID

The system searches the Primary Index using binary search.

```text
Enter ID
   ↓
Primary Index
   ↓
Binary Search
   ↓
Get File Offset
   ↓
Read Record
   ↓
Display Record
```

## Search By Name

The system searches the Secondary Index using binary search.

If multiple records have the same name, the system searches to the left and right of the found position to collect all matching IDs.

```text
Enter Name
   ↓
Secondary Index
   ↓
Find Matching ID(s)
   ↓
Primary Index
   ↓
Get File Offset
   ↓
Read Record(s)
```

---

# 🔄 CRUD Operations

The system supports the main record-management operations.

| Operation | Description |
|---|---|
| Create | Add a new customer or product |
| Read | Display or search records |
| Update | Modify an existing record |
| Delete | Delete an existing record |

---

# ➕ Create

When a new customer or product is added:

1. The user enters the record data.
2. The system checks that the ID is unique.
3. The system calculates the required record size.
4. The system searches for reusable deleted space.
5. The selected free-space strategy is applied.
6. The record is written to the data file.
7. The Primary Index is updated.
8. The Secondary Index is updated.

---

# 👀 Read

The system supports reading records by:

- Displaying all records.
- Searching by ID.
- Searching by name.
- Displaying up to five records.

The indexes are used to locate the records.

---

# ✏️ Update

When a record is updated, the system compares the old record size with the new record size.

### If the new record fits

If:

```text
New Size <= Old Size
```

the new record is written in the existing location, and the remaining space is filled with padding.

### If the new record does not fit

If:

```text
New Size > Old Size
```

the old record is marked as deleted.

Its space becomes available for reuse, and the new record is inserted using the selected free-space strategy.

The corresponding indexes are also updated.

---

# 🗑️ Delete

When a record is deleted, the system:

1. Locates the record using the Primary Index.
2. Marks the record as deleted.
3. Stores its size.
4. Links the deleted space to the free-space list.
5. Updates the file header.
6. Removes the record from the Primary Index.
7. Removes the record from the Secondary Index.

The deleted space can then be reused by a future insertion.

---

# ♻️ Free Space Management

The system maintains a list of deleted spaces inside the data file.

The file header points to the first deleted space.

Each deleted space stores information about:

- The next deleted space.
- The size of the deleted space.

Conceptually:

```text
File Header
     ↓
Deleted Space 1
     ↓
Deleted Space 2
     ↓
Deleted Space 3
     ↓
    -1
```

When a new record is inserted, the system checks this list before adding the record to the end of the file.

---

# 🚀 First Fit

The project implements a **First Fit** strategy.

First Fit scans the available deleted spaces in their current order and selects the **first space large enough** for the new record.

### Example

Available spaces:

```text
100 bytes
300 bytes
250 bytes
500 bytes
```

Required record size:

```text
220 bytes
```

First Fit selects:

```text
300 bytes
```

because it is the first space that can hold 220 bytes.

The unused part of the selected space is filled with padding.

---

# 🎯 Best Fit

The project also implements a **Best Fit** strategy.

Best Fit checks the available deleted spaces and selects the **smallest space that is large enough** for the new record.

### Example

Available spaces:

```text
100 bytes
300 bytes
250 bytes
500 bytes
```

Required record size:

```text
220 bytes
```

Best Fit selects:

```text
250 bytes
```

because it is the smallest space that can hold 220 bytes.

The unused part of the selected space is filled with padding.

---

# 🔁 First Fit vs Best Fit

| Strategy | How it chooses a free space |
|---|---|
| First Fit | First suitable space |
| Best Fit | Smallest suitable space |

For example:

```text
Available:
100, 300, 250, 500

Required:
220
```

The result is:

```text
First Fit → 300
Best Fit  → 250
```

Both strategies are implemented in the project.

---

# 📌 Current Strategy

Both Customer and Product implementations contain:

```cpp
firstFitStrategy(...)
bestFitStrategy(...)
```

However, the current `addNewCustomerToFile()` and `addNewProductToFile()` functions use:

```cpp
bestFitStrategy(...)
```

while the First Fit calls are commented out.

Therefore, **Best Fit is the currently active insertion strategy**, while First Fit remains implemented and can be selected by changing the function call.

---

# ⚙️ File Initialization

The project contains an `initFile()` function in `main.cpp`.

It is responsible for checking whether a data file exists.

For example:

```cpp
initFile("Customer.txt");
```

and:

```cpp
initFile("Product.txt");
```

If the file does not exist, it is created and initialized with the required header value.

This means the user does not need to manually create the Customer and Product data files before running the program.

---

# 💽 Index Persistence

The system stores the indexes in separate files.

When the program starts, it loads:

```text
ProductPrimaryIndex.txt
ProductSecondaryIndex.txt

CustomerPrimaryIndex.txt
CustomerSecondaryIndex.txt
```

When the program exits, the updated index lists are written back to these files.

This allows the index information to be preserved between program executions.

---

# 🧪 Example

## 1. Start the Program

```text
1. Customer System.
2. Product System.
0. Exit.
Enter Choice:
```

Choose:

```text
1
```

---

## 2. Add a Customer

Choose:

```text
1. Add New Customer To File.
```

Example input:

```text
Enter Customer id: 101
Enter Customer name (Minimum 5 characters): Ahmed Ali
Enter Customer phone: 01012345678
Enter Customer age: 25
```

The customer is added successfully.

---

## 3. Find the Customer

Choose:

```text
3. Find Customer By Id.
```

Enter:

```text
101
```

The system searches the Primary Index, obtains the file offset, reads the record, and displays the customer.

---

## 4. Add a Product

Return to the main menu and choose:

```text
2. Product System.
```

Then choose:

```text
1. Add New Product To File.
```

Example:

```text
Enter Product id: 201
Enter Product name (Minimum 5 characters): Milk
Enter Product price: 35
```

The product is stored and its indexes are updated.

---

## 5. Delete a Record

If a customer or product is deleted, its space becomes part of the available free-space list.

For example:

```text
Deleted Spaces:
100 bytes
300 bytes
250 bytes
```

If a new record needs:

```text
220 bytes
```

and Best Fit is active:

```text
Best Fit → 250 bytes
```

The system reuses that space instead of automatically appending the record to the end of the file.

---

# ▶️ How to Run

## Requirements

A C++ compiler and development environment are required.

Examples:

- Visual Studio
- Visual Studio Code
- Code::Blocks

## Steps

1. Clone or download the repository.
2. Open the project in your C++ development environment.
3. Make sure all project `.cpp` and `.h` files are included.
4. Compile the project.
5. Run the program.
6. Choose Customer System or Product System.
7. Select the required operation.

The required data files are created automatically when needed.

---

# 📂 Files Used

The project works with the following files:

```text
Customer.txt
Product.txt

CustomerPrimaryIndex.txt
CustomerSecondaryIndex.txt

ProductPrimaryIndex.txt
ProductSecondaryIndex.txt
```

### Data Files

```text
Customer.txt
Product.txt
```

Contain the stored customer and product records.

### Primary Index Files

```text
CustomerPrimaryIndex.txt
ProductPrimaryIndex.txt
```

Store the Primary Index records.

### Secondary Index Files

```text
CustomerSecondaryIndex.txt
ProductSecondaryIndex.txt
```

Store the Secondary Index records.

The index files are written in binary mode by the program.

---

# 🛠️ Technologies

## Language

- C++

## Concepts Used

- File Organization
- File Handling
- Binary File Handling
- Variable-Length Records
- File Offsets
- Primary Indexing
- Secondary Indexing
- Binary Search
- CRUD Operations
- Free Space Management
- First Fit
- Best Fit

---

# 🎓 Learning Outcomes

Through this project, the following concepts are applied practically:

- Managing records using C++ files.
- Working with binary file storage.
- Handling variable-length records.
- Calculating record sizes.
- Using file offsets.
- Implementing Primary Indexing.
- Implementing Secondary Indexing.
- Searching using binary search.
- Managing customer and product records.
- Implementing Create, Read, Update, and Delete operations.
- Managing deleted spaces.
- Reusing available file space.
- Implementing First Fit.
- Implementing Best Fit.
- Maintaining indexes while records are added, updated, or deleted.

---

# 📌 Conclusion

The **File Organization Supermarket System** is a C++ application that combines supermarket record management with file organization concepts.

It demonstrates how customer and product records can be stored, indexed, searched, updated, deleted, and inserted into reusable free spaces using **First Fit** and **Best Fit** strategies.
