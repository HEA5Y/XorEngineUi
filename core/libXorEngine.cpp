#include "libXorEngine.h"
#include <fstream>
#include <vector>

XorEngine::XorEngine() {}

XorEngine::~XorEngine() {}

bool XorEngine::crypto(const char* inputFilePath, const char* outputFilePath, uint64_t mask)
{
    std::ifstream inputPath(inputFilePath, std::ios::binary);
    std::ofstream outputPath(outputFilePath, std::ios::binary);
    if(!outputPath.is_open() || !inputPath.is_open()) return false;

    const size_t bufferSize = 4096;
    std::vector<char> buffer(bufferSize);

    long long filesize = inputPath.seekg(0, std::ios::end).tellg();
    inputPath.seekg(0, std::ios::beg);


    while(inputPath){
        inputPath.read(buffer.data(), bufferSize);
        size_t bytesRead = inputPath.gcount();
        if (bytesRead == 0) break;

        size_t blocks64 = bytesRead / 8;
        size_t bytesDone = blocks64 * 8;

        uint64_t* ptr64 = reinterpret_cast<uint64_t*>(buffer.data());
        for (size_t i = 0; i < blocks64; ++i) {
            ptr64[i] ^= mask;
        }

        if (bytesDone < bytesRead) {
            uint8_t maskBytes[8];
            memcpy(maskBytes, &mask, 8);

            for (size_t i = bytesDone; i < bytesRead; ++i) {
                size_t maskIdx = (i - bytesDone) % 8; 
                buffer[i] ^= maskBytes[maskIdx];
            }
        }
       outputPath.write(buffer.data(), bytesRead); 
    }
    
    return true;
}
