#define LOCALE_IMPLEMENTATION
#include "Locale/Locale.h"

/*
 * Get the name of the system locale.
 */
juce::String Locale::getLocaleName()
{
    using namespace juce;
    std::locale l("");
    return String(l.name()).initialSectionNotContaining(".");
}
