#pragma once
/**
 * @file  Icon_Cache.h
 *
 * @brief  Reads GTK icon cache files to quickly find icon files.
 */

#include <sys/mman.h>
#include <arpa/inet.h>
#include <map>
#include "JuceHeader.h"

namespace Icon { class Cache; }
/**
 * @brief  Maps GTK icon cache files to memory, where they can be used to look
 *         up icons with a specific name within an icon theme directory.
 *
 *  For more information on the GTK icon cache file format, see
 * https://raw.githubusercontent.com/GNOME/gtk/master/docs/iconcache.txt
 */
class Icon::Cache
{
public:
    /**
     * @brief  Maps an icon theme cache file to memory.
     *
     * @param themePath  The absolute path of an icon theme directory.
     */
    Cache(const juce::String& themePath);

    /**
     * @brief  Creates an empty, invalid cache object.
     */
    Cache() { }

    /**
     * @brief  Closes the cache file on destruction.
     */
    virtual ~Cache();

    /**
     * @brief  Checks if this object represents a valid cache file. This
     *         ensures that the cache file exists, is not out of date, and
     *         contains icon data.
     *
     * @return  Whether the object maps a valid cache file.
     */
    bool isValidCache() const;

    /**
     * @brief  Looks up an icon's data in the icon cache.
     *
     * @param iconName  The name of a requested icon file, without the file
     *                  extensions.
     *
     * @return          All icon theme subdirectories containing this icon,
     *                  mapped to the first available file extension for the
     *                  icon within the directory.
     */
    std::map<juce::String, juce::String> lookupIcon
    (const juce::String& iconName) const;

private:
    /**
     * @brief  Calculates the hash value of an icon name.
     *
     * @param icon  The name of a requested icon file, without the file
     *              extension.
     *
     * @return      The hash value needed to look up the icon in the icon cache.
     */
    juce::uint32 hashValue(const char* icon) const;

    /**
     * @brief  Reads a two-byte unsigned integer from an arbitrary offset in
     *         the cache file, after ensuring that the cache file is valid and
     *         the offset is within the file bounds.
     *
     * @param offset  The offset in bytes from the beginning of the cache file.
     *
     * @return        The uint16 at that offset, converted from network to host
     *                byte order if necessary, or 0 if the cache file or offset
     *                index are invalid.
     */
    juce::uint16 read16(juce::uint32 offset) const;

    /**
     * @brief  Reads a four-byte unsigned integer from an arbitrary offset in
     *         the cache file, after ensuring that the cache file is valid and
     *         the offset is within the file bounds.
     *
     * @param offset  The offset in bytes from the beginning of the cache file.
     *
     * @return        The uint32 at that offset, converted from network to host
     *                byte order if necessary, or 0 if the cache file or offset
     *                index are invalid.
     */
    juce::uint32 read32(juce::uint32 offset) const;

    /**
     * @brief  Reads a null-terminated character string from an arbitrary
     *         offset in the cache file, after ensuring that the cache file is
     *         valid and the offset is within the file bounds.
     *
     * @param offset  The offset in bytes from the beginning of the cache file.
     *
     * @return        The String at that offset, or the empty string if the
     *                file is invalid, the offset is out of bounds, or the null
     *                terminator is not found.
     */
    juce::String readString(juce::uint32 offset) const;


    // File descriptor needed to map the cache file to memory:
    int fd = -1;

    // Pointer to the memory-mapped cache file data:
    void* fileMap = MAP_FAILED;

    // Length of the cache file in bytes:
    juce::int64 fileLen = 0;

    // Number of hash buckets used to store hashed icon data in the file:
    juce::uint32 hashBuckets = 0;

    // All icon directory sub-paths within the icon theme directory:
    juce::Array<juce::String> directories;

    // The index of each hash bucket within the cache file:
    juce::Array<juce::uint32> hashOffsets;

    // Whether the system byte order is big-Endian. If this value is false,
    // numbers read from the cache file will need to be converted from network
    // to host order.
    bool bigEndian = false;
};
