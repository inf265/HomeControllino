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

void setup()
{
    serial.setup();
    eeprom.setup();
    networking.setup(eeprom.macAddress);
    webserver.setup(&inputHandlerSwitches, &eeprom);
    inputHandlerSwitches.setup();

    char tmp[512]{0};
    if (!eeprom.readConfig(tmp, 512))
    {
        LOGN(F("Read Config"));
        LOGN(tmp);
        if (deserializeJson(inputHandlerSwitches.switchConfig, (const char *)tmp) != DeserializationError::Ok)
        {
            eeprom.eraseConfig();
        }
    }
    else
    {
        LOGN(F("No Config present"));
    }
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

    if (networking.packetSize && networking.incomingPacket[0] == '{') // assume it is json
    {
        StaticJsonDocument<2048> envelope;
        if (deserializeJson(envelope, networking.incomingPacket) == DeserializationError::Ok)
        {
            if (ITSFORME(envelope["d"]))
            {
                switch (envelope["y"].as<uint8_t>())
                {
                case static_cast<uint8_t>(Type::SWITCHCONFIG):
                {
                    char switchConfigRaw[512]{0};
                    serializeJson(envelope["v"], switchConfigRaw, 512);
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
                    char switchConfigRaw[512]{0};
                    serializeJson(inputHandlerSwitches.switchConfig, switchConfigRaw, 512);
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
    // digitalWrite(CONTROLLINO_D5, HIGH);
    // delay(200);
    // digitalWrite(CONTROLLINO_D5, LOW);
    // delay(200);
}
