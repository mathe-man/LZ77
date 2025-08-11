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
void WriteToFile(const char* path, const std::vector<uint8_t>& data)
{
    std::ofstream file =std::ofstream(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open/create file");

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void output_result(std::vector<uint8_t> bytes, const char* out_path, size_t input_size, std::chrono::time_point<std::chrono::system_clock> start_time)
{

    // Write result
    WriteToFile(out_path, bytes);

    // Show more info
    std::cout
    << "Result of " << bytes.size() << " bytes.\n"
    << "Size ratio of " << LZ77::GetCompressionRatio(input_size, bytes.size())
    << " ( input/output ).\n"
    << "Operation ended in " << (std::chrono::high_resolution_clock::now() - start_time).count()
    << std::endl;

}


bool should_decompress(int argc, char** argv)
{
    if (argc > 3)
        if (argv[3] == "-d" || argv[3] == "--decompress")
            return true;

    return false;
}

int main(int argc, char** argv)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc < 3)
        { help(); return 0; }


    // Input
    std::vector<uint8_t> inputs;
    const char* out_path;

    inputs = ReadFromFile(argv[1]);
    out_path = argv[2];


    // Result
    std::vector<uint8_t> outputs;
    // Decompress / Compress
    if (should_decompress(argc, argv))
        outputs = LZ77::Decompress(inputs);
    else
        outputs = LZ77::Compress(inputs);

    output_result(outputs, out_path, inputs.size(), start_time);

    delete out_path;
}