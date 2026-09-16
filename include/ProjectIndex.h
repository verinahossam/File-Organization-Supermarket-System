#pragma once
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void solveGetLineProblem()
{
    if (cin.peek() == '\n')
        cin.ignore(1, '\n');
}

// ================= PRIMARY INDEX =================

struct PrimaryIndex
{
    int id;
    int offset;

    // Marked as const
    bool operator<(const PrimaryIndex& pi) const
    {
        return id < pi.id;
    }
};

void writePrimaryIndexFile(const string& filename, const vector<PrimaryIndex>& primaryIndexList)
{
    ofstream outFile(filename, ios::binary | ios::out | ios::trunc);
    for (size_t i = 0; i < primaryIndexList.size(); i++)
    {
        PrimaryIndex pi = primaryIndexList[i];
        outFile.write(reinterpret_cast<const char*>(&pi), sizeof(PrimaryIndex));
    }
    cout << filename << " Primary Index saved successfully.\n";
    outFile.close();
}

void readPrimaryIndexFile(const string& filename, vector<PrimaryIndex>& primaryIndexList)
{
    ifstream inFile(filename, ios::binary | ios::in);
    if (!inFile)
        return;

    PrimaryIndex pi;

    while (inFile.read(reinterpret_cast<char*>(&pi), sizeof(pi)))
    {
        primaryIndexList.push_back(pi);
    }
    cout << filename << " Primary Index loaded successfully.\n";
    inFile.close();
}

void addToPrimaryIndexList(PrimaryIndex& pi, vector<PrimaryIndex>& primaryIndexList)
{
    primaryIndexList.push_back(pi);
    sort(primaryIndexList.begin(), primaryIndexList.end());
}

void deleteFromPrimaryIndexList(int id, vector<PrimaryIndex>& primaryIndexList)
{
    int low = 0;
    int high = primaryIndexList.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        PrimaryIndex pi = primaryIndexList[mid];

        if (pi.id == id)
        {
            primaryIndexList.erase(primaryIndexList.begin() + mid);
            return;
        }
        else if (pi.id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
}

int searchInPrimaryIndexList(int id, const vector<PrimaryIndex>& primaryIndexList)
{
    int low = 0;
    int high = primaryIndexList.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        PrimaryIndex pi = primaryIndexList[mid];

        if (pi.id == id)
            return pi.offset;
        else if (pi.id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}


// ================= SECONDARY INDEX =================

struct SecondaryIndex
{
    int id;
    char name[50];

    // Marked as const
    bool operator<(const SecondaryIndex& se) const
    {
        int cmp = strcmp(name, se.name);
        if (cmp == 0)
            return id < se.id;
        return cmp < 0;
    }
};

void writeSecondaryIndexFile(const string& filename, const vector<SecondaryIndex>& secondaryIndexList)
{
    ofstream outFile(filename, ios::binary | ios::out | ios::trunc);
    for (size_t i = 0; i < secondaryIndexList.size(); i++)
    {
        SecondaryIndex se = secondaryIndexList[i];
        outFile.write(reinterpret_cast<const char*>(&se), sizeof(SecondaryIndex));
    }
    cout << filename << "Secondary Index saved successfully.\n";
    outFile.close();
}

void readSecondaryIndexFile(const string& filename, vector<SecondaryIndex>& secondaryIndexList)
{
    ifstream inFile(filename, ios::binary | ios::in);
    if (!inFile)
        return;

    SecondaryIndex se;
    // Fix: Prevent reading duplicates on EOF
    while (inFile.read(reinterpret_cast<char*>(&se), sizeof(se)))
    {
        secondaryIndexList.push_back(se);
    }
    cout << filename << "Secondary Index loaded successfully.\n";
    inFile.close();
}

void addToSecondaryIndexList(const SecondaryIndex& se, vector<SecondaryIndex>& secondaryIndexList)
{
    secondaryIndexList.push_back(se);
    sort(secondaryIndexList.begin(), secondaryIndexList.end());
}

void deleteFromSecondaryIndexList(const char name[], int id, vector<SecondaryIndex>& secondaryIndexList)
{
    int low = 0;
    int high = secondaryIndexList.size() - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        const SecondaryIndex& se = secondaryIndexList[mid];

        int cmp = strcmp(se.name, name);

        if (cmp == 0)
        {

            if (se.id == id)
            {

                secondaryIndexList.erase(secondaryIndexList.begin() + mid);
                return;
            }
            else if (se.id < id)
                low = mid + 1;
            else
                high = mid - 1;
        }

        else if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }


}

vector<int> searchInSecondaryIndexList(const char name[], const vector<SecondaryIndex>& secondaryIndexList)
{
    int low = 0;
    int high = secondaryIndexList.size() - 1;
    vector<int> IDs;
    int found = -1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        SecondaryIndex se = secondaryIndexList[mid];

        int cmp = strcmp(se.name, name);
        if (cmp == 0)
        {
            found = mid;
            break;
        }
        else if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    if (found != -1)
    {
        // search left for duplicates
        int i = found;
        while (i >= 0 && strcmp(secondaryIndexList[i].name, name) == 0)
        {
            IDs.push_back(secondaryIndexList[i].id);
            i--;
        }
        // search right for duplicates
        i = found + 1;
        while (i < (int)secondaryIndexList.size() && strcmp(secondaryIndexList[i].name, name) == 0)
        {
            IDs.push_back(secondaryIndexList[i].id);
            i++;
        }
    }
    return IDs;
}





