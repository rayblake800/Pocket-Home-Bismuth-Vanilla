#pragma once
#include <sys/mman.h>
#include <arpa/inet.h>
#include "JuceHeader.h"

/**
 * @file IconCache.h
 */

class IconCache
{
public:
    IconCache(const char* cachePath);
    
    virtual ~IconCache();

private:
    //Image flags
    static const constexpr uint16 hasSuffixPng = 1;
    static const constexpr uint16 hasSuffixXpm = 2;
    static const constexpr uint16 hasSuffixSvg = 4;
    static const constexpr uint16 hasIconFile  = 8;

    int fd = -1;
    void* fileMap = MAP_FAILED;
    uint32 fileLen = 0;
    uint32 hashBuckets = 0;
    
    Array<String> directories;
    Array<uint32> hashOffsets;

    bool bigEndian = false;

    uint32 hashValue(const char* icon);

    template<typename T> T read(uint32 offset)
    {
        if(fileMap == MAP_FAILED || (fileLen + sizeof(T)) <= offset
	        || (offset + sizeof(T)) < offset)
	{
	    return T();
	}
        T value = * (T *) ((char*) fileMap + offset);
        if(bigEndian)
	{
	    return value;
	}
        return htonl(value);	
    }

    String readString(uint32 offset);
    
};
