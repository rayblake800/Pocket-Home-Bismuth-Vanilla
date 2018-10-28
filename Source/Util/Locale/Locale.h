#pragma once
#include "JuceHeader.h"


/**
 * @file  Locale.h
 *
 * @brief  Handles localization tasks, loading localized display text and time
 *         strings.
 */
namespace Locale
{
    /**
     * @brief  Gets the name of the system locale.
     * 
     * @return   The short locale name, used for naming locale text files and
     *           identifying locale in .desktop files. 
     */
    juce::String getLocaleName();

    /**
     * @brief  An interface for classes that load localized text strings.
     *
     * @see Locale/TextUser.h
     */
    class TextUser;

    /**
     * @brief  Loads localized time strings.
     *
     * @see Locale/Time.h
     */
    class Time;

    /**
     * @brief  The private SharedResource class that reads locale files.  This
     *         may not be included outside of TextUser.h
     *
     * @see Locale/TextFile.h
     */
    class TextFile;

}
