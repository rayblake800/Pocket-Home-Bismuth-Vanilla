#pragma once
#include <sys/mman.h>
#include <arpa/inet.h>
#include <map>
#include "JuceHeader.h"

/**
 * @file IconCache.h
 * 
 * @brief  Reads GTK icon cache files to quickly find icon files.
 * 
 *  Maps GTK icon cache files to memory, where they can be used to look up icons
 * with a specific name within an icon theme directory. For more information on
 * the GTK icon cache file format, see 
 * https://raw.githubusercontent.com/GNOME/gtk/master/docs/iconcache.txt
 */

class IconCache
{
public:
    /**
     * Maps an icon cache file to memory.
     * 
     * @param cachePath  The path to an icon theme's icon-theme.cache file.
     */
    IconCache(String cachePath);
    
    virtual ~IconCache();

    /**
     * Checks if this object represents a valid cache file.  This ensures that
     * the cache file exists, is not out of date, and contains icon data. 
     * 
     * @return  true iff the object maps a valid cache file.
     */
    bool isValidCache();
    
    //icon flags
    /*
     * According to the documentation, these are the correct flag values.
     * However, they always provide incorrect results.
     
      static const constexpr uint16 pngExtensionFlag = 1;
      static const constexpr uint16 xpmExtensionFlag = 2;
      static const constexpr uint16 svgExtensionFlag = 4;
     
     * These values actually work:
     */
    
    static const constexpr uint16 xpmExtensionFlag = 1;
    static const constexpr uint16 svgExtensionFlag = 2;
    static const constexpr uint16 pngExtensionFlag = 4;

    
    /**
     * Looks up an icon's data in the icon cache.
     * 
     * @param iconName  The name of a requested icon file, without the file
     *                  extensions.
     * 
     * @return  All icon theme subdirectories containing this icon, mapped to
     *          the first available file extension for the icon within the 
     *          directory.
     */
    std::map<String,String> lookupIcon(const String& iconName);
    
private:
    /**
     * Calculate the hash value of an icon name.
     * 
     * @param icon  The name of a requested icon file, without the file
     *              extension.
     * 
     * @return  the hash value needed to look up the icon in the icon cache. 
     */
    uint32 hashValue(const char* icon);
    
    /**
     * Reads a two-byte unsigned integer from an arbitrary offset in the cache
     * file, after ensuring that the cache file is valid and the offset is
     * within the file bounds.
     * 
     * @param offset  The offset in bytes from the beginning of the cache file.
     * 
     * @return  The uint16 at that offset, converted from network to host byte
     *          order if necessary, or 0 if the cache file or offset index are
     *          invalid. 
     */
    uint16 read16(uint32 offset);
    
    /**
     * Reads a four-byte unsigned integer from an arbitrary offset in the cache
     * file, after ensuring that the cache file is valid and the offset is
     * within the file bounds.
     * 
     * @param offset  The offset in bytes from the beginning of the cache file.
     * 
     * @return  The uint32 at that offset, converted from network to host byte
     *          order if necessary, or 0 if the cache file or offset index are
     *          invalid. 
     */
    uint16 read32(uint32 offset);
    
    /**
     * Reads a null-terminated character string from an arbitrary offset in the 
     * cache file, after ensuring that the cache file is valid and the offset is
     * within the file bounds.
     * 
     * @param offset  The offset in bytes from the beginning of the cache file.
     * 
     * @return  The String at that offset, or the empty string if the file
     *          is invalid, the offset is out of bounds, or the null terminator
     *          is not found.
     */
    String readString(uint32 offset);

    //file descriptor needed to map the cache file to memory
    int fd = -1;
    
    //pointer to the memory-mapped cache file data
    void* fileMap = MAP_FAILED;
    
    //length of the cache file in bytes
    int64 fileLen = 0;
    
    //number of hash values used to store icon data
    uint32 hashBuckets = 0;
    
    //all icon directory sub-paths within the icon theme directory
    Array<String> directories;
    
    //the index of each hash bucket within the icon file
    Array<uint32> hashOffsets;
    
    //stores if the system byte order is big-Endian.  If this value is false,
    //numbers read from the cache file will need to be converted from network to
    //host order.
    bool bigEndian = false;

    
};
