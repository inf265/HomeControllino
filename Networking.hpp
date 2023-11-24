#pragma once

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMDNS.h>

namespace HC
{
    EthernetUDP multicastUdp;
    IPAddress multicastip(224, 0, 0, 42);
    unsigned int multicastport = 4242;

    EthernetUDP mDNSUdp;
    MDNS mdns(mDNSUdp);

    class Networking
    {
    public:
        void setup(uint8_t *mac)
        {
            // Start up networking
            Ethernet.begin(mac);
            if (1)
            {
                LOG(F("DHCP ("));
                LOG(F(")..."));
                LOG(F("success: "));
                LOGN(Ethernet.localIP());
            }

            if (multicastUdp.beginMulticast(multicastip, multicastport))
            {
                LOGN(F("Multicast address initialized"));
            }
            char macstr[21];
            memset(macstr, 0, 21);
            snprintf(macstr, 21, "controllino-%02x-%02x-%02x", mac[3], mac[4], mac[5]);
            if (mdns.begin(Ethernet.localIP(), macstr))
            {
                LOGN("MDNS initialized");
            }

            mdns.addServiceRecord("Controllino mDNS Webserver._http",
                                  80,
                                  MDNSServiceTCP);
        }

        void run()
        {
            mdns.run();

            packetSize = multicastUdp.parsePacket();
            if (packetSize)
            {
                // receive incoming UDP packets
                LOG(F("Received packet :"));
                int len = multicastUdp.read(incomingPacket, 2048);
                if (len > 0)
                {
                    incomingPacket[len] = 0;
                }
                LOG(F("UDP packet contents:"));
                LOGN(incomingPacket);
            }
        }

        char incomingPacket[2048]{0};
        int packetSize{0};
    };
}