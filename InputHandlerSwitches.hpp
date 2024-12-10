#pragma once

#include <Controllino.h>
#include <ArxContainer.h>
#include <AceButton.h>
#include <ArduinoJson.h>
#include <string.h>
#include "Identification.hpp"

#define CONTROLLINO_A16 CONTROLLINO_I16
#define CONTROLLINO_A17 CONTROLLINO_I17
#define CONTROLLINO_A18 CONTROLLINO_I18
#define CONTROLLINO_A19 CONTROLLINO_IN0
#define CONTROLLINO_A20 CONTROLLINO_IN1

using namespace ace_button;

namespace HC
{
    class Event
    {
    public:
        short inputButton;
        uint8_t buttonState;
        uint8_t eventType;
        int milliseconds;
    };
    static arx::vector<HC::Event *> events;

    class Compare
    {
    public:
        bool compare(const char *a, const char *b)
        {
            return strcmp(a, b) == 0;
        }
    };

    arx::map<const char *, short, Compare> trnslOutputs;
    arx::map<short, const char *> trnslInputs;

#define XDSTR(x) CONTROLLINO_D##x
#define XASTR(x) CONTROLLINO_A##x

#define STRINGIFY(x) #x
#define TOKENPASTE(x, y) STRINGIFY(x##y)

#define INIT_PINMODE_OUTPUT(x)                           \
    do                                                   \
    {                                                    \
        pinMode(XDSTR(x), OUTPUT);                       \
        trnslOutputs.insert(TOKENPASTE(D, x), XDSTR(x)); \
    } while (false)

#define INIT_PINMODE_INPUT(x)                           \
    do                                                  \
    {                                                   \
        pinMode(XDSTR(x), INPUT);                       \
        trnslInputs.insert(XASTR(x), TOKENPASTE(A, x)); \
        buttons[x].init(XASTR(x), LOW, x);              \
    } while (false)

    class InputHandlerSwitches
    {
    public:
        void setup()
        {
            INIT_PINMODE_OUTPUT(0);
            INIT_PINMODE_OUTPUT(1);
            INIT_PINMODE_OUTPUT(2);
            INIT_PINMODE_OUTPUT(3);
            INIT_PINMODE_OUTPUT(4);
            INIT_PINMODE_OUTPUT(5);
            INIT_PINMODE_OUTPUT(6);
            INIT_PINMODE_OUTPUT(7);
            INIT_PINMODE_OUTPUT(8);
            INIT_PINMODE_OUTPUT(9);
            INIT_PINMODE_OUTPUT(10);
            INIT_PINMODE_OUTPUT(11);
            INIT_PINMODE_OUTPUT(12);
            INIT_PINMODE_OUTPUT(13);
            INIT_PINMODE_OUTPUT(14);
            INIT_PINMODE_OUTPUT(15);
            INIT_PINMODE_OUTPUT(16);
            INIT_PINMODE_OUTPUT(17);
            INIT_PINMODE_OUTPUT(18);
            INIT_PINMODE_OUTPUT(19);
            INIT_PINMODE_OUTPUT(20);
            INIT_PINMODE_OUTPUT(21);
            INIT_PINMODE_OUTPUT(22);
            INIT_PINMODE_OUTPUT(23);

            INIT_PINMODE_INPUT(0);
            INIT_PINMODE_INPUT(1);
            INIT_PINMODE_INPUT(2);
            INIT_PINMODE_INPUT(3);
            INIT_PINMODE_INPUT(4);
            INIT_PINMODE_INPUT(5);
            INIT_PINMODE_INPUT(6);
            INIT_PINMODE_INPUT(7);
            INIT_PINMODE_INPUT(8);
            INIT_PINMODE_INPUT(9);
            INIT_PINMODE_INPUT(10);
            INIT_PINMODE_INPUT(11);
            INIT_PINMODE_INPUT(12);
            INIT_PINMODE_INPUT(13);
            INIT_PINMODE_INPUT(14);
            INIT_PINMODE_INPUT(15);
            INIT_PINMODE_INPUT(16);
            INIT_PINMODE_INPUT(17);
            INIT_PINMODE_INPUT(18);
            INIT_PINMODE_INPUT(19);
            INIT_PINMODE_INPUT(20);

            for (int idx = 0; idx <= 20; ++idx)
            {
                buttons[idx].setEventHandler(handleEvent);
            }
        }

        void run()
        {
            // LOGN("Runnig inputSwitchHandler");

            for (int idx = 0; idx <= 20; ++idx)
            {
                buttons[idx].check();
            }
            if (!events.size())
            {
                return;
            }
            LOG(F("We have an event. Q Size: "));
            LOGN(events.size());
            Event *event = events.front();
            int current = millis();
            if (event->milliseconds - current > 0)
            {
                bool coupleFound{false};
                JsonString virtualButton;
                int partner = 0; // switchConfig["c"][trnslInputs[event->inputButton]];
                for (const auto &kv : switchConfig["c"].as<JsonObject>())
                {
                    if (!kv.value().containsKey(trnslInputs[event->inputButton]))
                    {
                        break;
                    }
                    for (arx::map<short, const char *>::iterator t = trnslInputs.begin(); t != trnslInputs.end(); ++t)
                    {
                        if (strcmp(t->second, kv.value()[trnslInputs[event->inputButton]].as<const char *>()) == 0)
                        {
                            partner = t->first;
                        }
                    }
                    LOGN(kv.value()[trnslInputs[event->inputButton]].as<const char *>())
                    LOG("Found partner: ");
                    LOGN(partner);
                    virtualButton = kv.key();
                    coupleFound = true;
                }

                if (coupleFound) // switchConfig["c"].containsKey(trnslInputs[event->inputButton]))
                {
                    for (arx::vector<Event *>::iterator it = events.begin(); it != events.end(); ++it)
                    {
                        if (*it == event)
                        {
                            LOGN(F("We skip the first element"));
                            continue;
                        }

                        if ((*it)->inputButton == partner &&
                            (*it)->buttonState == event->buttonState &&
                            (*it)->eventType == event->eventType)
                        {
                            LOG(F("Rewrite button to: "));
                            LOGN(min((*it)->inputButton, event->inputButton) + 1000);
                            Event *p = *it;
                            events.erase(it);
                            event->inputButton = min(p->inputButton, event->inputButton) + 1000; // rewrite to virtual PIN
                            trnslInputs.insert(event->inputButton, virtualButton.c_str());
                            delete[] p;
                            break;
                        }
                    }
                }
                else
                {
                    LOGN(F("No coupled PIN"));
                }
            }

            if (event->milliseconds - current <= 0)
            {
                LOG("Trnl Eventnutton>: ");
                LOGN(trnslInputs[event->inputButton]);
                if (!switchConfig["a"][trnslInputs[event->inputButton]].size())
                {
                    LOG(F("No outputs found for button: "));
                    LOGN(event->inputButton);
                    events.pop_front();
                    delete[] event;
                    return;
                }
                LOG(F("Input "));
                LOG(event->inputButton);
                LOG(F(" - Output : "));
                for (uint32_t idx = 0; idx < switchConfig["a"][trnslInputs[event->inputButton]].size(); ++idx)
                {
                    JsonObject destination = switchConfig["a"][trnslInputs[event->inputButton]][idx];
                    LOGN(destination["d"].as<const char *>());
                    if (!ITSFORME(destination["d"].as<const char *>()))
                    {
                        StaticJsonDocument<128> sendToDestination;
                        sendToDestination["y"] = 3;
                        sendToDestination["a"] = event->buttonState;
                        sendToDestination["d"] = destination["d"].as<const char *>();
                        sendToDestination["t"] = destination["t"];
                        char switchConfigRaw[128]{0};
                        serializeJson(sendToDestination, switchConfigRaw, 128);
                        HC::multicastUdp.beginPacket(HC::multicastip, HC::multicastport);
                        HC::multicastUdp.write(switchConfigRaw, strlen(switchConfigRaw));
                        HC::multicastUdp.endPacket();
                        continue;
                    }
                    for (uint32_t j = 0; j < destination["t"].size(); ++j)
                    {
                        uint8_t button = trnslOutputs[destination["t"][j].as<const char *>()];
                        LOG(F("Output: "));
                        LOGN(destination["t"][j].as<const char *>());
                        digitalWrite(button, event->buttonState);
                    }
                    LOG(";");
                }
                LOG(F(" - buttonState: "));
                LOGN(event->buttonState);

                events.pop_front();
                delete[] event;
            }
        }
        void handleSwitchesMessage(JsonArray destination, uint8_t buttonState)
        {
            for (uint32_t j = 0; j < destination.size(); ++j)
            {
                uint8_t button = trnslOutputs[destination[j].as<const char *>()];
                LOG(F("Output: "));
                LOGN(destination[j].as<const char *>());
                digitalWrite(button, buttonState);
            }
        }

        static void handleEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
        {

            LOG(F("  button  "));
            LOG(button->getPin());
            LOG(F("  buttonstate  "));
            LOG(buttonState);
            LOG(F("  "));
            LOG(millis());
            LOGN((int)eventType);

            Event *event = new Event();
            event->inputButton = button->getPin();
            event->eventType = eventType;
            event->buttonState = buttonState;    // on OR off, RESP. 1 OR 0
            event->milliseconds = millis() + 30; // End of life, action required
            events.push_back(event);
        }
        static const uint8_t NUM_INPUTS{21};
        AceButton buttons[NUM_INPUTS];
        StaticJsonDocument<1024> switchConfig;
    };
}