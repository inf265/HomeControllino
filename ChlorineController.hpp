#pragma once
#include <Arduino.h>
#include <Controllino.h>
#include "PoolControl_Config.h"
#include "PoolControlContext.hpp"
#include "MedianValue.hpp"
#include "InjectionPumpControl.hpp"

#define CHLORINEPUMP_PIN CONTROLLINO_R13

/* Chlor liquide 48: 1-1,5 l pro Tag für 100m3
   --> für 75m3: 0,75 - 1,0 l / Tag
   --> nicht gleichzeitig mit ph minus
    Pumpe hat 1,5l/h
    --> sind 30 - 45 minuten pro Tag
    --> sind 3 bis 4 mal 10min über die Waterpump dauer verteilt
 */

class ChlorineController
{
public:
    ChlorineController() : injectionPump(CHLORINEPUMP_PIN,
                                         PoolControlContext::instance()->config.chlorinePumpCycleRunTime,
                                         PoolControlContext::instance()->config.chlorinePumpCyclePauseTime,
                                         PoolControlContext::instance()->config.chlorinePumpMaxRuntime) {}
    void init()
    {
        pinMode(CHLORINEPUMP_PIN, OUTPUT);
        digitalWrite(CHLORINEPUMP_PIN, LOW);
        PoolControlContext::instance()->data.redoxPumpState = 0;
        injectionPump.init(CHLORINEPUMP_PIN,
                           PoolControlContext::instance()->config.chlorinePumpCycleRunTime,
                           PoolControlContext::instance()->config.chlorinePumpCyclePauseTime,
                           PoolControlContext::instance()->config.chlorinePumpMaxRuntime);
    }
    void run()
    {
        values.add(PoolControlContext::instance()->data.redoxValue);
        if (!values.isReady())
        {
            return;
        }

        PoolControlContext::instance()->data.redoxValueMedian = values.get();

        TimeOfDay now = PoolControlContext::instance()->data.date;
        if (PoolControlContext::instance()->config.switchChlorOn < now && now < PoolControlContext::instance()->config.switchChlorOff)
        {
            injectionPump.on();
        }
        else
        {
            injectionPump.off();
        }

        if (injectionPump.isOn())
        {
            PoolControlContext::instance()->data.redoxPumpState = 1;
        }
        else
        {
            PoolControlContext::instance()->data.redoxPumpState = 0;
        }
    }

private:
    MedianValue values;
    InjectionPumpControl injectionPump;
};