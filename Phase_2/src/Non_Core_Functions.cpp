#include "OS.hpp"

// Fumction to Initialize Core Data Strutures.
void OS::init()
{
    for (int i = 0; i < 300; i++)
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
    program_card_flag = true;
    C = false;
    framearray.clear();
    ptr_counter = 0;
    SI = 0;
    flag_Exe_end = true;
    count_program_cards = 0;
}

// Function to copy line to buffer.
void OS::copytobuffer()
{
    for (int i = 0; line[i] != '\0'; i++)
    {
        buffer[i] = line[i];
    }
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
    for (int i = 0; i < 300; i++)
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

// Function to store Data Memory
void OS::storedata()
{
    int k = 0;
    int loc = RA;
    for (int i = loc; i < loc + 10; i++)
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

void OS::extractjobdetails()
{
    pcb.JOBID = pcb.TTL = pcb.TLL = 0;
    for (int i = 0; i < 4; i++)
    {
        pcb.JOBID += (buffer[4 + i] - '0') * pow(10, 3 - i);
        pcb.TTL += (buffer[8 + i] - '0') * pow(10, 3 - i);
        pcb.TLL += (buffer[12 + i] - '0') * pow(10, 3 - i);
    }
}
void OS::init_page_table()
{
    int loc = PTR * 10;
    for (int i = loc; i < loc + 10; i++)
    {
        M[i][0] = '0';
        M[i][2] = '*';
        M[i][3] = '*';
    }
}

void OS::update_page_table_program()
{
    int num = ALLOCATE();
    Frame = num;
    RA = Frame * 10;
    int i = PTR * 10 + count_program_cards;
    M[i][0] = '1';
    M[i][3] = num % 10 + '0';
    num /= 10;
    M[i][2] = num + '0';

    count_program_cards++;
    return;
}

void OS::update_page_table_data()
{
    int num = ALLOCATE();
    Frame = num;
    RA = Frame * 10 + (VA % 10);
    int loc = PTR * 10 + (VA / 10);
    M[loc][0] = '1';
    M[loc][3] = num % 10 + '0';
    num /= 10;
    M[loc][2] = num + '0';
}
