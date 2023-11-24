#pragma once
#include <Controllino.h>

namespace HC
{
    class Serial
    {
    public:
        void setup()
        {
            ::Serial.begin(115200);
            while (!::Serial && !::Serial.available())
            {
            }
        }
    };
}