#pragma once

#include<functional>
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

bool fileExists(const String& path);
String getLocale();
String getTheme();
String getFallbackTheme();
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