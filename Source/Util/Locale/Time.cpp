#include "Locale/Time.h"

/* Class localized text key: */
static const juce::Identifier textUserKey = "Locale::Time";

/* Individual string keys: */
static const juce::Identifier neverKey          = "never";
static const juce::Identifier nowKey            = "now";
static const juce::Identifier momentsAgoKey     = "momentsAgo";

static const juce::Identifier secondsPrefixKey  = "secondsPrefix";
static const juce::Identifier secondsPostfixKey = "secondsPostfix";

static const juce::Identifier minutesPrefixKey  = "minutesPrefix";
static const juce::Identifier minutesPostfixKey = "minutesPostfix";

static const juce::Identifier hoursPrefixKey    = "hoursPrefix";
static const juce::Identifier hoursPostfixKey   = "hoursPostfix";

static const juce::Identifier daysPrefixKey     = "daysPrefix";
static const juce::Identifier daysPostfixKey    = "daysPostfix";

static const juce::Identifier weeksPrefixKey    = "weeksPrefix";
static const juce::Identifier weeksPostfixKey   = "weeksPostfix";

static const juce::Identifier yearsPrefixKey    = "yearsPrefix";
static const juce::Identifier yearsPostfixKey   = "yearsPostfix";


Locale::Time::Time(juce::Time time) : TextUser(textUserKey),
time(time) { }

/*
 * Gets a localized string describing the amount of time between the present and
 * this object's stored time.
 */
juce::String Locale::Time::approxTimePassed() const
{
    using juce::int64;
    using juce::String;
    int64 timeMS = time.toMilliseconds();
    if(timeMS == 0)
    {
        return localeText(neverKey);
    }
    int64 timePassed = juce::Time::currentTimeMillis() - time.toMilliseconds();
    if(timePassed < 1000)
    {
        return localeText(nowKey);
    }
    if(timePassed < 2000)
    {
        return localeText(momentsAgoKey);
    }
    timePassed /= 1000;
    if(timePassed < 120)
    {
        return localeText(secondsPrefixKey) + String(timePassed) 
                + localeText(secondsPostfixKey);
    }
    timePassed /= 60;
    if(timePassed < 120)
    {
        return localeText(minutesPrefixKey) + String(timePassed) 
                + localeText(minutesPostfixKey);
    }
    timePassed /= 60;
    if(timePassed < 48)
    {
        return localeText(hoursPrefixKey) + String(timePassed) 
                + localeText(hoursPostfixKey);
    }
    timePassed /= 24;
    if(timePassed < 14)
    {
        return localeText(daysPrefixKey) + String(timePassed) 
                + localeText(daysPostfixKey);
    }
    if(timePassed > 730)
    {
        return localeText(yearsPrefixKey) + String(timePassed/365) + 
                localeText(yearsPostfixKey);
    }
    return localeText(weeksPrefixKey) + String(timePassed/7) 
            + localeText(weeksPostfixKey);
}
