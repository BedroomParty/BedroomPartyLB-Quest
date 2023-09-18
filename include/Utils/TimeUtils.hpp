#pragma once
#include <string>
#include <chrono>

//yoinked from beatleader
namespace BedroomPartyLB::TimeUtils
{
    const int Second = 1;
    const int Minute = 60 * Second;
    const int Hour = 60 * Minute;
    const int Day = 24 * Hour;
    const int Month = 30 * Day;
    const int Year = 365 * Day;

    inline std::string GetRelativeTimeString(long timeSet) 
    {
        int timeSetSeconds = (int)timeSet;
        int nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int delta = nowSeconds - timeSetSeconds;
        if (delta < 1 * Minute) return delta == 1 ? "one second ago" : std::to_string(delta) + " seconds ago";
        if (delta < 2 * Minute) return "a minute ago";
        if (delta < 45 * Minute) return std::to_string(delta / Minute) + " minutes ago";
        if (delta < 90 * Minute) return "an hour ago";
        if (delta < 24 * Hour) return std::to_string(delta / Hour) + " hours ago";
        if (delta < 48 * Hour) return "yesterday";
        if (delta < 30 * Day) return std::to_string(delta / Day) + " days ago";
        if (delta < 12 * Month) 
        {
            int months = delta / Month;
            return months <= 1 ? "one month ago" : std::to_string(months) + " months ago";
        }
        int years = delta / Year;
        return years <= 1 ? "one year ago" : std::to_string(years) + " years ago";
    }
}