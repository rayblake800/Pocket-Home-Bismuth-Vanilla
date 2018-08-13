#pragma once
#include <exception>
#include "JuceHeader.h"

/**
 * @file JSONFile.h
 * 
 * @brief Provides a simplified interface for reading and writing JSON file
 *        data.
 */
class JSONFile
{   
public:
    /**
     * Creates a JSON data file interface.
     * 
     * @param filePath      The path to a .json file.  This may be a path 
     *                      relative to the pocket-home assets folder, an 
     *                      absolute path, or a path relative to the current 
     *                      working directory.
     * 
     * @param defaultPath   The location of the default version of this .json
     *                      file, relative to the pocket-home assets folder.
     *                      If no file is found at filePath and this value is
     *                      not the empty string, this will attempt to copy the
     *                      default file to filePath.
     */
    JSONFile(const juce::String filePath,
            const juce::String defaultPath = juce::String());
    
    /**
     * Saves all changes back to the source file, if applicable.
     */
    virtual ~JSONFile();

    /**
     * Gets a property stored in this JSON data.
     * 
     * @param   key  The key string identifying the property.
     * 
     * @tparam  T    The type of value to return.
     * 
     * @return  The value extracted from the JSON data.
     * 
     * @throws FileException   If the file is missing or could not be read.
     * 
     * @throws TypeException   If no property with type T was found.
     */
    template<typename T > T getProperty(const juce::String& key)
    {
        using namespace juce;
        readFileIfUnopened();
        jassert(!jsonData.isVoid());
        if(!jsonData.hasProperty(key))
        {        
            throw TypeException(getTypeName<T>(), "void", key);
        }
        
        var property = jsonData[Identifier(key)];
        typeCheck<T>(property, key);
        return extractProperty<T>(property);
    }
     
    /**
     * Sets a property stored in this JSON data.
     * 
     * @param key       The key string used to set the value.
     * 
     * @param newValue  A new value to store in the JSON object.
     * 
     * @tparam T        The type of value being stored.
     * 
     * @return  True if the property changed, false if the existing value
     *          matched the new value.
     * 
     * @throws FileException   If the file is missing or could not be read.
     * 
     * @throws TypeException  If a property exists that shares this key but is 
     *                        not of type T.
     */
    template<typename T > bool setProperty(const juce::String& key, T newValue)
    {
        using namespace juce;
        readFileIfUnopened();
        jassert(!jsonData.isVoid());
        var newProperty(newValue);
        if(jsonData.hasProperty(key))
        {
            var oldProperty = jsonData[Identifier(key)];
            if(oldProperty == newProperty)
            {
                return false;
            }
            typeCheck<T>(oldProperty, key);
        }     
        jsonData.getDynamicObject()->setProperty(key, var(newValue));
        unwrittenChanges = true;
        return true;
    }
    
    /**
     * Checks if JSON data contains data of a specific type assigned to a 
     * specific key.
     * 
     * @param key  The key value to search for in the JSON data.
     * 
     * @tparam T   The expected type of the value stored in the JSON data
     * 
     * @return  True iff a value of type T exists at the given key.
     */
    template<typename T> bool propertyExists(const juce::String& key)
    {
        using namespace juce;
        readFileIfUnopened();
        if(jsonData.isVoid())
        {
            return false;
        }
        if(!jsonData.hasProperty(key))
        {
            return false;
        }
        if(getTypeName<T>() == getTypeName<var>())
        {
            return !jsonData.getProperty(key, var()).isVoid();
        }
        return getTypeName<T>() == getTypeName
                (jsonData.getProperty(key, var()));    
    }
    
    /**
     * Re-writes all data back to the config file, as long as there are
     * changes to write.
     * 
     * @throws FileException  If changes could not be written to the file.
     */
    void writeChanges();
    
    /**
     * Signals a failure to read from or write to the JSON config file.
     */
    struct FileException : public std::exception{
    public:
        /**
         * @param filePath       The path of the file that couldn't be accessed.
         * 
         * @param errorMessage   A short error message describing the failure.
         */
        FileException(juce::String filePath, juce::String errorMessage) :
        filePath(filePath), errorMessage(errorMessage) { }
        
        /**
         * Gets the path of the file that couldn't be accessed.
         * 
         * @return  The JSON file's path.
         */
        juce::String getFilePath()
        {
            return filePath;
        }

        /**
         * Gets an error message explaining the reason for the exception.
         * 
         * @return  A short error message.
         */
        juce::String getErrorMessage()
        {
            return errorMessage;
        }
        
    private:     
        juce::String filePath;
        juce::String errorMessage;
    };
    
    
    /**
     * Signals that a JSON object property did not have the expected type.
     */
    struct TypeException : public std::exception
    {
    public:
        /**
         * @param expectedType  The name of the expected property type.
         * 
         * @param foundType     The name of the actual property type.
         * 
         * @param key           The key string of the property that caused the
         *                      error.
         */
        TypeException(juce::String expectedType, juce::String foundType,
                juce::String key) :
                expectedType(expectedType),
                foundType(foundType),
                key(key) { }
                
        
        /**
         * Gets the name of the expected type of the JSON property.
         * 
         * @return  A string describing the expected property type.
         */
        juce::String getExpectedType()
        {
            return expectedType;
        }
        
        /**
         * Gets the name of the actual type of the JSON property.
         * 
         * @return  A string describing the actual property type.
         */
        juce::String getFoundType()
        {
            return foundType;
        }
        
        /**
         * Gets the key string of the JSON property responsible for the
         * exception
         * 
         * @return  The property key string.
         */
        juce::String getPropertyKey()
        {
            return key;
        }
        
        /**
         * Gets a short error message suitable for debug output only.
         */
        juce::String getErrorMessage()
        {
            juce::String error = "Key \"";
            error += key;
            error += "\" Expected: ";
            error += expectedType;
            error += ", Found:";
            error += foundType;
            return error;
        }
        
    private:
        juce::String expectedType;
        juce::String foundType;
        juce::String key;
    };
    
private:
    
    /**
     * Checks if the type of a property matches the expected type, throwing an
     * exception if they do not match.
     * 
     * @param property  A property read from a JSON file.
     * 
     * @param key       The property key string.
     * 
     * @tparam T        The expected type of the value stored in the property
     *                  var.
     * 
     * @throws TypeException   If the value stored in the property var is not
     *                         of type T.
     */
    template<typename T > void typeCheck
    (const juce::var& property, const juce::String& key)
    {
        using namespace juce;
        String expected = getTypeName<T>();
        String found = getTypeName(property);
        if(expected != getTypeName<var>() && expected != found)
        {
            throw TypeException(expected, found, key);
        }
    }
    
    /**
     * Removes and returns a value of type T from a var container. The caller
     * is responsible for ensuring that the container definitely contains
     * a value of type T.
     * 
     * @param container  A JSON property read from the JSON file.
     * 
     * @tparam T        The type of object stored in the container.
     * 
     * @return  The contained value of type T. 
     */
    template<typename T> T extractProperty(juce::var& container);
    
    /**
     * Gets the name of the data type held in the JSON var object.
     * 
     * @param property  A var object that may or may not contain data.
     * 
     * @return  A string describing the data in the property var.
     */
    juce::String getTypeName(const juce::var& property);
    
    /**
     * Gets the name of a data type that may be contained in a JSON var object.
     * 
     * @tparam T  The type name to get.
     * 
     * @return  The name of type T as a Juce String.
     */
    template<typename T >juce::String getTypeName();
    
private:
    /**
     * If the file has not yet been opened, this will open it, and read its
     * data into the jsonData var object.  If the file does not exist, it will
     * be created.
     * 
     * @throws  FileException  If the file could not be read, or could not be
     *                         created.
     */
    void readFileIfUnopened();
    
    
    //The path used to load this JSON file
    juce::String filePath;
    
    //Contains all JSON object data loaded from the file.
    juce::var jsonData;
    
    //Indicates if the file has been opened yet.  The file is not opened for
    //reading until it is first needed.
    bool openedFile = false;
    
    //Indicates if this object contains unsaved changes that need to be written
    //to the source file.
    bool unwrittenChanges = false;
};
