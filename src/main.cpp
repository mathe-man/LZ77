#include <iostream>
#include <vector>
#include "CompressionManager.h"

int main()
{
    std::cout << "Exemple ratio : \n"
    << CompressionManager::GetCompressionRatio(7, 5)
    << std::endl;

    return 0;
}