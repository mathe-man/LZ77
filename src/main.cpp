#include <iostream>
#include <vector>
#include "CompressionManager.h"

auto debug_bytes = std::vector<uint8_t>
{ 3, 1, 1, 4, 1, 1, 2, 7, 2, 2 };

int main()
{
    std::cout << "Exemple ratio : \n"
    << CompressionManager::GetCompressionRatio(7, 5)
    << std::endl;

    return 0;
}