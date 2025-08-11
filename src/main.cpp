#include <cstring>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <LZ77.h>

auto debug_bytes = std::vector<uint8_t>
{ 3, 1, 1, 4, 1, 1, 2, 7, 2, 2 };


void help()
{
    std::cout
    << "LZ77 Compression algorithm \n"
       "implemented in C++ by mathe-man (https://github.com/mathe-man)\n"
       "See https://github.com/mathe-man/LZ77 for more infos          \n"
       "==============================================================\n"
       "Usage:\n"
       "| LZ77 input\n"
       "| LZ77 -args input\n"
       "-------------------------------\n"
       "Args*:\n"
       "| h/H : show this message.\n"
       "| d/D : decompress the input**.\n"
       "| i/I : give more information about the result at the end.\n"
       "| *note: args not be seperated by spaces, see the Examples.\n"
       "| **note: when the d argument is not gave the input will be compressed.\n"
       "-------------------------------\n"
       "Examples:\n"
       "| LZ77 my_message\n"
       "| LZ77 -id my_compressed_message  # It will decompress and show infos.\n"
       "| LZ77 -h my_message              # It will only show this message without making any other operation\n"
       "-------------------------------\n"
       "Ask your questions and report issues on the github page"
       "https://github.com/mathe-man/LZ77\n"
    << std::endl;
}



std::vector<uint8_t> ToBytes(void* source, size_t size)
{
    auto input = std::vector<uint8_t>();

    for (int i = 0; i < size; i++)
        input.push_back(reinterpret_cast<uint8_t*>(source)[i]);

    return input;
}

std::vector<uint8_t> ReadFromFile(const char* path)
{
    if (!std::filesystem::exists(path))
        throw std::runtime_error("File not found");

    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open file");

    // Get file size
    auto size = std::filesystem::file_size(path);
    std::vector<uint8_t> result(size);

    // Read all bytes
    file.read(reinterpret_cast<char*>(result.data()), size);

    return result;
}
bool WriteToFile(const char* path, const std::vector<uint8_t>& data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open/create file");

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

bool contains(std::string* source, std::string key)
{
    if (!source)
        return false;

    for (auto c : *source)
        for (auto k : key)
            if (c == k)
                return true;
    return false;
}

void output_result(std::vector<uint8_t> bytes, size_t input_size, std::chrono::time_point<std::chrono::system_clock> start_time, std::string* args)
{
    for (auto byte : bytes)
        std::cout << byte;
    std::cout << std::endl;

    // Show more info
    if (contains(args, "iI")) {
        std::cout
        << "Result of " << bytes.size() << " bytes.\n"
        << "Size ratio of " << LZ77::GetCompressionRatio(input_size, bytes.size())
        << " ( input/output ).\n"
        << "Operation ended in " << (std::chrono::high_resolution_clock::now() - start_time).count()
        << std::endl;
    }
}


int main(int argc, char** argv)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc < 2)
        { help(); return 0; }

    std::string* args = nullptr;
    if (argv[1][0] == '-')
        args = new std::string(argv[1]);

    // Help, -h / -H
    if (contains(args, "hH"))
        { help(); return 0; }

    // TODO get inputs and give outputs using binary files, console(ASCII) characters don't show correctly the result, end up making error

    // Input
    std::vector<uint8_t> inputs;
    if (!args)  // If there is no arg the input is argv[1]
        inputs = ToBytes(argv[1], strlen(argv[1]));
    else if (argc < 3)  // If there is args but no input, throw an error
        throw std::invalid_argument("Missing required input after arguments");
    else                // If there is args, the input is argv[2]
        inputs = ToBytes(argv[2], strlen(argv[2]));


    // Result
    std::vector<uint8_t> outputs;

    // Decompress
    if (contains(args, "dD"))
        outputs = LZ77::Decompress(inputs);
    else
        outputs = LZ77::Compress(inputs);

    output_result(outputs, inputs.size(), start_time, args);

    delete args;
}