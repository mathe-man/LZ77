#pragma once

#include <cstdint>
#include <vector>
#include <cstddef>


struct Pattern
{
    uint8_t offset;
    uint8_t length;
    uint8_t next;
};


class LZ77 {
public:
    [[nodiscard]] std::vector<uint8_t> static Compress(std::vector<uint8_t> bytes, size_t search_buffer_size = 0, size_t look_ahead_buffer_size = 0);
    [[nodiscard]] std::vector<uint8_t> static Decompress(std::vector<uint8_t> bytes);

    [[nodiscard]] static float GetCompressionRatio(const std::vector<uint8_t>& base, const std::vector<uint8_t>& compressed) {
        return GetCompressionRatio(base.size(), compressed.size());
    }
    [[nodiscard]] static float GetCompressionRatio(const size_t& base_size, const size_t& compressed_size) {
        return static_cast<float>(base_size) / static_cast<float>(compressed_size);
    }


//private:
    [[nodiscard]] static std::vector<uint8_t> GetSearchBuffer(std::vector<uint8_t> bytes, size_t index, size_t buffer_size = 0);
    [[nodiscard]] static std::vector<uint8_t> GetLookAheadBuffer(std::vector<uint8_t> bytes, size_t index, size_t buffer_size = 0);
    [[nodiscard]] static Pattern SearchPattern(const std::vector<uint8_t>& search_buffer, std::vector<uint8_t> look_ahead_buffer);
    [[nodiscard]] static size_t SearchInBuffer(std::vector<uint8_t> buffer, std::vector<uint8_t> search);

    [[nodiscard]] static std::vector<uint8_t> ToBytes(const std::vector<Pattern>& patterns);
};
