#include <Controllino.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMDNS.h>
// #include <EthernetWebServer.h>
#include "defines.h"

#define DEBUG 1

byte macAddress[6];
char macstr[18];

EthernetUDP Udp;
IPAddress multicastip(224, 0, 2, 60);
unsigned int multicastport = 50002;

EthernetUDP mDNSUdp;
MDNS mdns(mDNSUdp);

EthernetWebServer server(80);

int reqCount = 0; // number of requests received

void handleRoot()
{
#define BUFFER_SIZE 512

    char temp[BUFFER_SIZE];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    int day = hr / 24;

    hr = hr % 24;

    snprintf(temp, BUFFER_SIZE - 1,
             "<html>\
<head>\
<title>%s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from %s</h1>\
<h3>running EthernetWebServer</h3>\
<h3>on %s</h3>\
<p>Uptime: %d d %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>",
             BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60);

    server.send(200, F("text/html"), temp);
}

void handleNotFound()
{
    String message = F("File Not Found\n\n");

    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, F("text/plain"), message);
}

#define ORIGINAL_STR_LEN (2048 * MULTIPLY_FACTOR)

void drawGraph()
{
    static String out;
    static uint16_t previousStrLen = ORIGINAL_STR_LEN;

    if (out.length() == 0)
    {
        ET_LOGWARN1(F("String Len = 0, extend to"), ORIGINAL_STR_LEN);
        out.reserve(ORIGINAL_STR_LEN);
    }

    out = F("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n"
            "<circle cx=\"50\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />\n");
    out += F("</svg>\n");

    server.send(200, "image/svg+xml", out);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    // Random MAC address stored in EEPROM
    if (EEPROM.read(1) == '#')
    {
        for (int i = 2; i < 6; i++)
        {
            macAddress[i] = EEPROM.read(i);
        }
    }
    else
    {
        randomSeed(analogRead(0));
        for (int i = 2; i < 6; i++)
        {
            macAddress[i] = random(0, 255);
            EEPROM.write(i, macAddress[i]);
        }
        EEPROM.write(1, '#');
    }
    snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

    // Start up networking
    Ethernet.begin(macAddress);
    if (DEBUG)
    {
        Serial.print("DHCP (");
        Serial.print(macstr);
        Serial.print(")...");
        Serial.print("success: ");
        Serial.println(Ethernet.localIP());
    }
    if (Udp.beginMulticast(multicastip, multicastport))
    {
        Serial.println("Multicast address initialized");
    }

    mdns.begin(Ethernet.localIP(), "controllino");

    server.on(F("/"), handleRoot);
    server.on(F("/inline"), []()
              { server.send(200, F("text/plain"), F("This works as well")); });
    server.on(F("/test.svg"), drawGraph);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop()
{
    mdns.run();
    mdns.addServiceRecord("Controllino mDNS Webserver Example._http",
                          80,
                          MDNSServiceTCP);

    server.handleClient();

    // Recieve Packets
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        // receive incoming UDP packets
        Serial.print("Received packet :");
        char incomingPacket[255]{0};
        int len = Udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = 0;
        }
        Serial.print("UDP packet contents:");
        Serial.println(incomingPacket);
    }

    digitalWrite(CONTROLLINO_D5, HIGH);
    delay(500);
    digitalWrite(CONTROLLINO_D5, LOW);
    delay(500);
    char ReplyBuffer[] = "acknowledged";
    Udp.beginPacket(multicastip, multicastport);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
}
