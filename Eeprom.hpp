#pragma once

#include <stdint.h>
#include <TrueRandom.h>
#include <EEPROM.h>

namespace HC
{
    static const uint32_t CONFIG_OFFSET = 100;

    class Eeprom
    {
    public:
        void setup()
        {
            if (EEPROM.read(0) == '#')
            {
                for (int i = 0; i < 6; i++)
                {
                    macAddress[i] = EEPROM.read(i + 1);
                }
            }
            // Random MAC address stored in EEPROM
            // for (int i = 0; i < 6; i++)
            // {
            //     EEPROM.write(i + 1, 0);
            //     EEPROM.write(i + 1, macAddress[i]);
            // }
            // EEPROM.write(0, '#');
            snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
            LOG("MAC: ");
            LOGN(macstr);
        }

        void writeConfig(const char *data, uint32_t size)
        {
            for (uint32_t i = 0; i < size; ++i)
            {
                LOG((char)data[i]);
                EEPROM.write(CONFIG_OFFSET + i, (uint8_t)data[i]);
            }
        }

        void eraseConfig()
        {
            EEPROM.write(CONFIG_OFFSET, 0);
        }

        uint8_t readConfig(char *data, uint32_t size)
        {
            uint32_t idxCnt{0};
            uint32_t jsonCnt{0};
            memset(data, 0, size);

            while (idxCnt < size)
            {
                data[idxCnt] = EEPROM.read(CONFIG_OFFSET + idxCnt);
                // LOGN((char)data[idxCnt]);
                // LOGN(jsonCnt);
                if (idxCnt == 0 && data[idxCnt] != '{')
                {
                    return 1; // failure
                }
                if (data[idxCnt] == '{')
                {
                    LOGN(idxCnt);
                    ++jsonCnt;
                }
                else if (data[idxCnt] == '}')
                {
                    --jsonCnt;
                }
                if (jsonCnt == 0)
                {
                    LOGN(F("Json End"));
                    return 0; // good case
                    break;
                }
                ++idxCnt;
            }
            if (jsonCnt > 0)
            {
                EEPROM.write(CONFIG_OFFSET, 0);
                return 2; // failure
            }
            return 0; // good case, last character was the bracket }
        }

        uint8_t macAddress[6]{0}; // {0x00, 0xfa, 0x0e, 0x86, 0x96, 0xa4};
        char macstr[18]{0};
    };
}