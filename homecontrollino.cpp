#include <Controllino.h>
#include "Logging.hpp"
#include "Serial.hpp"
#include "Networking.hpp"
#include "WebServer.hpp"
#include "Eeprom.hpp"
#include "InputHandlerSwitches.hpp"
#include "Identification.hpp"
#include "ArduinoJson.h"

HC::Serial serial;
HC::Eeprom eeprom;
HC::Networking networking;
HC::WebServer webserver;
HC::InputHandlerSwitches inputHandlerSwitches;

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

char switchConfigRaw[1024]{0};

void setup()
{
    serial.setup();
    eeprom.setup();
    if (!eeprom.readConfig(switchConfigRaw, 1024))
    {
        LOGN(F("Read Config"));
        LOGN(switchConfigRaw);
        if (deserializeJson(inputHandlerSwitches.switchConfig, (const char *)switchConfigRaw) != DeserializationError::Ok)
        {
            eeprom.eraseConfig();
        }
        else
        {
            if (!inputHandlerSwitches.switchConfig.containsKey("a"))
            {
                eeprom.eraseConfig();
            }
        }
    }
    else
    {
        LOGN(F("No Config present"));
    }
    String ipaddress = "";
    if (inputHandlerSwitches.switchConfig.containsKey("ipaddress"))
    {
        ipaddress = inputHandlerSwitches.switchConfig["ipaddress"].as<String>();
    }
    networking.setup(eeprom.macAddress, ipaddress);
    webserver.setup(&inputHandlerSwitches, &eeprom);
    inputHandlerSwitches.setup();

    snprintf(myId, 9, "%02x-%02x-%02x", eeprom.macAddress[3], eeprom.macAddress[4], eeprom.macAddress[5]);
}

enum class Type : uint8_t
{
    SWITCHCONFIG = 0,
    ERASESWITCHCONFIG = 1,
    BCASTCONFIG = 2,
    SWITCHTODESTINATION = 3
};

void loop()
{
    networking.run();
    webserver.run();
    Ethernet.maintain();

    if (networking.packetSize && networking.incomingPacket[0] == '{') // assume it is json
    {
        DynamicJsonDocument envelope(1024);
        if (deserializeJson(envelope, networking.incomingPacket) == DeserializationError::Ok)
        {
            if (ITSFORME(envelope["d"]))
            {
                switch (envelope["y"].as<uint8_t>())
                {
                case static_cast<uint8_t>(Type::SWITCHCONFIG):
                {
                    memset(switchConfigRaw, 0, 1024);
                    serializeJson(envelope["v"], switchConfigRaw, 1024);
                    eeprom.writeConfig(switchConfigRaw, strlen(switchConfigRaw));
                    if (deserializeJson(inputHandlerSwitches.switchConfig, (const char *)switchConfigRaw) != DeserializationError::Ok)
                    {
                        eeprom.eraseConfig();
                    }
                    break;
                }
                case static_cast<uint8_t>(Type::ERASESWITCHCONFIG):
                {
                    eeprom.eraseConfig();
                    inputHandlerSwitches.switchConfig.clear();
                    break;
                }
                case static_cast<uint8_t>(Type::BCASTCONFIG):
                {
                    serializeJson(inputHandlerSwitches.switchConfig, switchConfigRaw, 1024);
                    HC::multicastUdp.beginPacket(HC::multicastip, HC::multicastport);
                    HC::multicastUdp.write(switchConfigRaw, strlen(switchConfigRaw));
                    HC::multicastUdp.endPacket();
                    break;
                }
                case static_cast<uint8_t>(Type::SWITCHTODESTINATION):
                {
                    inputHandlerSwitches.handleSwitchesMessage(envelope["t"].as<JsonArray>(), envelope["a"].as<uint8_t>());
                    break;
                }
                default:
                {
                    LOGN(F("Message will not be processed"));
                }
                }
            }
            else
            {
                LOGN(F("Message is not for me"));
            }
        }
    }

    if (!inputHandlerSwitches.switchConfig.isNull() &&
        inputHandlerSwitches.switchConfig.is<JsonObject>())
    {
        inputHandlerSwitches.run();
    }

    // LOGN(freeRam());
    // delay(500);
}
