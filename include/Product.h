#pragma once
#include"ProjectIndex.h"


vector<PrimaryIndex> primaryIndexListProduct;
vector<SecondaryIndex> secondaryIndexListProduct;

bool checkidProduct(int id)
{
    if (searchInPrimaryIndexList(id, primaryIndexListProduct) == -1)
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
class Product
{
public:
    int id;
    float price;
    char name[50];

    void readProduct(bool isupdate = false)
    {
        if (!isupdate)
        {
            do
            {
                cout << "Enter Product id: ";
                cin >> id;

            } while (checkidProduct(id));
        }
        do
        {
            cout << "Enter Product  name (Minimum 5 characters): ";
            solveGetLineProblem();
            cin.getline(name, 30);
            if (strlen(name) < 5)
            {
                cout << ">> Error: Name is too short! Please enter at least 5 characters.\n";
            }
        } while (strlen(name) < 5);

        cout << "Enter Product price: ";
        cin >> price;
    }

    void displayProduct()
    {
        cout << "Product Id       : " << id << endl;
        cout << "Product Name     : " << name << endl;
        cout << "Product price    : " << price << endl;
    }

    // id len name price
    short calcLength()
    {
        short fixedLen = sizeof(int) + sizeof(float); // id  , price
        short varLen = sizeof(name);
        short lengthIndicator = sizeof(short); // len
        return fixedLen + varLen + lengthIndicator;
    }
};

// write Product To File
void writeProductToFile(fstream& ioFile, const Product& c)
{
    ioFile.write((char*)&c.id, sizeof(c.id));

    short len = strlen(c.name);
    ioFile.write((char*)&len, sizeof(len));
    ioFile.write(c.name, len);

    ioFile.write((char*)&c.price, sizeof(c.price));
}

// read Product From File
void readProductFromFile(fstream& ioFile, Product& c)
{
    ioFile.read((char*)&c.id, sizeof(c.id));

    short len = 0;
    ioFile.read((char*)&len, sizeof(len));
    ioFile.read(c.name, len);
    c.name[len] = '\0';

    ioFile.read((char*)&c.price, sizeof(c.price));
}

// displayAll Products
void displayAllProducts(fstream& ioFile)
{
    Product c;
    cout << "====== Products ======\n";
    for (int i = 0; i < primaryIndexListProduct.size(); i++)
    {
        int offset = primaryIndexListProduct[i].offset;
        ioFile.seekg(offset);
        readProductFromFile(ioFile, c);
        c.displayProduct();
        cout << "\n---------------------------\n";
    }
}

void displayTopFiveProducts(fstream& ioFile)
{
    Product c;
    cout << "====== Top 5 Products ======\n";
    int NumOfProducts = (primaryIndexListProduct.size() < 5) ? primaryIndexListProduct.size() : 5;
    for (int i = 0; i < NumOfProducts; i++)
    {
        int offset = primaryIndexListProduct[i].offset;
        ioFile.seekg(offset);
        readProductFromFile(ioFile, c);
        c.displayProduct();
        cout << "\n---------------------------\n";
    }
}

void firstFitStrategy(fstream& ioFile, Product& s)
{
    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    if (header == -1)
    {
        ioFile.seekp(0, ios::end);
        PrimaryIndex pi = { s.id, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListProduct);

        SecondaryIndex se;
        se.id = s.id;
        strcpy_s(se.name, s.name);
        addToSecondaryIndexList(se, secondaryIndexListProduct);
        writeProductToFile(ioFile, s);
    }
    else
    {
        short currentPos = header;
        short prevPos = -1;
        short neededSize = s.calcLength();
        while (currentPos != -1)
        {
            // id len name[* H S]  price
            ioFile.seekg(currentPos + sizeof(int) + sizeof(short) + sizeof(char)); // skip id + len + *
            short nextDeleted, deletedSize;
            ioFile.read((char*)&nextDeleted, sizeof(nextDeleted));
            ioFile.read((char*)&deletedSize, sizeof(deletedSize));

            if (deletedSize >= neededSize)
            {
                ioFile.seekp(currentPos);
                writeProductToFile(ioFile, s);
                short padding = deletedSize - neededSize;
                while (padding--)
                    ioFile.put('#');

                PrimaryIndex pi = { s.id, (int)ioFile.tellp() };
                addToPrimaryIndexList(pi, primaryIndexListProduct);

                SecondaryIndex se;
                se.id = s.id;
                strcpy_s(se.name, s.name);
                addToSecondaryIndexList(se, secondaryIndexListProduct);

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
        PrimaryIndex pi = { s.id, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListProduct);

        SecondaryIndex se;
        se.id = s.id;
        strcpy_s(se.name, s.name);
        addToSecondaryIndexList(se, secondaryIndexListProduct);
        writeProductToFile(ioFile, s);
    }
}
void bestFitStrategy(fstream& ioFile, Product& s)
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
        PrimaryIndex pi = { s.id, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListProduct);

        SecondaryIndex se;
        se.id = s.id;
        strcpy_s(se.name, s.name);
        addToSecondaryIndexList(se, secondaryIndexListProduct);
        writeProductToFile(ioFile, s);
    }
    else
    {

        ioFile.seekg(bestPos + sizeof(int) + sizeof(short) + sizeof(char));
        short nextDeleted;
        ioFile.read((char*)&nextDeleted, sizeof(short));

        ioFile.seekp(bestPos);
        PrimaryIndex pi = { s.id, (int)ioFile.tellp() };
        addToPrimaryIndexList(pi, primaryIndexListProduct);

        SecondaryIndex se;
        se.id = s.id;
        strcpy_s(se.name, s.name);
        addToSecondaryIndexList(se, secondaryIndexListProduct);

        writeProductToFile(ioFile, s);
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
void addNewProductToFile(fstream& ioFile, Product& s)
{
    // id len name price
    //firstFitStrategy(ioFile, s);
    bestFitStrategy(ioFile, s);
    cout << "Product added successfully.\n";
}

// find Product By Id
void findProductById(fstream& ioFile, const int& id)
{
    Product c;
    int offset = searchInPrimaryIndexList(id, primaryIndexListProduct);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readProductFromFile(ioFile, c);
        cout << "Product Found.\n";
        c.displayProduct();
        cout << "\n--------------------------\n";
    }
}

// update Product Data by id
void updateProductById(fstream& ioFile, const int& id)
{

    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    Product s;
    short offset = searchInPrimaryIndexList(id, primaryIndexListProduct);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readProductFromFile(ioFile, s);

        cout << "Current Data::\n";
        s.displayProduct();
        cout << "\n--------------------------\n";
        char oldName[50];
        strcpy_s(oldName, s.name);
        short oldLen = s.calcLength();

        cout << "Edit Data::\n";
        s.readProduct(true);
        cout << "New Data::\n";
        s.displayProduct();
        cout << "\n--------------------------\n";
        short newLen = s.calcLength();

        if (oldLen >= newLen)
        {
            ioFile.seekp(offset);
            writeProductToFile(ioFile, s);
            short padding = oldLen - newLen;
            while (padding--)
                ioFile.put('#');

            deleteFromSecondaryIndexList(oldName, s.id, secondaryIndexListProduct);
            SecondaryIndex se;
            se.id = s.id;
            strcpy_s(se.name, s.name);
            addToSecondaryIndexList(se, secondaryIndexListProduct);
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
            deleteFromPrimaryIndexList(id, primaryIndexListProduct);
            deleteFromSecondaryIndexList(oldName, id, secondaryIndexListProduct);

            addNewProductToFile(ioFile, s);
            cout << "Product updated successfully.\n";
        }
    }
}

// delete Product By Id
void deleteProductById(fstream& ioFile, const int& id)
{
    ioFile.seekg(0);
    short header;
    ioFile.read((char*)&header, sizeof(header));

    Product s;
    short offset = searchInPrimaryIndexList(id, primaryIndexListProduct);
    if (offset == -1)
        cout << "Not Found.\n";
    else
    {
        ioFile.seekg(offset);
        readProductFromFile(ioFile, s);

        s.displayProduct();
        cout << "\n--------------------------\n";
        ioFile.seekp(offset + sizeof(int) + sizeof(short));

        ioFile.put('*');
        ioFile.write((char*)&header, sizeof(header));
        short deletedSize = s.calcLength();
        ioFile.write((char*)&deletedSize, sizeof(deletedSize));

        // update header
        ioFile.seekp(0);
        ioFile.write((char*)&offset, sizeof(offset));
        deleteFromPrimaryIndexList(id, primaryIndexListProduct);
        deleteFromSecondaryIndexList(s.name, id, secondaryIndexListProduct);
        cout << "Product deleted successfully.\n";
    }
}

// find Customer By name
void findProductByname(fstream& ioFile, char name[])
{
    Product c;
    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListProduct);
    if (IDs.size() == 0)
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findProductById(ioFile, IDs[i]);
        }
    }
}

// delete Product By name
void deleteProductByname(fstream& ioFile, char name[])
{
    Product c;
    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListProduct);
    if (IDs.empty())
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findProductById(ioFile, IDs[i]);
        }

        cout << "Enter id to delete: ";
        int id;
        cin >> id;
        deleteProductById(ioFile, id);
    }
}

void updateProductByname(fstream& ioFile, char name[])
{
    Product c;

    vector<int> IDs = searchInSecondaryIndexList(name, secondaryIndexListProduct);

    if (IDs.empty())
    {
        cout << "Not Found.\n";
        return;
    }
    else
    {
        for (int i = 0; i < IDs.size(); i++)
        {
            findProductById(ioFile, IDs[i]);
        }

        cout << "Enter id to update: ";
        int id;
        cin >> id;
        updateProductById(ioFile, id);
    }
}


