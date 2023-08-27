#include "Core_Function.cpp"

int main()
{
    string in = "..\\Input\\input.txt";
    string out = "..\\Output\\output.txt";

    // string in = "..\\Input\\custom_input.txt";
    // string out = "..\\Output\\custom_output.txt";

    OS NewOS(in,out);
    
    return 0;
}

