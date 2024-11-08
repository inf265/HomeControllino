#pragma once
#include <Arduino.h>
#include <Controllino.h>
#include "PoolControl_Config.h"
#include "PoolControlContext.hpp"
#include "MedianValue.hpp"
#include "InjectionPumpControl.hpp"

#define PHPUMP_PIN CONTROLLINO_R12

/* Ph dosierung ca. 50-100ml pro 10m3 Wasser für Senkung um 0,1 pH
    Pumpe hat 1,5l/h
    --> sind 350-700 ml für 70m3 und 0,1 pH
    --> sind 14-28 minuten für 70m3 und 0,1 pH
 */

class PhController
{
public:
    PhController() : injectionPump(PHPUMP_PIN,
                                   PoolControlContext::instance()->config.phPumpCycleRunTime,
                                   PoolControlContext::instance()->config.phPumpCyclePauseTime,
                                   PoolControlContext::instance()->config.phPumpMaxRuntime) {}
    void init()
    {
        pinMode(PHPUMP_PIN, OUTPUT);
        digitalWrite(PHPUMP_PIN, LOW);
        PoolControlContext::instance()->data.phPumpState = 0;
        injectionPump.init(PHPUMP_PIN,
                           PoolControlContext::instance()->config.phPumpCycleRunTime,
                           PoolControlContext::instance()->config.phPumpCyclePauseTime,
                           PoolControlContext::instance()->config.phPumpMaxRuntime);
    }
    void run()
    {
        values.add(PoolControlContext::instance()->data.phValue);
        if (!values.isReady())
        {
            return;
        }
        PoolControlContext::instance()->data.phValueMedian = values.get();

        if (values.get() > (float)8.8) // dont ask me why I took 8.8
        {
            // for failed measurements
            injectionPump.off();
        }
        else if (values.get() > PoolControlContext::instance()->config.phTargetValue)
        {
            injectionPump.on();
        }
        else if (values.get() <= PoolControlContext::instance()->config.phTargetValue - PoolControlContext::instance()->config.phTargetValueHysterese)
        {
            injectionPump.off();
        }
        else
        {
            injectionPump.maintain();
        }
        if (injectionPump.isOn())
        {
            PoolControlContext::instance()->data.phPumpState = 1;
        }
        else
        {
            PoolControlContext::instance()->data.phPumpState = 0;
        }
    }

private:
    MedianValue values;
    InjectionPumpControl injectionPump;
};