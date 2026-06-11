#include "libXorEngine.h"

XorEngine::XorEngine(){

}
XorEngine::~XorEngine(){

}

bool XorEngine::crypto(const char* inputFilePath, const char* outputFilePath, uint64_t mask, std::function<void(int)> progressCallback)
{
    std::ifstream inputPath(inputFilePath, std::ios::binary | std::ios::ate);
    if (!inputPath.is_open()) return false;
    std::streamsize fileSize = inputPath.tellg();
    inputPath.seekg(0, std::ios::beg);
    QString tempPath = QString::fromUtf8(outputFilePath) + ".tmp";
    std::ofstream outputPath(tempPath.toStdString(), std::ios::binary);
    if (!outputPath.is_open()) return false;
    const size_t bufferSize = 65536;
    std::vector<char> buffer(bufferSize);
    std::streamsize totalBytesProcessed = 0;

    while (inputPath) {
        inputPath.read(buffer.data(), bufferSize);
        size_t bytesRead = inputPath.gcount();
        if (bytesRead == 0) break;
        size_t blocks64 = bytesRead / 8;
        uint64_t* ptr64 = reinterpret_cast<uint64_t*>(buffer.data());
        for (size_t i = 0; i < blocks64; ++i) {
            ptr64[i] ^= mask;
        }
        if (bytesRead > blocks64 * 8) {
            uint8_t maskBytes[8];
            memcpy(maskBytes, &mask, 8);
            for (size_t i = blocks64 * 8; i < bytesRead; ++i) {
                buffer[i] ^= maskBytes[(i - (blocks64 * 8)) % 8];
            }
        }
        outputPath.write(buffer.data(), bytesRead);
        totalBytesProcessed += bytesRead;
        if (progressCallback && fileSize > 0) {
            int percent = static_cast<int>((totalBytesProcessed * 100) / fileSize);
            progressCallback(percent);
        }
    }
    outputPath.close();
    inputPath.close();
    QFile finalFile(QString::fromUtf8(outputFilePath));
    QFile tempFile(tempPath);

    // Если файл существовал, удаляем его
    if (finalFile.exists()) {
        if (!finalFile.remove()) return false;
    }

    // Переименовываем временный в финальный
    return tempFile.rename(QString::fromUtf8(outputFilePath));
}