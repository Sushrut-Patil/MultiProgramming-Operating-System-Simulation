#include "Non_Core_Functions.cpp"

void OS::LOAD()
{
    while (!finput.eof())
    {
        clearbuffer();
        getline(finput, line);
        copytobuffer();

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            printf("Control card detected\n");
            INITIALIZATION();
            PTR = ALLOCATE();
            init_page_table();
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
            update_page_table();
            storedata();
        }
    }
}

void OS::INITIALIZATION()
{
    TI = 0;
    SI = 3;
    VA = 0;
    PI = 0;
    LLC = 0;
    TTC = 0;
    init();
    extractjobdetails();
}

short OS::ALLOCATE()
{
    int frameno = rand() % 30;
    for (int i : ptrarray)
    {
        if (i == frameno)
        {
            return ALLOCATE();
        }
    }
    ptrarray.push_back(frameno);
    return frameno;
}
void OS::STARTEXE()
{
    IC = 0;
    EXECUTE();
    return;
}

void OS::EXECUTE()
{

    while (flag_Exe_end)
    {
        ADDRESSMAP_IC();
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[RA][i];
        }
        ADDRESSMAP_VA();
        IC++;
        if (PI != 0)
        {
            MOS();
            return;
        }

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
            {
                R[i] = M[RA][i];
            }
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
            {
                M[RA][i] = R[i];
            }
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            C = true;
            for (int i = 0; i < 4; i++)
            {
                if (R[i] != M[RA][i])
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
                IC = VA;
            }
        }
        else
        {
            PI = 1;
        }
        SIMULATION();
        if (SI != 0 || PI != 0 || TI != 0)
        {
            MOS();
        }
        
    }
}

void OS::ADDRESSMAP_IC()
{
    if (IC < 0 && IC > 99)
    {
        cout << "IC is not Valid\n";
        PI = 2;
        return;
    }
    if (IC % 10 == 0 && IC != 0)
    {
        ptr_counter++;
    }
    int ptr_add = PTR * 10 + ptr_counter;
    int frameno = (M[ptr_add][2] - '0') * 10 + (M[ptr_add][3] - '0');
    RA = frameno * 10 + IC % 10;
    return;
}

void OS::ADDRESSMAP_VA()
{
    if (IR[0] == 'H')
    {
        return;
    }
    if (IR[2] - '0' < 0 || IR[2] - '0' > 9 || IR[3] - '0' < 0 || IR[3] - '0' > 9)
    {
        PI = 2;
        return;
    }
    VA = (IR[2] - '0') * 10 + (IR[3] - '0');
    if (VA < 0 && VA > 99)
    {
        cout << "VA is not Valid\n";
        PI = 2;
        return;
    }
    if (IR[0] == 'G' && IR[1] == 'D')
    {
        int frameno = ALLOCATE();
        int mem_loc = (VA / 10) * 10;
        Datastoremap.insert(pair<int, int>(mem_loc, frameno));
        RA = frameno * 10;
    }
    else if (IR[0] == 'S' && IR[1] == 'R')
    {
        int mem_loc = (VA / 10) * 10;
        if (Datastoremap.find(mem_loc) == Datastoremap.end())
        {
            int frameno = ALLOCATE();
            Datastoremap.insert(pair<int, int>(mem_loc, frameno));
        }
        RA = Datastoremap[mem_loc] * 10 + (VA % 10);
    }
    else
    {
        int mem_loc = (VA / 10) * 10;
        if (Datastoremap.find(mem_loc) == Datastoremap.end())
        {
            cout << "Frame Not Assigned\n";
            PI = 3;
        }
        else
        {
            RA = Datastoremap[mem_loc] * 10 + (VA % 10);
        }
    }
    return;
}
void OS::MOS()
{
    if (TI == 0)
    {
        if (SI == 1)
        {
            READ();
        }
        else if (SI == 2)
        {
            WRITE();
        }
        else if (SI == 3)
        {
            TERMINATE(0);
        }
        else if (PI == 1)
        {
            TERMINATE(4);
        }
        else if (PI == 2)
        {
            TERMINATE(5);
        }
        else if (PI == 3)
        {
            /* If Page Fault Valid, ALLOCATE, update page Table,Adjust IC if necessary,EXECUTE USER PROGRAM OTHERWISE TERMINATE (6)*/
            if (IR[0] == 'G' && IR[1] == 'D' && IR[0] == 'S' && IR[1] == 'R')
            {
                update_page_table();
                IC--;
            }
            else
            {
                TERMINATE(6);
            }
        }
    }
    else if (TI == 2)
    {
        if (SI == 1)
        {
            TERMINATE(3);
        }
        else if (SI == 2)
        {
            WRITE();
            TERMINATE(3);
        }
        else if (SI == 3)
        {
            TERMINATE(0);
        }
        else if (PI == 1)
        {
            TERMINATE(3, 4);
        }
        else if (PI == 2)
        {
            TERMINATE(3, 5);
        }
        else if (PI == 3)
        {
            TERMINATE(3);
        }
        else
        {
            TERMINATE(3);
        }
        
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
    if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
    {
        cout << "OUT of Data\n";
        cout << "End card detected\n";
        TERMINATE(1);
    }
    storedata();
    SI = 0;
    return;
}

void OS::WRITE()
{
    if (LLC + 1 > pcb.TLL)
    {
        TERMINATE(2);
    }
    else
    {
        LLC++;
        for (int i = RA; i < RA + 10; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                foutput << M[i][j];
            }
        }
        foutput << endl;
        SI = 0;
    }

    return;
}

void OS::TERMINATE(short EM1)
{
    printmemory();
    foutput << "JOBID : " << pcb.JOBID << "\n"
            << ERRORMSGS[EM1] << "\n"
            << "IC : " << IC << "\n"
            << "IR : " << IR[0] << IR[1] << IR[2] << IR[3] << "\n"
            << "TTC : " << TTC << "\n"
            << "LLC : " << LLC << "\n\n\n"
            << flush;
    flag_Exe_end = false;
    return;
}

void OS::TERMINATE(short EM1, short EM2)
{
    printmemory();
    foutput << "JOBID : " << pcb.JOBID << "\n"
            << ERRORMSGS[EM1] << " & " << ERRORMSGS[EM2] << "\n"
            << "IC : " << IC << "\n"
            << "IR : " << IR[0] << IR[1] << IR[2] << IR[3] << "\n"
            << "TTC : " << TTC << "\n"
            << "LLC : " << LLC << "\n\n\n"
            << flush;
    flag_Exe_end = false;
    return;
}
void OS::SIMULATION()
{
    
    if (TTC + 1 > pcb.TTL)
    {
        TI = 2;
    }
    else
    {
        TTC++;
    }
    return;
}
