#pragma once
#include <Ethernet.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "Eeprom.hpp"
#include "RealTimeClock.hpp"
#include <EthernetUdp.h>

// EthernetUDP Udp;

#include <avr/wdt.h>
// void reboot()
// {
//     // pinMode(47, OUTPUT);
//     // digitalWrite(CONTROLLINO_R4, HIGH);
//     wdt_disable();
//     wdt_enable(WDTO_15MS);
//     while (1)
//     {
//     }
// }

namespace HC
{
    EthernetServer server(80);

    enum class ParseState : uint8_t
    {
        REQMETHOD,
        REQURI,
        RESTHEAD,
        HEADLINE,
        BODY
    };

    enum class METHOD : uint8_t
    {
        GET,
        POST
    };

    enum class SERVICE : uint8_t
    {
        NOSERVICE,
        EDITOR,
        TOGGLE
    };

    class WebServer
    {
    public:
        HC::Eeprom *eeprom;
        bool doReboot{false};

        void setup(HC::Eeprom *eeprom)
        {
            server.begin();
            this->eeprom = eeprom;
            Udp.begin(13000);
            pinMode(CONTROLLINO_R4, OUTPUT);
        }

        void handleConfigGet(EthernetClient client)
        {
            bool end{false};
            bool endDetection{false};
            char buf[4]{0};
            char *pBuf = buf;
            while (!end)
            {
                char c = client.read();
                LOG(c);

                if (c == '\r' || c == '\n')
                {
                    endDetection = true;
                }
                else
                {
                    endDetection = false;
                    memset(buf, 0, 4);
                    pBuf = buf;
                }

                if (endDetection)
                {
                    *pBuf = c;
                    ++pBuf;
                }

                if (strncmp(buf, "\r\n\r\n", 4) == 0)
                {
                    end = true;
                }
            }

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
            client.println(PoolControlContext::instance()->config.switchConfigRaw);
            client.println(F("editor.set(json)"));
            client.println(F("document.getElementById('getJSON').onclick = function () {"));
            client.println(F("    var xhr = new XMLHttpRequest();"));
            client.print(F("    var url = \"http://"));
            client.print(PoolControlContext::instance()->data.clientIP);
            client.print(F("/config"));
            client.println(F("\";"));
            client.println(F("    xhr.open(\"POST\", url, true);"));
            client.println(F("    xhr.setRequestHeader(\"Content-Type\", \"application/json\");"));
            client.println(F("    xhr.onreadystatechange = function () {"));
            client.println(F("      if (xhr.readyState === 4 && xhr.status === 200) {"));
            client.println(F("        alert(\"Applied.\")"));
            client.println(F("      }"));
            client.println(F("    };"));
            client.println(F("    const setting = editor.get();"));
            client.println(F("    var data = JSON.stringify(setting);"));
            client.println(F("    xhr.send(data);"));
            client.println(F("}"));
            client.println(F("</script>"));
            client.println(F("</body>"));
            client.println(F("</html>"));
            client.flush();
        }

        void handleDataGet(EthernetClient client)
        {
            bool end{false};
            bool endDetection{false};
            char buf[4]{0};
            char *pBuf = buf;
            char memory[1024]{0};
            while (!end)
            {
                char c = client.read();
                if (c == '\r' || c == '\n')
                {
                    endDetection = true;
                }
                else
                {
                    endDetection = false;
                    memset(buf, 0, 4);
                    pBuf = buf;
                }

                if (endDetection)
                {
                    *pBuf = c;
                    ++pBuf;
                }

                if (strncmp(buf, "\r\n\r\n", 4) == 0)
                {
                    end = true;
                }
            }
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/html"));
            client.println(F("Connection: close")); // the connection will be closed after completion of the response
            client.println();
            client.println(F("<html><head><link rel=\"icon\" href=\"data:,\"></head><body><table></table>"));
            client.println(F("</body></html>"));
            client.println(F("<script>"));
            client.print(F("let json = '"));
            memset(memory, 0, 1024);
            Networking::getSensorReadings(memory, 1024);
            client.print(memory);
            client.println(F("'"));
            client.println(F("var data = JSON.parse(json);"));
            client.println(F("var table = document.querySelector('table');"));
            client.println(F("var rows = '';"));
            client.println(F("for (var p in data) {"));
            client.println(F("rows += '<tr><td>' + p + '</td><td>' + data[p] + '</td></tr>' }"));
            client.println(F("table.innerHTML = rows;"));
            client.println(F("</script>"));
            client.println(F("</body>"));
            client.println(F("</html>"));
            client.flush();
        }

#define TMPMEM_SIZE 650

        void handlePost(EthernetClient client, SERVICE service)
        {
            bool end = false;
            ParseState ps = ParseState::HEADLINE;
            char tmpMem[TMPMEM_SIZE]{0};
            memset(tmpMem, 0, TMPMEM_SIZE);
            int pBidx{0};

            int length = 0;
            while (!end)
            {
                char c = client.read();
                tmpMem[pBidx] = c;
                ++pBidx;
                switch (ps)
                {
                case ParseState::HEADLINE:
                {
                    if (strncmp(tmpMem, "\r\n", 2) == 0)
                    {
                        ps = ParseState::BODY;
                        memset(tmpMem, 0, TMPMEM_SIZE);
                        pBidx = 0;
                        break;
                    }
                    if (c == '\n')
                    {
                        if (strncmp(tmpMem, "Content-Length", 14) == 0)
                        {
                            for (int i = 0; i < TMPMEM_SIZE; ++i)
                            {
                                if (tmpMem[i] == ':')
                                {
                                    length = atoi(&tmpMem[i + 1]);
                                    break;
                                }
                            }
                        }
                        memset(tmpMem, 0, TMPMEM_SIZE);
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
            LOGN(tmpMem);
            if (service == SERVICE::EDITOR)
            {
                JsonDocument tmp;
                DeserializationError err = deserializeJson(tmp, (const char *)tmpMem);
                LOGN(err.c_str());
                if (err == DeserializationError::Ok)
                {
                    LOGN(F("Writing config."));
                    eeprom->writeConfig(tmpMem, strlen(tmpMem));
                    doReboot = true;
                }
                else
                {
                    LOGN(F("Cannot deserialize data"));
                }
            }
            if (service == SERVICE::TOGGLE)
            {
                // DynamicJsonDocument tmp(32);
                // if (deserializeJson(tmp, tmpMem) == DeserializationError::Ok)
                // {
                //     digitalWrite(trnslOutputs[tmp["toggle"]], !digitalRead(trnslOutputs[tmp["toggle"]]));
                // }
            }
        }

        void run()
        {
            // listen for incoming clients
            EthernetClient client = server.available();
            if (client)
            {
                LOGN(F("new client"));
                // an http request ends with a blank line
                boolean currentLineIsBlank = true;
                char tmpMem[64]{0};
                int pBidx{0};
                // String parseBuf;
                ParseState s = ParseState::REQMETHOD;
                METHOD method = METHOD::GET;
                SERVICE service = SERVICE::NOSERVICE;
                while (client.connected())
                {
                    if (client.available())
                    {
                        char c = client.read();
                        LOG(c);
                        tmpMem[pBidx] = c;
                        ++pBidx;

                        switch (s)
                        {
                        case ParseState::REQMETHOD:
                        {
                            if (strncmp(tmpMem, "POST ", 5) == 0)
                            {
                                LOGN(F("Found POST"));
                                method = METHOD::POST;
                                pBidx = 0;
                                s = ParseState::REQURI;
                            }
                            else if (strncmp(tmpMem, "GET ", 4) == 0)
                            {
                                LOGN(F("Found GET"));
                                method = METHOD::GET;
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
                                LOG(tmpMem);
                                if (strncmp(&tmpMem[pBidx - 7], "config ", 6) == 0)
                                {
                                    if (method == METHOD::GET)
                                    {
                                        handleConfigGet(client);
                                    }
                                    else if (method == METHOD::POST)
                                    {
                                        service = SERVICE::EDITOR;
                                        handlePost(client, service);
                                    }
                                    client.stop();
                                    client.clearWriteError();
                                    LOGN(F("client disconnected"));
                                }
                                if (strncmp(&tmpMem[pBidx - 5], "data ", 4) == 0)
                                {
                                    if (method == METHOD::GET)
                                    {
                                        handleDataGet(client);
                                    }
                                    else if (method == METHOD::POST)
                                    {
                                        service = SERVICE::TOGGLE;
                                        // handlePost(client, service);
                                    }
                                    client.stop();
                                    client.clearWriteError();
                                    LOGN(F("client disconnected"));
                                }
                                if (strncmp(&tmpMem[pBidx - 7], "reboot ", 6) == 0)
                                {
                                    doReboot = true;
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
                            pBidx = 0;
                            if (c == '\n' && currentLineIsBlank)
                            {
                                // send a standard http response header
                                LOGN(F("Send standard"));
                                client.println(F("HTTP/1.1 200 OK"));
                                client.println(F("Content-Type: text/html"));
                                client.println(F("Connection: close")); // the connection will be closed after completion of the response
                                client.println();
                                client.println(F("<html><head><link rel=\"icon\" href=\"data:,\"></head><body>"));
                                client.println(F("No site."));
                                client.println(F("</body></html>"));
                                // close the connection:
                                client.stop();
                                LOGN(F("client disconnected"));
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