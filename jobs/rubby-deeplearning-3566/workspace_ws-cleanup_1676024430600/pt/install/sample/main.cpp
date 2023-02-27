#include <iostream>
#include "API/rubby/perception.h"

#define ERROR_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define WARNING_PRINT(x) std::cout << "" << (x) << "" << std::endl
#define INFO_PRINT(x) std::cout << "" << (x) << "" << std::endl

void Help()
{
    ERROR_PRINT("please input: ");
    INFO_PRINT("\tconfig dir: [string]");
    INFO_PRINT("\tinput image dir: [string]");
}

int main(int argc, char *argv[])
{
    PerceptionSimulator test;
    if (argc < 3)
    {
        Help();
        return 0;
    }
}
