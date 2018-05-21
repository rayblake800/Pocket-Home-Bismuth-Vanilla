#include "LocalizedTime.h"

LocalizedTime::LocalizedTime(Time time) : Localized("LocalizedTime"),
time(time) { }

/*
 * Gets a localized string describing the amount of time between the
 * present and this object's stored time.  This is only intended to be
 * used for time values in the past.
 * 
 * @return  the approximate difference between the present and this
 *          object's time, or the localized equivalent of "now"
 *          if the time is not in the past.
 */
String LocalizedTime::approxTimePassed()
{
    int64 timeMS = time.toMilliseconds();
    if(timeMS == 0)
    {
        return localeText(never);
    }
    int64 timePassed = Time::currentTimeMillis() - time.toMilliseconds();
    if(timePassed <= 0)
    {
        return localeText(now);
    }
    if(timePassed < 2000)
    {
        return localeText(moments_ago);
    }
    timePassed /= 1000;
    if(timePassed < 120)
    {
        return localeText(seconds_pre) + String(timePassed) 
                + localeText(seconds_post);
    }
    timePassed /= 60;
    if(timePassed < 120)
    {
        return localeText(minutes_pre) + String(timePassed) 
                + localeText(minutes_post);
    }
    timePassed /= 60;
    if(timePassed < 48)
    {
        return localeText(hours_pre) + String(timePassed) 
                + localeText(hours_post);
    }
    timePassed /= 24;
    if(timePassed < 14)
    {
        return localeText(days_pre) + String(timePassed) 
                + localeText(days_post);
    }
    if(timePassed > 730)
    {
        return localeText(years_pre) + String(timePassed/365) + 
                localeText(years_post);
    }
    return localeText(weeks_pre) + String(timePassed/7) 
            + localeText(weeks_post);
}