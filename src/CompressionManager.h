#pragma once

#include <cstdint>
#include <vector>
#include <cstddef>

class CompressionManager {
public:
    CompressionManager();
    ~CompressionManager();


    [[nodiscard]] std::vector<uint8_t> Compress(std::vector<uint8_t> bytes) const;
    [[nodiscard]] std::vector<uint8_t> Decompress(std::vector<uint8_t> bytes) const;

    [[nodiscard]] static float GetCompressionRatio(const std::vector<uint8_t>& base, const std::vector<uint8_t>& compressed) {
        return GetCompressionRatio(base.size(), compressed.size());
    }
    [[nodiscard]] static float GetCompressionRatio(const size_t& base_size, const size_t& compressed_size) {
        return static_cast<float>(base_size) / static_cast<float>(compressed_size);
    }
};
