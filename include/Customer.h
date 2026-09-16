#pragma once
#include"ProjectIndex.h"

vector<PrimaryIndex> primaryIndexListCustomer;
vector<SecondaryIndex> secondaryIndexListCustomer;

bool checkid(int id)
{
    if (searchInPrimaryIndexList(id, primaryIndexListCustomer) == -1)
    {
        return false;
    }
    else
    {
        cout << "\n----------------------\n";
        cout << "already exists. Please enter a unique ID.\n\n";
        return true;
    }
}
class Customer
{
public:
    int CustomerId;
    float Age;
    char Fullname[50], phonenumbre[13];

    void readCustomer(bool isupdate = false)
    {
        if (!isupdate)
        {
            do
            {
                cout << "Enter Customer id: ";
                cin >> CustomerId;

            } while (checkid(CustomerId));
        }
        do
        {
            cout << "Enter Customer  name (Minimum 5 characters): ";
            solveGetLineProblem();
            cin.getline(Fullname, 30);
            if (strlen(Fullname) < 5)
            {
                cout << ">> Error: Name is too short! Please enter at least 5 characters.\n";
            }
        } while (strlen(Fullname) < 5);

        cout << "Enter Customer phone: ";
        solveGetLineProblem();
        cin.getline(phonenumbre, 13);

        cout << "Enter Customer age: ";
        cin >> Age;
    }

    void displayCustomer()
    {
        cout << "Customer Id       : " << CustomerId << endl;
        cout << "Customer Full Name: " << Fullname << endl;
        cout << "Customer Phone    : " << phonenumbre << endl;
        cout << "Customer Age      : " << Age << endl;
    }

    // id len fname len phone age
    short calcLength()
    {
        short fixedLen = sizeof(int) + sizeof(float); // id  , age
        short varLen = sizeof(Fullname) + sizeof(phonenumbre);
        short lengthIndicator = 2 * sizeof(short); // len
        return fixedLen + varLen + lengthIndicator;
    }
};

// write Customer To File
void writeCustomerToFile(fstream& ioFile, const Customer& c)
{
    ioFile.write((char*)&c.CustomerId, sizeof(c.CustomerId));

    short len = strlen(c.Fullname);
    ioFile.write((char*)&len, sizeof(len));
    ioFile.write(c.Fullname, len);

    len = strlen(c.phonenumbre);
    ioFile.write((char*)&len, sizeof(len));
    ioFile.write(c.phonenumbre, len);

    ioFile.write((char*)&c.Age, sizeof(c.Age));
}

// read Customer From File
void readCustomerFromFile(fstream& ioFile, Customer& c)
{
    ioFile.read((char*)&c.CustomerId, sizeof(c.CustomerId));

    short len;

    ioFile.read((char*)&len, sizeof(len));
    ioFile.read(c.Fullname, len);
    c.Fullname[len] = '\0';

    ioFile.read((char*)&len, sizeof(len));
    ioFile.read(c.phonenumbre, len);
    c.phonenumbre[len] = '\0';

    ioFile.read((char*)&c.Age, sizeof(c.Age));
}

// displayAll Customers
void displayAllCustomers(fstream& ioFile)
{
    Customer c;
    cout << "====== Customers ======\n";
    for (int i = 0; i < primaryIndexListCustomer.size(); i++)
    {
        int offset = primaryIndexListCustomer[i].offset;
        ioFile.seekg(offset);
        readCustomerFromFile(ioFile, c);
        c.displayCustomer();
        cout << "\n---------------------------\n";
    }
}

void displayTopFiveCustomers(fstream& ioFile)
{
    Customer c;
    cout << "====== Top 5 Customers ======\n";
    int NumOfCustomer = (primaryIndexListCustomer.size() < 5) ? primaryIndexListCustomer.size() : 5;
    for (int i = 0; i < NumOfCustomer; i++)
    {
        int offset = primaryIndexListCustomer[i].offset;
        ioFile.seekg(offset);
        readCustomerFromFile(ioFile, c);
        c.displayCustomer();
        cout << "\n---------------------------\n";
    }
}

void firstFitStrategy(fstream& ioFile, Customer& s)
{
    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    if (header == -1)
    {
        ioFile.seekp(0, ios::end);
        PrimaryIndex pi = { s.CustomerId, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListCustomer);

        SecondaryIndex se;
        se.id = s.CustomerId;
        strcpy_s(se.name, s.Fullname);
        addToSecondaryIndexList(se, secondaryIndexListCustomer);

        writeCustomerToFile(ioFile, s);
    }
    else
    {
        short currentPos = header;
        short prevPos = -1;
        short neededSize = s.calcLength();
        while (currentPos != -1)
        {
            // id len fname[* H S] len phone age
            ioFile.seekg(currentPos + sizeof(int) + sizeof(short) + sizeof(char)); // skip id + len + *
            short nextDeleted, deletedSize;
            ioFile.read((char*)&nextDeleted, sizeof(nextDeleted));
            ioFile.read((char*)&deletedSize, sizeof(deletedSize));

            if (deletedSize >= neededSize)
            {
                ioFile.seekp(currentPos);
                PrimaryIndex pi = { s.CustomerId, currentPos };
                addToPrimaryIndexList(pi, primaryIndexListCustomer);

                SecondaryIndex se;
                se.id = s.CustomerId;
                strcpy_s(se.name, s.Fullname);
                addToSecondaryIndexList(se, secondaryIndexListCustomer);

                writeCustomerToFile(ioFile, s);
                short padding = deletedSize - neededSize;
                while (padding--)
                    ioFile.put('#');

                if (prevPos == -1)
                {
                    ioFile.seekp(0);
                    ioFile.write((char*)&nextDeleted, sizeof(nextDeleted));
                }
                else
                {
                    ioFile.seekp(prevPos + sizeof(int) + sizeof(short) + sizeof(char));
                    ioFile.write((char*)&nextDeleted, sizeof(nextDeleted));
                }
                return;
            }
            prevPos = currentPos;
            currentPos = nextDeleted;
        }
        ioFile.seekp(0, ios::end);
        PrimaryIndex pi = { s.CustomerId, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListCustomer);

        SecondaryIndex se;
        se.id = s.CustomerId;
        strcpy_s(se.name, s.Fullname);
        addToSecondaryIndexList(se, secondaryIndexListCustomer);
        writeCustomerToFile(ioFile, s);
    }
}
void bestFitStrategy(fstream & ioFile, Customer & s)
{
    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    short neededSize = s.calcLength();
    short currentPos = header;
    short prevPos = -1;

    short bestPos = -1;
    short bestPrev = -1;
    short bestSize = 32767;

    while (currentPos != -1)
    {
        // id len fname[* H S] len phone age
        ioFile.seekg(currentPos + sizeof(int) + sizeof(short) + sizeof(char));// skip id + len + '*'
        short nextDeleted;
        ioFile.read((char*)&nextDeleted, sizeof(nextDeleted));
        short deletedSize;
        ioFile.read((char*)&deletedSize, sizeof(deletedSize));

        if (deletedSize >= neededSize && deletedSize < bestSize)
        {
            bestSize = deletedSize;
            bestPos = currentPos;
            bestPrev = prevPos;
        }
        prevPos = currentPos;
        currentPos = nextDeleted;
    }

    if (bestPos == -1)
    {
        ioFile.seekp(0, ios::end);
        PrimaryIndex pi = { s.CustomerId,(int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListCustomer);
        SecondaryIndex si;
        si.id = s.CustomerId;
        strcpy_s(si.name, s.Fullname);
        addToSecondaryIndexList(si, secondaryIndexListCustomer);
        writeCustomerToFile(ioFile, s);
    }
    else
    {

        ioFile.seekg(bestPos + sizeof(int) + sizeof(short) + sizeof(char));
        short nextDeleted;
        ioFile.read((char*)&nextDeleted, sizeof(short));

        ioFile.seekp(bestPos);
        PrimaryIndex pi = { s.CustomerId,(int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListCustomer);
       
        SecondaryIndex si;
        si.id = s.CustomerId;
        strcpy_s(si.name, s.Fullname);
        addToSecondaryIndexList(si, secondaryIndexListCustomer);

        writeCustomerToFile(ioFile, s);
        short padding = bestSize - neededSize;
        while (padding--)ioFile.put('#');

        if (bestPrev == -1)
        {
            ioFile.seekp(0);
            ioFile.write((char*)&nextDeleted, sizeof(short));
        }
        else
        {
            ioFile.seekp(bestPrev + sizeof(int) + sizeof(short) + sizeof(char));
            ioFile.write((char*)&nextDeleted, sizeof(short));
        }
    }
}
// Add with reclaiming space
void addNewCustomerToFile(fstream& ioFile, Customer& s)
{
    // id len name len phonenumbre age
    // firstFitStrategy(ioFile, s);
    bestFitStrategy(ioFile, s);
    cout << "Customer added successfully.\n";
}

// find Customer By Id
void findCustomerById(fstream& ioFile, const int& id)
{
    Customer c;
    int offset = searchInPrimaryIndexList(id, primaryIndexListCustomer);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readCustomerFromFile(ioFile, c);
        cout << "Customer Found.\n";
        c.displayCustomer();
        cout << "\n--------------------------\n";
    }
}

// update Customer Data by id
void updateCustomerById(fstream& ioFile, const int& id)
{

    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    Customer s;
    short offset = searchInPrimaryIndexList(id, primaryIndexListCustomer);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readCustomerFromFile(ioFile, s);

        cout << "Current Data::\n";
        s.displayCustomer();
        cout << "\n--------------------------\n";
        short oldLen = s.calcLength();
        char oldName[50];
        strcpy_s(oldName, s.Fullname);
        cout << "Edit Data::\n";
        s.readCustomer(true);
        cout << "New Data::\n";
        s.displayCustomer();
        cout << "\n--------------------------\n";
        short newLen = s.calcLength();

        if (oldLen >= newLen)
        {
            ioFile.seekp(offset);
            writeCustomerToFile(ioFile, s);
            short padding = oldLen - newLen;
            while (padding--)
                ioFile.put('#');

            deleteFromSecondaryIndexList(oldName, s.CustomerId, secondaryIndexListCustomer);
            SecondaryIndex se;
            se.id = s.CustomerId;
            strcpy_s(se.name, s.Fullname);
            addToSecondaryIndexList(se, secondaryIndexListCustomer);
        }
        else
        {
            ioFile.seekp(offset + sizeof(int) + sizeof(short));
            ioFile.put('*');
            ioFile.write((char*)&header, sizeof(header));
            ioFile.write((char*)&oldLen, sizeof(oldLen));

            // update header
            ioFile.seekp(0);
            ioFile.write((char*)&offset, sizeof(offset));
            deleteFromPrimaryIndexList(id, primaryIndexListCustomer);
            deleteFromSecondaryIndexList(oldName, id, secondaryIndexListCustomer);

            addNewCustomerToFile(ioFile, s);
            cout << "Customer updated successfully.\n";
        }
    }
}

// delete Customer By Id
void deleteCustomerById(fstream& ioFile, const int& id)
{
    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    Customer s;
    short offset = searchInPrimaryIndexList(id, primaryIndexListCustomer);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readCustomerFromFile(ioFile, s);

        s.displayCustomer();
        cout << "\n--------------------------\n";
        ioFile.seekp(offset + sizeof(int) + sizeof(short));

        ioFile.put('*');
        ioFile.write((char*)&header, sizeof(header));
        short deletedSize = s.calcLength();
        ioFile.write((char*)&deletedSize, sizeof(deletedSize));

        // update header
        ioFile.seekp(0);
        ioFile.write((char*)&offset, sizeof(offset));
        deleteFromPrimaryIndexList(id, primaryIndexListCustomer);
        deleteFromSecondaryIndexList(s.Fullname, id, secondaryIndexListCustomer);
        cout << "Customer deleted successfully.\n";
    }
}

// find Customer By name
void findCustomerByname(fstream& ioFile, char name[])
{
    Customer c;
    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListCustomer);
    if (IDs.size() == 0)
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findCustomerById(ioFile, IDs[i]);
        }
    }
}

// delete Customer By name
void deleteCustomerByname(fstream& ioFile, char name[])
{
    Customer c;
    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListCustomer);
    if (IDs.empty())
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findCustomerById(ioFile, IDs[i]);
        }

        cout << "Enter id to delete: ";
        int id;
        cin >> id;
        deleteCustomerById(ioFile, id);
    }
}

void updateCustomerByname(fstream& ioFile, char name[])
{
    Customer c;

    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListCustomer);

    if (IDs.empty())
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findCustomerById(ioFile, IDs[i]);
        }

        cout << "Enter id to update: ";
        int id;
        cin >> id;
        updateCustomerById(ioFile, id);
    }
}



