#include "JSONFile.h"
#include "AssetFiles.h"

/* Maximum number of decimal places saved when writing double values to JSON: */
static const constexpr int decimalPlacesSaved = 5;

/*
 * Creates a JSON data file interface, creating a new JSON file or reading an
 * existing JSON file's data.
 */
JSONFile::JSONFile(const juce::String filePath) : filePath(filePath) 
{ 
    using juce::var;
    jsonData = AssetFiles::loadJSONAsset(filePath);
    if (!jsonData.isObject())
    {
        jsonData = var(); 
        juce::File sourceFile = AssetFiles::findAssetFile(filePath);
        if (sourceFile.existsAsFile())
        {
            if (sourceFile.getSize() > 0)
            {
                throw FileException(filePath, "Not a valid JSON file!");
            }
        }
        else
        {
            juce::Result createFile = sourceFile.create();
            if (!createFile)
            {
                throw FileException(filePath, createFile.getErrorMessage());
            }
        }
        jsonData = var(new juce::DynamicObject());
    }
}

/*
 * Saves all changes back to the source file if applicable.
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
 * Removes and returns a value of type T from a var container. 
 */
template<> juce::String JSONFile::extractProperty<juce::String>
(juce::var& container) const { return container; }

template<> int JSONFile::extractProperty<int>
(juce::var& container) const { return container; }

template<> bool JSONFile::extractProperty<bool>
(juce::var& container) const { return container; }

template<> double JSONFile::extractProperty<double>
(juce::var& container) const { return container; }

template<> juce::var JSONFile::extractProperty<juce::var>
(juce::var& container) const { return container; }

template<> 
juce::Array<juce::var> JSONFile::extractProperty <juce::Array<juce::var>>
(juce::var& container) const
{
    using juce::Array;
    using juce::var;
    Array<var>* arrayProperty = container.getArray();
    if(arrayProperty != nullptr)
    {
        return Array<var>(*container.getArray());
    } 
        DBG("JSONFile::" << __func__ << ": Array is null!");
    return Array<var>();
}

template<> juce::DynamicObject* JSONFile::extractProperty<juce::DynamicObject*>
(juce::var& container) const
{
    return container.getDynamicObject();
}


/*
 * Gets the name of the data type held in the JSON var object.
 */
juce::String JSONFile::getTypeName(const juce::var& property) const
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
template<>juce::String JSONFile::getTypeName<int>() const
{
    return "int";
}

template<>juce::String JSONFile::getTypeName<bool>() const
{
    return "bool";
}

template<>juce::String JSONFile::getTypeName<double>() const
{
    return "double";
}

template<>juce::String JSONFile::getTypeName<juce::String>() const
{
    return "String";
}

template<>juce::String JSONFile::getTypeName<juce::DynamicObject*>() const
{
    return "DynamicObject*";
}

template<>juce::String JSONFile::getTypeName<juce::Array<juce::var>>() const
{
    return "Array<var>";
}

template<>juce::String JSONFile::getTypeName<juce::var>() const
{
    return "var";
}
