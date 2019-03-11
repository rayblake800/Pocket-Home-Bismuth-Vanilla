#pragma once
/**
 * @file  Window_Info.h
 *
 * @brief  Provides functions for getting information about the main application
 *         window.
 */

namespace juce { template <typename T> class Rectangle; }

namespace Window
{
    namespace Info
    {
        /**
         * @brief  Gets the size of the main application window.
         * 
         * @return  The bounds of the application window, measured in pixels.
         */
        juce::Rectangle<int> getBounds();

        /**
         * @brief  Gets the width of the main application window.
         *
         * @return   The application window width, measured in pixels.
         */
        int getWidth();

        /**
         * @brief  Gets the height of the main application window.
         *
         * @return   The application window height, measured in pixels.
         */
        int getHeight();
    }
}
