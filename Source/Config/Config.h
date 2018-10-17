#pragma once

/**
 * @file Config.h
 * 
 * @brief  A module that reads and writes files that hold application 
 *         configuration settings.
 *
 * The Config module defines an interface for reading and writing application 
 * configuration settings from user-accessable JSON files. Config allows easy,
 * threadsafe access to configuration data, minimizing the amount of necessary
 * file IO.  Config also defines Listener objects, which are notified whenever
 * configuration data they track is changed at any point.
 *
 * Each configuration file has its own FileHandler
 * class. Any object of that FileHandler class can be used to read or write 
 * all data values in the JSON file in a threadsafe manner.  
 *
 * FileHandlers also define their own listener classes, which receive 
 * notifications whenever data keys they select are changed through any 
 * FileHandler connected to the same JSON file.
 */

namespace Config
{
    /**
     * @brief  A restricted SharedResource class type used for reading and 
     *         writing JSON data.
     *
     * Each JSON configuration file has its own FileResource subclass.  A single
     * object of that subclass handles all direct access to that JSON file.
     *
     * @see  FileResource.h
     */
    class FileResource;

    /**
     * @brief  A ResourceHandler class template used to access a FileResource.
     *
     * Each FileResource subclass has a corresponding FileHandler subclass. Any
     * object of that FileHandler subclass can safely access the data stored in
     * its single FileResource object.
     *
     * @tparam ResourceType  The FileResource subclass this FileHandler will 
     *                       access.
     *
     * @see  FileHandler.h
     */
    template<class ResourceType> class FileHandler;

    /**
     * @brief  A FileHandler for accessing the main config.json configuration
     *         file.
     *
     * This class provides a good example of a minimal FileHandler 
     * implementation.
     *
     * @see  MainFile.h
     */
    class MainFile;

    /**
     * @brief  A FileResource that reads and writes the main config.json
     *         configuration file.
     *
     *  This class provides a good example of a minimal FileResource 
     *  implementation.
     *
     *  @see  MainResource.h
     */
    class MainResource;

    /**
     * @brief  An alert window that displays any problems in reading or writing
     *         configuration files.
     * 
     * @see  AlertWindow.h
     */
    class AlertWindow;

    /**
     * @brief  Identifies a basic data value in a configuration file, and
     *         specifies its type.
     *
     * @see  DataKey.h
     */
    class DataKey;
}
