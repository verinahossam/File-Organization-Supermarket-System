
#include"Product.h"
#include"Customer.h"





void initFile(string filename)
{
    fstream ioFile;
    ioFile.open(filename, ios::out | ios::in | ios::binary);

    if (!ioFile)
    {
        ofstream out(filename, ios::binary | ios::out);
        short header = -1;
        out.write((char*)&header, sizeof(header));
        out.close();
    }
}


void customerMainQueries()
{
    cout << "1. Add New Customer To File.\n";
    cout << "2. Display All Customers.\n";
    cout << "3. Find Customer By Id.\n";
    cout << "4. Update Customer By Id.\n";
    cout << "5. Delete Customer By Id.\n";
    cout << "6. Find Customer By Name.\n";
    cout << "7. Update Customer By Name.\n";
    cout << "8. Delete Customer By Name.\n";
    cout << "9. Display Five Top Customers.\n";
    cout << "0. Exit.\n";
}

void CustomerFile()
{
    initFile("Customer.txt");

    fstream ioFile;
    ioFile.open("Customer.txt", ios::out | ios::in | ios::binary);
    

    int choice;
    Customer c;
    do
    {
        customerMainQueries();
        cout << "Enter Choice: ";
        cin >> choice;
        int id;
        char name[50];

        switch (choice)
        {
        case 1:
            c.readCustomer();
            addNewCustomerToFile(ioFile, c);
            break;
        case 2:
            displayAllCustomers(ioFile);
            break;
        case 3:
            cout << "Enter id to search: ";
            cin >> id;
            findCustomerById(ioFile, id);
            break;
        case 4:
            cout << "Enter id to update: ";
            cin >> id;
            updateCustomerById(ioFile, id);
            break;
        case 5:
            cout << "Enter id to delete: ";
            cin >> id;
            deleteCustomerById(ioFile, id);
            break;
        case 6:
            cout << "Enter name to search: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            findCustomerByname(ioFile, name);
            break;
        case 7:
            cout << "Enter name to update: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            updateCustomerByname(ioFile, name);
            break;
        case 8:
            cout << "Enter name to delete: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            deleteCustomerByname(ioFile, name);
            break;
        case 9:
            displayTopFiveCustomers(ioFile);
            break;
        case 0:
            cout << "Exit...\n";
            break;
        default:
            cout << "Error!\n";
        }
        if (choice != 0)
        {
            system("pause");
            system("cls");
        }
        ioFile.clear();
    } while (choice != 0);
  
    ioFile.close();
}

void productMainQueries()
{
    cout << "1. Add New Product To File.\n";
    cout << "2. Display All Products.\n";
    cout << "3. Find Product By Id.\n";
    cout << "4. Update Product By Id.\n";
    cout << "5. Delete Product By Id.\n";
    cout << "6. Find Product By Name.\n";
    cout << "7. Update Product By Name.\n";
    cout << "8. Delete Product By Name.\n";
    cout << "9. Display Top Five.\n";
    cout << "0. Exit.\n";
}

void ProductFile()
{
    initFile("Product.txt");
    fstream ioFile;
    ioFile.open("Product.txt", ios::out | ios::in | ios::binary);
   

    int choice;
    Product c;
    do
    {
        productMainQueries();
        cout << "Enter Choice: ";
        cin >> choice;
        int id;
        char name[50];

        switch (choice)
        {
        case 1:
            c.readProduct();
            addNewProductToFile(ioFile, c);
            break;
        case 2:
            displayAllProducts(ioFile);
            break;
        case 3:
            cout << "Enter id to search: ";
            cin >> id;
            findProductById(ioFile, id);
            break;
        case 4:
            cout << "Enter id to update: ";
            cin >> id;
            updateProductById(ioFile, id);
            break;
        case 5:
            cout << "Enter id to delete: ";
            cin >> id;
            deleteProductById(ioFile, id);
            break;
        case 6:
            cout << "Enter name to search: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            findProductByname(ioFile, name);
            break;
        case 7:
            cout << "Enter name to update: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            updateProductByname(ioFile, name);
            break;
        case 8:
            cout << "Enter name to delete: ";
            solveGetLineProblem();
            cin.getline(name, 50);
            deleteProductByname(ioFile, name);
            break;
        case 9:
            displayTopFiveProducts(ioFile);
            break;
        case 0:
            cout << "Exit...\n";
            break;
        default:
            cout << "Error!\n";
        }
        if (choice != 0)
        {
            system("pause");
            system("cls");
        }
        ioFile.clear();
    } while (choice != 0);
  
    ioFile.close();
}



void startProgram()
{
    readPrimaryIndexFile("ProductPrimaryIndex.txt", primaryIndexListProduct);
    readSecondaryIndexFile("ProductSecondaryIndex.txt", secondaryIndexListProduct);

    readPrimaryIndexFile("CustomerPrimaryIndex.txt", primaryIndexListCustomer);
    readSecondaryIndexFile("CustomerSecondaryIndex.txt", secondaryIndexListCustomer);

    int choice;
    do
    {

        cout << "1. Customer System.\n";
        cout << "2. Product System.\n";
        cout << "0. Exit.\n ";
        cout << "Enter Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            CustomerFile();
            break;
        case 2:
            ProductFile();
            break;
        case 0:
            cout << "Exit...\n";
            break;
        default:
            cout << "Error!\n";
        }
        if (choice != 0)
        {
            system("pause");
            system("cls");
        }

    } while (choice != 0);


    writePrimaryIndexFile("CustomerPrimaryIndex.txt", primaryIndexListCustomer);
    writeSecondaryIndexFile("CustomerSecondaryIndex.txt", secondaryIndexListCustomer);

    writePrimaryIndexFile("ProductPrimaryIndex.txt", primaryIndexListProduct);
    writeSecondaryIndexFile("ProductSecondaryIndex.txt", secondaryIndexListProduct);
}




int main()
{
	startProgram();
	return 0;
}


