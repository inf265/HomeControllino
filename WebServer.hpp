#pragma once
#include <Ethernet.h>
#include "InputHandlerSwitches.hpp"
namespace HC
{
    EthernetServer server(80);

    class WebServer
    {
    public:
        HC::InputHandlerSwitches *ihs;

        void setup(HC::InputHandlerSwitches *inputHandlerSwitches)
        {
            server.begin();
            ihs = inputHandlerSwitches;
        }

        void run()
        {
            // listen for incoming clients
            EthernetClient client = server.available();
            if (client)
            {
                LOGN("new client");
                // an http request ends with a blank line
                boolean currentLineIsBlank = true;
                while (client.connected())
                {
                    if (client.available())
                    {
                        char c = client.read();
                        // Serial.write(c);
                        //  if you've gotten to the end of the line (received a newline
                        //  character) and the line is blank, the http request has ended,
                        //  so you can send a reply
                        if (c == '\n' && currentLineIsBlank)
                        {
                            // send a standard http response header
                            client.println(F("HTTP/1.1 200 OK"));
                            client.println(F("Content-Type: application/json"));
                            client.println(F("Connection: close")); // the connection will be closed after completion of the response
                            client.println();
                            char switchConfigRaw[512]{0};
                            serializeJson(ihs->switchConfig, switchConfigRaw, 512);
                            client.println(switchConfigRaw);
                            break;
                        }
                        if (c == '\n')
                        {
                            // you're starting a new line
                            currentLineIsBlank = true;
                        }
                        else if (c != '\r')
                        {
                            // you've gotten a character on the current line
                            currentLineIsBlank = false;
                        }
                    }
                }
                // close the connection:
                client.stop();
                LOGN("client disconnected");
            }
        }
    };
}