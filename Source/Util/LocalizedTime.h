#pragma once
#include "Localized.h"

/**
 * @file LocalizedTime.h
 * 
 * @brief Generates localized time strings.
 */
class LocalizedTime : public Localized
{
public:
    /**
     * @param time  The time being used to generate text.
     */
    LocalizedTime(juce::Time time);

    virtual ~LocalizedTime() { }

    /**
     * Gets a localized string describing the amount of time between the
     * present and this object's stored time.  This is only intended to be
     * used for time values in the past.
     * 
     * @return  the approximate difference between the present and this
     *          object's time, the localized equivalent of "now"
     *          if the time is not in the past, or the localized equivalent of
     *          "never" if the time is set to the Unix epoch.
     */
    juce::String approxTimePassed();

private:
    juce::Time time;

    //Localized text keys
    static const constexpr char * never = "never";
    static const constexpr char * now = "now";
    static const constexpr char * moments_ago = "moments_ago";

    static const constexpr char * seconds_pre = "seconds_pre";
    static const constexpr char * minutes_pre = "minutes_pre";
    static const constexpr char * hours_pre = "hours_pre";
    static const constexpr char * days_pre = "days_pre";
    static const constexpr char * weeks_pre = "weeks_pre";
    static const constexpr char * years_pre = "years_pre";

    static const constexpr char * seconds_post = "seconds_post";
    static const constexpr char * minutes_post = "minutes_post";
    static const constexpr char * hours_post = "hours_post";
    static const constexpr char * days_post = "days_post";
    static const constexpr char * weeks_post = "weeks_post";
    static const constexpr char * years_post = "years_post";
};