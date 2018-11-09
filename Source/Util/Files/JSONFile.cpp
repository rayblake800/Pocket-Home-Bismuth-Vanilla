#include "JSONFile.h"
#include "AssetFiles.h"

//Maximum number of decimal places to save when writing double values to JSON.
static const constexpr int decimalPlacesSaved = 5;

/*
 * Opens a JSON data file interface.
 */
JSONFile::JSONFile(const juce::String filePath)
: filePath(filePath) { }

/*
 * Saves all changes back to the source file, if applicable.
 */
JSONFile::~JSONFile()
{
    try
    {
        writeChanges();
    }
    catch(FileException e)
    {
        DBG("JSONFile::" << __func__ << ": " << e.what());
    }
}

/*
 * Re-writes all data back to the config file, as long as there are
 * changes to write.
 */
void JSONFile::writeChanges()
{
    using namespace juce;
    if (!unwrittenChanges || jsonData.isVoid())
    {
        return;
    }
    String jsonText = JSON::toString(jsonData, false, decimalPlacesSaved);
    File jsonFile = AssetFiles::findAssetFile(filePath);
    if (!jsonFile.replaceWithText(jsonText))
    {
        throw FileException(filePath, "Writing changes failed.");
    }
    else
    {
	jsonFile.setLastModificationTime(Time::getCurrentTime());
        unwrittenChanges = false;
    }
}
 
/*
 * Unloads all file data from memory, writing any pending changes first.
 * File data will be reloaded from disk if any methods accessing property
 * data are called.
 */
void JSONFile::unloadData()
{
    if(!openedFile)
    {
        return;
    }
    writeChanges();
    jsonData = juce::var();
    openedFile = false;
}

/*
 * Removes and returns a value of type T from a var container. The caller
 * is responsible for ensuring that the container definitely contains
 * a value of type T.
 */
template<> juce::String JSONFile::extractProperty<juce::String>
(juce::var& container) { return container; }

template<> int JSONFile::extractProperty<int>
(juce::var& container) { return container; }

template<> bool JSONFile::extractProperty<bool>
(juce::var& container) { return container; }

template<> double JSONFile::extractProperty<double>
(juce::var& container) { return container; }

template<> juce::var JSONFile::extractProperty<juce::var>
(juce::var& container) { return container; }

template<> 
juce::Array<juce::var> JSONFile::extractProperty <juce::Array<juce::var>>
(juce::var& container)
{
    using namespace juce;
    Array<var>* arrayProperty = container.getArray();
    if(arrayProperty != nullptr)
    {
        return Array<var>(*container.getArray());
    } 
        DBG("JSONFile::" << __func__ << ": Array is null!");
    return Array<var>();
}

template<> juce::DynamicObject* JSONFile::extractProperty<juce::DynamicObject*>
(juce::var& container)
{
    return container.getDynamicObject();
}


/*
 * Gets the name of the data type held in the JSON var object.
 */
juce::String JSONFile::getTypeName(const juce::var& property)
{
    if (property.isVoid())
    {
        return "void";
    }
    if (property.isUndefined())
    {
        return "undefined";
    }
    if (property.isInt())
    {
        return "int";
    }
    if (property.isInt64())
    {
        return "int64";
    }
    if (property.isBool())
    {
        return "bool";
    }
    if (property.isDouble())
    {
        return "double";
    }
    if (property.isString())
    {
        return "String";
    }
    if (property.isArray())
    {
        return "Array<var>";
    }
    if (property.isBinaryData())
    {
        return "MemoryBlock*";
    }
    if (property.isMethod())
    {
        return "Method";
    }
    if (property.isObject())
    {
        return "DynamicObject*";
    }
    return "var";
}

/*
 * Gets the name of a data type that may be contained in a JSON var object.
 */
template<>juce::String JSONFile::getTypeName<int>()
{
    return "int";
}

template<>juce::String JSONFile::getTypeName<bool>()
{
    return "bool";
}

template<>juce::String JSONFile::getTypeName<double>()
{
    return "double";
}

template<>juce::String JSONFile::getTypeName<juce::String>()
{
    return "String";
}

template<>juce::String JSONFile::getTypeName<juce::DynamicObject*>()
{
    return "DynamicObject*";
}

template<>juce::String JSONFile::getTypeName<juce::Array<juce::var>>()
{
    return "Array<var>";
}

template<>juce::String JSONFile::getTypeName<juce::var>()
{
    return "var";
}

/*
 * If the file has not yet been opened, this will open it, and read its
 * data into the jsonData var object.
 */
void JSONFile::readFileIfUnopened()
{
    using namespace juce;
    if (!openedFile && jsonData.isVoid())
    {
        jsonData = AssetFiles::loadJSONAsset(filePath);
        if (!jsonData.isObject())
        {
            jsonData = var(); 
            File sourceFile = AssetFiles::findAssetFile(filePath);
            if (sourceFile.existsAsFile())
            {
                if (sourceFile.getSize() > 0)
                {
                    throw FileException(filePath, "Not a valid JSON file!");
                }
            }
            else
            {
                Result createFile = sourceFile.create();
                if (!createFile)
                {
                    throw FileException(filePath, createFile.getErrorMessage());
                }
            }
            jsonData = var(new DynamicObject());
        }
        openedFile = true;
    }
}
