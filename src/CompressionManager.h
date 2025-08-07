#pragma once

#include <cstdint>
#include <vector>
#include <cstddef>


struct Pattern
{
    size_t offset;
    size_t length;
    uint8_t next;
};


class CompressionManager {
public:
    CompressionManager();
    ~CompressionManager();


    [[nodiscard]] std::vector<uint8_t> static Compress(std::vector<uint8_t> bytes, size_t search_buffer_size, size_t look_ahead_buffer_size);
    [[nodiscard]] std::vector<uint8_t> static Decompress(std::vector<uint8_t> bytes);

    [[nodiscard]] static float GetCompressionRatio(const std::vector<uint8_t>& base, const std::vector<uint8_t>& compressed) {
        return GetCompressionRatio(base.size(), compressed.size());
    }
    [[nodiscard]] static float GetCompressionRatio(const size_t& base_size, const size_t& compressed_size) {
        return static_cast<float>(base_size) / static_cast<float>(compressed_size);
    }


// private: // Remove private to use in debug
    [[nodiscard]] static std::vector<uint8_t> GetSearchBuffer(std::vector<uint8_t> bytes, size_t buffer_size, size_t index);
    [[nodiscard]] static std::vector<uint8_t> GetLookAheadBuffer(std::vector<uint8_t> bytes, size_t buffer_size, size_t index);
    [[nodiscard]] static Pattern SearchPattern(const std::vector<uint8_t>& search_buffer, std::vector<uint8_t> look_ahead_buffer);
    [[nodiscard]] static size_t SearchInBuffer(std::vector<uint8_t> buffer, std::vector<uint8_t> search);
};
