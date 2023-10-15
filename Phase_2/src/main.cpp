#include "Core_Function.cpp"

int main()
{
    string in = "..\\Input\\test_input.txt";
    string out = "..\\Output\\test_output.txt";

    // string in = "..\\Input\\phase2_input.txt";
    // string out = "..\\Output\\phase2_output.txt";

    OS NewOS(in,out);
    NewOS.LOAD();
    
    return 0;
}

