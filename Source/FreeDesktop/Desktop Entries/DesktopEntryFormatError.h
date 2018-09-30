#pragma once
#include <exception>
#include "JuceHeader.h"

/**
 * @file  DesktopEntryFormatError.h
 *
 * @brief  Signals that an attempt was made to add invalid data to a
 *         DesktopEntry object.
 */
 struct DesktopEntryFormatError : public std::exception
{
public:
    /**
     * @brief Creates a new exception for an invalid data value.
     *
     * @param badValue  The value that failed to comply with desktop entry
     *                  standards.
     */
    DesktopEntryFormatError(const juce::String badValue) :
    badValue(badValue),
    errorMessage(juce::String("Invalid desktop entry value:\"") + badValue
            + "\"")
    { }

    /**
     * @brief  Gets a string describing the error. 
     *
     * @return  A non-specific format error message, along with the invalid
     *          value that triggered the exception.
     */
    virtual const char* what() const noexcept override
    {
        return errorMessage.toRawUTF8();
    }

    /**
     * @brief  Gets the value that triggered the exception.
     *
     * @return  The value that failed to comply with desktop entry 
     *          standards.
     */
    const juce::String& getBadValue() const
    {
        return badValue;
    }

private:
    const juce::String badValue;
    const juce::String errorMessage;
};
