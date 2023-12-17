#pragma once
#include <Ethernet.h>
#include "InputHandlerSwitches.hpp"
#include "Eeprom.hpp"
namespace HC
{
    EthernetServer server(80);

    enum class ParseState
    {
        REQMETHOD,
        REQURI,
        RESTHEAD,
        HEADLINE,
        BODY
    };

    class WebServer
    {
    public:
        HC::InputHandlerSwitches *ihs;
        HC::Eeprom *eeprom;

        void setup(HC::InputHandlerSwitches *inputHandlerSwitches, HC::Eeprom *eeprom)
        {
            server.begin();
            ihs = inputHandlerSwitches;
            this->eeprom = eeprom;
        }

        void handleEditor(EthernetClient client)
        {
            char switchConfigRaw[512]{0};
            serializeJson(ihs->switchConfig, switchConfigRaw, 512);
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/html"));
            client.println(F("Connection: close")); // the connection will be closed after completion of the response
            client.println();
            client.println(F("<html lang=\"en\">"));
            client.println(F("<head><link rel=\"icon\" href=\"data:,\">"));
            client.println(F("<meta charset=\"utf-8\">"));
            client.println(F("<title>Config</title>"));
            client.println(F("<link href=\"https://cdn.jsdelivr.net/npm/jsoneditor@8.6.3/dist/jsoneditor.css\" rel=\"stylesheet\" type=\"text/css\">"));
            client.println(F("<script src=\"https://cdn.jsdelivr.net/npm/jsoneditor@8.6.3/dist/jsoneditor.js\"></script>"));
            client.println(F("<style type=\"text/css\">"));
            client.println(F("#jsoneditor {"));
            client.println(F("width: 500px;"));
            client.println(F("height: 500px;"));
            client.println(F("}"));
            client.println(F("</style>"));
            client.println(F("</head>"));
            client.println(F("<body>"));
            client.println(F("<p>"));
            client.println(F("<button id=\"getJSON\">Apply</button>"));
            client.println(F("</p>"));
            client.println(F("<div id=\"jsoneditor\"></div>"));
            client.println(F("<script>"));
            client.println(F("const container = document.getElementById('jsoneditor')"));
            client.println(F("const options = {}"));
            client.println(F("const editor = new JSONEditor(container, options)")); // editor.set(json)
            client.print(F("const json = "));
            client.println(switchConfigRaw);
            client.println(F("editor.set(json)"));
            client.println(F("document.getElementById('getJSON').onclick = function () {"));
            client.println(F("    var xhr = new XMLHttpRequest();"));
            client.print(F("    var url = \"http://"));
            client.print(mdnsName);
            client.print(F(".local"));
            client.println(F("\";"));
            client.println(F("    xhr.open(\"POST\", url, true);"));
            client.println(F("    xhr.setRequestHeader(\"Content-Type\", \"application/json\");"));
            client.println(F("    xhr.onreadystatechange = function () {"));
            client.println(F("      if (xhr.readyState === 4 && xhr.status === 200) {"));
            client.println(F("        alert(\"Applied.\")"));
            client.println(F("      }"));
            client.println(F("    };"));
            client.println(F("    var data = JSON.stringify(json);"));
            client.println(F("    xhr.send(data);"));
            client.println(F("}"));
            client.println(F("</script>"));
            client.println(F("</body>"));
            client.println(F("</html>"));
        }

        void handlePost(EthernetClient client)
        {
            bool end = false;
            ParseState ps = ParseState::HEADLINE;
            char parseBuf[512]{0};
            int pBidx{0};

            int length = 0;
            while (!end)
            {
                char c = client.read();
                parseBuf[pBidx] = c;
                ++pBidx;
                switch (ps)
                {
                case ParseState::HEADLINE:
                {
                    if (strncmp(parseBuf, "\r\n", 2) == 0)
                    {
                        ps = ParseState::BODY;
                        memset(parseBuf, 0, 512);
                        pBidx = 0;
                        break;
                    }
                    if (c == '\n')
                    {
                        if (strncmp(parseBuf, "Content-Length", 14) == 0)
                        {
                            for (int i = 0; i < 512; ++i)
                            {
                                if (parseBuf[i] == ':')
                                {
                                    length = atoi(&parseBuf[i + 1]);
                                    break;
                                }
                            }
                        }
                        memset(parseBuf, 0, 512);
                        pBidx = 0;
                    }
                    break;
                }
                case ParseState::BODY:
                {
                    --length;
                    if (!length)
                    {
                        // we read the entire body
                        client.println(F("HTTP/1.1 200 OK"));
                        client.println(F("Connection: close")); // the connection will be closed after completion of the response
                        end = true;
                    }
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
            LOGN(parseBuf);
            eeprom->writeConfig(parseBuf, strlen(parseBuf));
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
                char parseBuf[512]{0};
                int pBidx{0};
                // String parseBuf;
                ParseState s = ParseState::REQMETHOD;
                while (client.connected())
                {
                    if (client.available())
                    {
                        char c = client.read();
                        parseBuf[pBidx] = c;
                        ++pBidx;

                        switch (s)
                        {
                        case ParseState::REQMETHOD:
                        {
                            if (strncmp(parseBuf, "POST ", 5) == 0)
                            {
                                LOGN(F("Found POST"));
                                handlePost(client);
                                client.stop();
                                LOGN(F("client disconnected"));
                            }
                            else if (strncmp(parseBuf, "GET ", 4) == 0)
                            {
                                LOGN(F("Found GET"));
                                memset(parseBuf, 0, 512);
                                pBidx = 0;
                                s = ParseState::REQURI;
                            }
                            break;
                        }
                        case ParseState::REQURI:
                        {
                            if (c == ' ')
                            {
                                LOGN(F("Found Re-Line:"));
                                LOG(parseBuf);
                                LOGN(".");
                                if (strncmp(&parseBuf[pBidx - 7], "editor ", 5) == 0) // parseBuf.endsWith(F("editor ")))
                                {
                                    handleEditor(client);
                                    client.stop();
                                    LOGN(F("client disconnected"));
                                }
                                s = ParseState::RESTHEAD;
                            }
                            break;
                        }
                        case ParseState::RESTHEAD:
                        {
                            // Serial.write(c);
                            //  if you've gotten to the end of the line (received a newline
                            //  character) and the line is blank, the http request has ended,
                            //  so you can send a reply
                            if (c == '\n' && currentLineIsBlank)
                            {
                                // send a standard http response header
                                LOGN(F("Send standard"));
                                client.println(F("HTTP/1.1 200 OK"));
                                client.println(F("Content-Type: text/html"));
                                client.println(F("Connection: close")); // the connection will be closed after completion of the response
                                client.println();
                                client.println(F("<html><head><link rel=\"icon\" href=\"data:,\"></head><body>"));
                                client.println(F("<button type=\"button\" onclick=\"alert('Hello sworld!')\">Click Me!</button>"));
                                client.println(F("</body></html>"));
                                // close the connection:
                                client.stop();
                                LOGN("client disconnected");
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
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                }
            }
        }
    };
}