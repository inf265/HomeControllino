#pragma once

#include <Controllino.h>

#include "PoolControlContext.hpp"

class AdcController
{
public:
    void init()
    {
        pinMode(CONTROLLINO_A6, INPUT);  // ph 5V on header
        pinMode(CONTROLLINO_A8, INPUT);  // redox 5V on header
        pinMode(CONTROLLINO_I16, INPUT); // waterflowswitch 12V on klemmleiste
    }
    void run()
    {
        PoolControlContext::instance()->data.phAdcValue = (float)analogRead(CONTROLLINO_A6) * (float)((float)5000.0 / (float)1023.0);
        PoolControlContext::instance()->data.redoxAdcValue = (float)analogRead(CONTROLLINO_A8) * (float)((float)5000.0 / (float)1023.0);
        PoolControlContext::instance()->data.phValue = PoolControlContext::instance()->config.phCalculationMValue * (float)(PoolControlContext::instance()->data.phAdcValue) + PoolControlContext::instance()->config.phCalculationCValue;
        PoolControlContext::instance()->data.redoxValue = PoolControlContext::instance()->config.redoxCalculationMValue * (float)(PoolControlContext::instance()->data.redoxAdcValue) + PoolControlContext::instance()->config.redoxCalculationCValue;

        PoolControlContext::instance()->data.waterFlowSwitch = digitalRead(CONTROLLINO_I16) == HIGH ? true : false;
    }
};
