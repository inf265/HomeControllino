#include <Controllino.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMDNS.h>

#define DEBUG 1

byte mac[6];
char macstr[18];

EthernetUDP Udp;
IPAddress multicastip(224, 0, 2, 60);
unsigned int multicastport = 50002;

EthernetUDP mDNSUdp;
MDNS mdns(mDNSUdp);

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
            mac[i] = EEPROM.read(i);
        }
    }
    else
    {
        randomSeed(analogRead(0));
        for (int i = 2; i < 6; i++)
        {
            mac[i] = random(0, 255);
            EEPROM.write(i, mac[i]);
        }
        EEPROM.write(1, '#');
    }
    snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Start up networking
    Ethernet.begin(mac);
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
}

void loop()
{
    mdns.run();
    mdns.addServiceRecord("Controllino mDNS Webserver Example._http",
                          80,
                          MDNSServiceTCP);
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
