#pragma once
#include <Arduino.h>
#include "PoolControl_Config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "PoolControlContext.hpp"

#define ONE_WIRE_BUS CONTROLLINO_D7

enum class TemperatureSensor : uint8_t
{
    TEMPERATURE_HOUSING,
    TEMPERATURE_WATER
};

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemperature(&oneWire);

class TemperatureSensors
{
public:
    void init()
    {
    }
    void run()
    {
        retrieveDataFromTemperatureSensors();
        PoolControlContext::instance()->data.housingTemperature = (float)dallasTemperature.getTempCByIndex(1);
        PoolControlContext::instance()->data.waterTemperature = (float)dallasTemperature.getTempCByIndex(0);
    }

    void scanSensorIds()
    {
    }

    String getTemperature(TemperatureSensor temperatureSensor)
    {
        switch (temperatureSensor)
        {
        case TemperatureSensor::TEMPERATURE_HOUSING:
        {
            return String(PoolControlContext::instance()->data.housingTemperature);
        }
        case TemperatureSensor::TEMPERATURE_WATER:
        {
            return String(PoolControlContext::instance()->data.waterTemperature);
        }
        default:
        {
            return STR_UNAVAILABLE;
            break;
        }
        }
    }

    void assignSensorId()
    {
    }

private:
    // std::shared_ptr<OneWire> oneWire;
    // std::shared_ptr<DallasTemperature> dallasTemperature;

    void retrieveDataFromTemperatureSensors()
    {
        dallasTemperature.requestTemperatures();
    }
};