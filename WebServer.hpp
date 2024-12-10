#pragma once
#include <Ethernet.h>
#include "InputHandlerSwitches.hpp"
#include "Eeprom.hpp"
namespace HC
{
    EthernetServer server(80);
    char tmpMem[1024]{0};

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
        HC::InputHandlerSwitches *ihs;
        HC::Eeprom *eeprom;

        void setup(HC::InputHandlerSwitches *inputHandlerSwitches, HC::Eeprom *eeprom)
        {
            server.begin();
            ihs = inputHandlerSwitches;
            this->eeprom = eeprom;
        }

        void handleEditorGet(EthernetClient client)
        {
            bool end{false};
            bool endDetection{false};
            char buf[4]{0};
            char *pBuf = buf;
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
            memset(tmpMem, 0, 1024);
            serializeJson(ihs->switchConfig, tmpMem, 1024);
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
            client.println(F("<a href=\"http://"));
            client.println(Ethernet.localIP());
            client.println(F("/editor\">Switch to IP if you're on DNS name</a><br/>"));
            client.println(F("<p>"));
            client.println(F("<button id=\"getJSON\">Apply</button>"));
            client.println(F("</p>"));
            client.println(F("<div id=\"jsoneditor\"></div>"));
            client.println(F("<script>"));
            client.println(F("const container = document.getElementById('jsoneditor')"));
            client.println(F("const options = {}"));
            client.println(F("const editor = new JSONEditor(container, options)")); // editor.set(json)
            client.print(F("const json = "));
            client.println(tmpMem);
            client.println(F("editor.set(json)"));
            client.println(F("document.getElementById('getJSON').onclick = function () {"));
            client.println(F("    var xhr = new XMLHttpRequest();"));
            client.print(F("    var url = \"http://"));
            client.print(Ethernet.localIP());
            client.print(F("/editor"));
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

        void handleToggleGet(EthernetClient client)
        {
            bool end{false};
            bool endDetection{false};
            char buf[4]{0};
            char *pBuf = buf;
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
            memset(tmpMem, 0, 1024);
            serializeJson(ihs->switchConfig, tmpMem, 1024);
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/html"));
            client.println(F("Connection: close")); // the connection will be closed after completion of the response
            client.println();
            client.println(F("<html><head><link rel=\"icon\" href=\"data:,\"></head><body>"));
            client.println(F("<a href=\"http://"));
            client.println(Ethernet.localIP());
            client.println(F("/toggle\">Switch to IP if you're on DNS name</a><br/>"));
            for (arx::map<const char *, short, Compare>::iterator it = trnslOutputs.begin(); it != trnslOutputs.end(); ++it)
            {
                client.print(F("<button type=\"button\" onclick=\"toggle('"));
                client.print(it->first);
                client.print(F("')\">"));
                client.print(it->first);
                client.println(F("</button>"));
            }
            client.println(F("</body></html>"));
            client.println(F("<script>"));
            client.println(F("function toggle(output) {"));
            client.println(F("var xhr = new XMLHttpRequest();"));
            client.print(F("var url = \"http://"));
            client.print(Ethernet.localIP());
            client.print(F("/toggle"));
            client.println(F("\";"));
            client.println(F("xhr.open(\"POST\", url, true);"));
            client.println(F("xhr.setRequestHeader(\"Content-Type\", \"application/json\");"));
            client.println(F("xhr.onreadystatechange = function () {"));
            client.println(F("if (xhr.readyState === 4 && xhr.status === 200) {"));
            client.println(F("console.log(\"Applied.\")"));
            client.println(F("}"));
            client.println(F("};"));
            client.println(F("const json = { toggle : output }"));
            client.println(F("var data = JSON.stringify(json);"));
            client.println(F("xhr.send(data);"));
            client.println(F("}"));
            client.println(F("</script>"));
            client.println(F("</body>"));
            client.println(F("</html>"));
            client.flush();
        }

        void handlePost(EthernetClient client, SERVICE service)
        {
            bool end = false;
            ParseState ps = ParseState::HEADLINE;
            memset(tmpMem, 0, 1024);
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
                        memset(tmpMem, 0, 1024);
                        pBidx = 0;
                        break;
                    }
                    if (c == '\n')
                    {
                        if (strncmp(tmpMem, "Content-Length", 14) == 0)
                        {
                            for (int i = 0; i < 1024; ++i)
                            {
                                if (tmpMem[i] == ':')
                                {
                                    length = atoi(&tmpMem[i + 1]);
                                    break;
                                }
                            }
                        }
                        memset(tmpMem, 0, 1024);
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
                eeprom->writeConfig(tmpMem, strlen(tmpMem));
                if (deserializeJson(ihs->switchConfig, (const char *)tmpMem) != DeserializationError::Ok)
                {
                    eeprom->eraseConfig();
                }
            }
            if (service == SERVICE::TOGGLE)
            {
                DynamicJsonDocument tmp(32);
                if (deserializeJson(tmp, tmpMem) == DeserializationError::Ok)
                {
                    digitalWrite(trnslOutputs[tmp["toggle"]], !digitalRead(trnslOutputs[tmp["toggle"]]));
                }
            }
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
                memset(tmpMem, 0, 1024);
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
                                memset(tmpMem, 0, 1024);
                                pBidx = 0;
                                s = ParseState::REQURI;
                            }
                            else if (strncmp(tmpMem, "GET ", 4) == 0)
                            {
                                LOGN(F("Found GET"));
                                method = METHOD::GET;
                                memset(tmpMem, 0, 1024);
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
                                LOGN(".");
                                if (strncmp(&tmpMem[pBidx - 7], "editor ", 5) == 0)
                                {
                                    if (method == METHOD::GET)
                                    {
                                        handleEditorGet(client);
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
                                if (strncmp(&tmpMem[pBidx - 7], "toggle ", 5) == 0)
                                {
                                    if (method == METHOD::GET)
                                    {
                                        handleToggleGet(client);
                                    }
                                    else if (method == METHOD::POST)
                                    {
                                        service = SERVICE::TOGGLE;
                                        handlePost(client, service);
                                    }
                                    client.stop();
                                    client.clearWriteError();
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