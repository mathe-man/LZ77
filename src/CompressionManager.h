#pragma once

#include <cstdint>
#include <vector>


class CompressionManager {
public:
    CompressionManager();
    ~CompressionManager();


    std::vector<uint8_t> Compress(std::vector<uint8_t> bytes) const;
    std::vector<uint8_t> Decompress(std::vector<uint8_t> bytes) const;

    float GetCompressionRatio(std::vector<uint8_t> base, std::vector<uint8_t> compressed) const{
        return GetCompressionRatio(base.size(), compressed.size());
    }
    float GetCompressionRatio(size_t base_size, size_t compressed_size) const{
        return static_cast<float>(base_size) / static_cast<float>(compressed_size);
    }
};
