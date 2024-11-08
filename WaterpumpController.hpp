#pragma once

#include <Arduino.h>
#include <Controllino.h>
#include "PoolControl_Config.h"
#include "PoolControlContext.hpp"
#include "TimeOfDay.hpp"

#define WATERPUMP_PIN CONTROLLINO_R10

class WaterpumpController
{
public:
    void init()
    {
        pinMode(WATERPUMP_PIN, OUTPUT);
        digitalWrite(WATERPUMP_PIN, LOW);
        PoolControlContext::instance()->data.waterPumpState = 0;
    }
    void run()
    {
        if (PoolControlContext::instance()->config.switchOn == PoolControlContext::instance()->config.switchOff)
        {
            return;
        }
        if (PoolControlContext::instance()->config.switchOn > PoolControlContext::instance()->config.switchOff)
        {
            return;
        }
        TimeOfDay now = PoolControlContext::instance()->data.date;
        if (PoolControlContext::instance()->config.switchOn < now && now < PoolControlContext::instance()->config.switchOff)
        {
            if (PoolControlContext::instance()->data.waterPumpState == 0)
            {
                PoolControlContext::instance()->data.waterPumpRunningSince = PoolControlContext::instance()->data.date;
            }
            PoolControlContext::instance()->data.waterPumpState = 1;
        }
        else
        {
            PoolControlContext::instance()->data.waterPumpState = 0;
        }
        // next "if" will not switch the pump off when it just started but
        // the flowswitch is still of for a small amount of time. Anyway, after that
        // time, when the flowswitch goes off suddely, we will switch the pump off
        // and go into error state
        if (PoolControlContext::instance()->data.waterPumpState == 1 &&
            PoolControlContext::instance()->data.waterFlowSwitch == 0 &&
            flowSwitchTooLongOff())
        {
            PoolControlContext::instance()->data.waterPumpState = 0;
            PoolControlContext::instance()->data.error = true;
            PoolControlContext::instance()->data.errorText = "Flowswitch still off when pump started";
            RealTimeClock::getFullDateTimeString(PoolControlContext::instance()->data.date, PoolControlContext::instance()->data.errorTimestamp);
        }

        if (PoolControlContext::instance()->data.error)
        {
            PoolControlContext::instance()->data.waterPumpState = 0;
        }
        digitalWrite(WATERPUMP_PIN, PoolControlContext::instance()->data.waterPumpState);
    }

private:
    bool flowSwitchTooLongOff()
    {
        TimeSpan difftime = PoolControlContext::instance()->data.date - PoolControlContext::instance()->data.waterPumpRunningSince;
        if (difftime.totalseconds() >= PoolControlContext::instance()->config.waterPumpOffWhenFlowswitchOffTime.totalseconds())
        {
            return true;
        }
        return false;
    }
};