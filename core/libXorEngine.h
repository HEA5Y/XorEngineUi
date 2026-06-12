#pragma once 
#include <cstdint>
#include <fstream>
#include <functional>
#include <QFile>
#include <QFileInfo>
#include <atomic>
#include <functional>
#include <QWaitCondition>
#include <QMutex>

class XorEngine
{
public:
    XorEngine();
    ~XorEngine();

    bool crypto(const char* inputFilePath, const char* outputFilePath, uint64_t mask,
                std::function<void(int)> progressCallback,
                std::atomic<bool>* isPaused,
                QMutex* pauseMutex,
                QWaitCondition* pauseCond);
};