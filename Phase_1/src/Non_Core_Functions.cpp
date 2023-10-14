#include "OS.hpp"

// Fumction to Initialize Core Data Strutures.
void OS::init()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }
    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }
    memory_used = 0;
    flag = true;
    C = false;
    IC = 0;
    SI = 3;
}

// Function to clear buffer.
void OS::clearbuffer()
{
    for (int i = 0; i < 40; i++)
    {
        buffer[i] = ' ';
    }
}

// Function to Print Memory to Terminal
void OS::printmemory()
{
    for (int i = 0; i < 100; i++)
    {
        cout << "M[" << i << "] ";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << "\n";
    }
    cout << endl;
}

// Function to Extract Operand
void OS::extractaddress()
{
    address = (IR[2] - '0') * 10 + IR[3] - '0';
}

// Function to store Data Memory
void OS::storedata()
{
    int k = 0;
    for (int i = address; i < address + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = buffer[k];
            k++;
        }
        if (buffer[k] == '\n')
        {
            break;
        }
    }
}