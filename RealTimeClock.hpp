#pragma once

#include <Arduino.h>
#include <Controllino.h>

#include "PoolControlContext.hpp"
#include "TimeOfDay.hpp"

class RealTimeClock
{
public:
    void init()
    {
        Controllino_RTC_init(0);
        // Controllino_SetTimeDateStrings(__DATE__, __TIME__);
    };

    void run()
    {
        PoolControlContext::instance()->data.date = getNow();
    }

    DateTime getNow()
    {
        return DateTime(Controllino_GetYear(), Controllino_GetMonth(), Controllino_GetDay(),
                        Controllino_GetHour(), Controllino_GetMinute(), Controllino_GetSecond());
    }

    static char *getFullDateTimeString(DateTime &dateTime, char *result)
    {
        memset(result, 0, 20);
        memcpy(result, "DD.MM.YYYY hh:mm:ss", 20);
        return dateTime.toString(result);
    }

private:
    // std::shared_ptr<RTC_DS3231> realTimeClock;
};
