#pragma once 
#include <cstdint>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport) 
#endif

class DLL_EXPORT XorEngine
{
public:
    XorEngine();
    ~XorEngine();

    bool crypto(const char* inputFilePath, const char* outputFilePath, uint64_t mask);
};