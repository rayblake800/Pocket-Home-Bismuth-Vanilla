#pragma once

#include<functional>
#include<map>
#include "../JuceLibraryCode/JuceHeader.h"

File absoluteFileFromPath(const String &path);
File assetFile(const String &path);
File assetConfigFile(const String &fileName);

Image createImageFromFile(const File &imageFile);
ImageButton *createImageButton(const String &name, const File &imageFile);
ImageButton *createImageButton(const String &name, const Image &image);
ImageButton *createImageButtonFromDrawable(const String &name, const Drawable &drawable);

void fitRectInRect(Rectangle<int> &rect, int x, int y, int width, int height,
                   Justification justification, const bool onlyReduceInSize);
void fitRectInRect(Rectangle<int> &rect, const Rectangle<int> &container,
                   Justification justification, const bool onlyReduceInSize);

float smoothstep(float edge0, float edge1, float x);
float mix(float a, float b, float t);
float mapLinear(float x, float imin, float imax, float omin, float omax);

void animateTranslation(Component *component, int x, int y, float alpha, int durationMillis);

std::vector<String> split(const String &orig, const String &delim);

//returns true if the file at path already exists
bool fileExists(const String& path);
//get the locale name as it would appear in .desktop files
String getLocale();
//get the user's icon theme
String getTheme();
//get the fallback icon theme
String getFallbackTheme();
//get the user's home path
String getHomePath();

//perform function(struct dirent*) on all files in path
void foreachFile(const String& path, std::function<void(struct dirent*)> fn);

//List all non-directory files in path
std::vector<String> listFiles(const String& path);

//list all directory files in path, ignoring ./ and ../
std::vector<String> listDirectoryFiles(const String& path);

/**
for each entry in container, run fn(entry)
if fn returns true, stop the loop
 * container: must have a .size method, and
 * data entries accessible using container[index]
 * 
 * fn:must be some form of callable function that accepts an element
 * of the type held in c, and returns a boolean value.
**/
template<class C,class F>
void foreach(C container,F fn){
    for(int i = 0; i<container.size();i++){
        if(fn(container[i]))break;
    }
}


/**
 * Saves file path locations to a single cache file
 * so that hunting down icons doesn't take so long.
 * If the selected icon theme changes, existing data
 * will be discarded.
 */
class PathRecord{
public:
    //Saved data is loaded during construction
    PathRecord();
    PathRecord(const PathRecord& orig);
    //Any unsaved changes will be written to the record file
    ~PathRecord();
    
    /**
     * Given the name of an icon, return its absolute path, or an
     * empty string if no path is found
     * @param iconName the name of some icon file
     * @return absolute system path of the icon
     */
    String getRecord(String iconName);
    
    /**
     * Add a new icon path to the record. Note that this will not write
     * to the cache file right away.
     * @param iconName the name of some icon file
     * @param fullPath absolute system path of the icon
     */
    void addRecord(String iconName,String fullPath);
    
    /**
     * write all records to the cache file, as long as there's something new
     * to write
     */
    void writeRecords();
    //value that indicates the file doesn't exist
    static constexpr const char* noPath="NO_PATH_EXISTS";
private:
    std::map<String,String> pathRecords;//maps iconName=fullPath pairs
    String iconTheme;//current icon theme
    //cache file location in the user's home directory
    static constexpr const char* recordPath=".pocket-home/pathcache";
    bool changesWritten=true;
};