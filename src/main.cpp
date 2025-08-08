#include <iostream>
#include <vector>
#include <LZ77.h>

auto debug_bytes = std::vector<uint8_t>
{ 3, 1, 1, 4, 1, 1, 2, 7, 2, 2 };

int main()
{
    auto result = LZ77::Compress(debug_bytes, 4, 4);

    auto original = LZ77::Decompress(result);

    std::cout << (original ==  debug_bytes) << std::endl;
    // Expect a 1
    // Compression & Decompression works fines

    return 0;
}