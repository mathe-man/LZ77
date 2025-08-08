#include <LZ77.h>

#include <iostream>
#include <span>


std::vector<uint8_t> LZ77::Compress(std::vector<uint8_t> bytes, size_t search_buffer_size, size_t look_ahead_buffer_size)
{
    auto result = std::vector<Pattern>();

    for (size_t i = 0; i < bytes.size(); i++)
    {
        auto search_buffer = GetSearchBuffer(bytes, search_buffer_size, i);
        auto look_ahead_buffer = GetLookAheadBuffer(bytes, look_ahead_buffer_size, i);

        auto pattern = SearchPattern(search_buffer, look_ahead_buffer);
        result.push_back(pattern);
        i += pattern.length; // Jump the compressed elements
    }

    return ToBytes(result);
}

std::vector<uint8_t> LZ77::Decompress(std::vector<uint8_t> bytes)
{
    auto result = std::vector<uint8_t>();

    if (bytes.size() % 3 != 0)
        throw std::invalid_argument("Invalid length");

    for (size_t i = 0; i < bytes.size(); i += 3)
    {
        if (bytes[i] > 0 && bytes[i + 1] > 0)
        {
            // Check if the offset and length of the pattern is correct
            if (bytes[i] > result.size())
                throw std::invalid_argument(std::string("Invalid offset at index " + bytes[i]).c_str());
            if (bytes[i+1] > bytes[i])
                throw std::invalid_argument(std::string("Invalid length at index " + bytes[i]).c_str());


            for (int j = 0; j < bytes[i+1] ; j++)
                result.push_back(result[result.size() - bytes[i]]); // At each iteration result.size() is incremented by 1 so we don't have to add j to the index

        }
        // Add the next element of the pattern
        result.push_back(bytes[i+2]);
    }

    return result;
}


std::vector<uint8_t> LZ77::GetSearchBuffer(std::vector<uint8_t> bytes, size_t buffer_size, size_t index)
{
    auto result = std::vector<uint8_t>();

    if (buffer_size > index)    // Avoids going into negative index later, returning a smaller buffer
        buffer_size = index;

    // Set i to the beginning of the search buffer and iterate
    for (size_t i = index - buffer_size; i < index; i++)
        result.push_back(bytes[i]);

    return result;
}

std::vector<uint8_t> LZ77::GetLookAheadBuffer(std::vector<uint8_t> bytes, size_t buffer_size, size_t index)
{
    auto result = std::vector<uint8_t>();

    if (buffer_size + index - 1 > bytes.size()) // Avoids having an out of range index, returning a smaller buffer
        buffer_size = bytes.size() - index;

    for (auto i = index; i < index + buffer_size; i++)
        result.push_back(bytes[i]);

    return result;
}


Pattern LZ77::SearchPattern(const std::vector<uint8_t>& search_buffer, std::vector<uint8_t> look_ahead_buffer)
{
    auto view = std::span<uint8_t>(look_ahead_buffer);

    // Start the loop from the biggest size to find the biggest pattern
    for (size_t i = look_ahead_buffer.size() -1; i > 0; i--)
    {
        auto span = view.subspan(0, i);

        auto index_in_buffer = SearchInBuffer(search_buffer, std::vector(span.begin(), span.end()));

        if (index_in_buffer < search_buffer.size()) // Mean that an index as been found
        {
            auto distance_to_index = std::abs(int(search_buffer.size() - index_in_buffer));
            return { (uint8_t)distance_to_index, (uint8_t)span.size(),  look_ahead_buffer[i]};
        }
    }

    // This pattern is not present in the search buffer so we only add the next element
    return {0, 0, look_ahead_buffer[0]};
}

size_t LZ77::SearchInBuffer(std::vector<uint8_t> buffer, std::vector<uint8_t> search)
{
    for (size_t i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] == search[0])
        {
            auto equals = true;
            for (size_t j = 1; j < search.size(); j++)
                if (buffer[i + j] != search[j])
                    equals = false;
            if (equals)
                return i;
        }
    }

    return buffer.size();
}


std::vector<uint8_t> LZ77::ToBytes(const std::vector<Pattern>& patterns)
{
    auto result = std::vector<uint8_t>();

    for (auto pattern : patterns)
    {
        result.push_back(pattern.offset);
        result.push_back(pattern.length);
        result.push_back(pattern.next);
    }

    return result;
}
