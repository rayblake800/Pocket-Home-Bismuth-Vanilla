/**
 * @file Utils.h
 * 
 * TODO: work as much of this as possible into distinct modules, having a
 * random pile of assorted global functions is less than ideal.
 * also, documentation
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

File absoluteFileFromPath(const String &path);
File assetFile(const String &path);

Image createImageFromFile(const File &imageFile);

/**
 * Creates a Drawable object from an svg file
 * @param svgFile a valid svg file.
 * @return A drawable if the file was valid, nullptr otherwise
 */
Drawable * createSVGDrawable(const File& svgFile);

/**
 * 
 * @param rect
 * @param x
 * @param y
 * @param width
 * @param height
 * @param justification
 * @param onlyReduceInSize
 */
void fitRectInRect(Rectangle<int> &rect, int x, int y, int width, int height,
        Justification justification, const bool onlyReduceInSize);

/**
 * 
 * @param rect
 * @param container
 * @param justification
 * @param onlyReduceInSize
 */
void fitRectInRect(Rectangle<int> &rect, const Rectangle<int> &container,
        Justification justification, const bool onlyReduceInSize);

/**
 * 
 * @param edge0
 * @param edge1
 * @param x
 * @return 
 */
float smoothstep(float edge0, float edge1, float x);

/**
 * 
 * @param a
 * @param b
 * @param t
 * @return 
 */
float mix(float a, float b, float t);

/**
 * 
 * @param x
 * @param imin
 * @param imax
 * @param omin
 * @param omax
 * @return 
 */
float mapLinear(float x, float imin, float imax, float omin, float omax);

/**
 * 
 * @param component
 * @param x
 * @param y
 * @param alpha
 * @param durationMillis
 */
void animateTranslation(Component *component, int x, int y, float alpha, int durationMillis);

/**
 * 
 * @param orig
 * @param delim
 * @return 
 */
Array<String> split(const String &orig, const String &delim);


/**
 * @return the user's home path
 */
String getHomePath();

//perform function(struct dirent*) on all files in path
/**
 * 
 * @param path
 * @param fn
 */
void foreachFile(const String& path, std::function<void(struct dirent*) > fn);


static DrawableRectangle highlightFocus;

#if JUCE_DEBUG
/**
 * Print debug info about the component tree
 */
void componentTrace();
#endif

/**
 * List all non-directory files in path
 * @param path
 * @return 
 */
std::vector<String> listFiles(const String& path);

/**
 * list all directory files in path, ignoring ./ and ../
 * @param path
 * @return 
 */
std::vector<String> listDirectoryFiles(const String& path);

/**
 * 
 * @return the size of the active window
 */
Rectangle<int> getWindowSize();


/**
 * resizes a font to fit in a containing rectangle.
 * If fitting it in would require mangling the font size too much, the
 * font gets set to size zero.
 * @param font
 * @param text
 * @param container
 * @return 
 */
juce::Font fontResizedToFit(juce::Font font, String text, Rectangle<int>container);

/**
 * Requests user confirmation before performing some action
 * @param title the text to print at the top of the confirmation box.
 * @param message more detailed information on the action being confirmed.
 * @param onConfirm if the user presses the "OK" button, this callback
 * function will run.
 */
void confirmAction(String title, String message, std::function<void() > onConfirm);

/**
 * 
 * @param a
 * @param b
 * @param c
 * @return the median of a, b, and c
 */
template<typename T>
T median(T a, T b, T c) {
    if (a > b) {
        if (b > c) {
            return b;
        }
        if (a < c) {
            return a;
        }
        return c;
    }
    if (a > c) {
        return a;
    }
    if (b < c) {
        return b;
    }
    return c;
}

/**
 * @param checkIfC2 a pointer to an object of type C1
 * @return true iff the object pointed to by checkIfC2 also has type C2.
 */
template<class C1, class C2>
bool isClass(C1* checkIfC2) {
    C2* testPtr = dynamic_cast<C2*> (checkIfC2);
    return testPtr != nullptr;
}