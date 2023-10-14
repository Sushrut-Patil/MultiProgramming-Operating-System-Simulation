#include "Non_Core_Functions.cpp"

void OS::LOAD()
{
    while (finput.good())
    {
        clearbuffer();
        getline(finput, line);
        for (int i = 0; line[i] != '\0'; i++)
        {
            buffer[i] = line[i];
        }

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            printf("Program card detected\n");
            init();
            continue;
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            printf("Data card detected\n");
            flag = false;
            STARTEXE();
            continue;
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            printf("End card detected\n");
            continue;
        }
        if (flag)
        {
            address = memory_used;
            storedata();
            memory_used += 10;
        }
    }
}

void OS::STARTEXE()
{
    IC = 0;
    EXECUTE();
}

void OS::EXECUTE()
{
    while (1)
    {
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[IC][i];
        }
        extractaddress();
        IC++;

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            return;
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
            {
                R[i] = M[address][i];
            }
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
            {
                M[address][i] = R[i];
            }
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
            {
                C = true;
                if (R[i] != M[address][i])
                {
                    C = false;
                    break;
                }
            }
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            if (C)
            {
                IC = address;
            }
        }
    }
}

void OS::MOS()
{
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    default:
        break;
    }
}

void OS::READ()
{
    clearbuffer();
    getline(finput, line);
    for (int i = 0; line[i] != '\0'; i++)
    {
        buffer[i] = line[i];
    }
    storedata();
    EXECUTE();
}
void OS::WRITE()
{
    for (int i = address; i < address + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            foutput << M[i][j];
        }
    }
    foutput << endl;
    EXECUTE();
}
void OS::TERMINATE()
{
    printmemory();
    foutput << "\n\n"
            << flush;
    LOAD();
}
