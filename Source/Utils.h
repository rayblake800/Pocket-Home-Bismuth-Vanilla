#pragma once

#include<functional>
#include<map>
#include "../JuceLibraryCode/JuceHeader.h"

File absoluteFileFromPath(const String &path);
File assetFile(const String &path);

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
//get the user's home path
String getHomePath();

//perform function(struct dirent*) on all files in path
void foreachFile(const String& path, std::function<void(struct dirent*)> fn);

//Print debug info about the component tree
void componentTrace();

//List all non-directory files in path
std::vector<String> listFiles(const String& path);

//list all directory files in path, ignoring ./ and ../
std::vector<String> listDirectoryFiles(const String& path);

Rectangle<int>getWindowSize();