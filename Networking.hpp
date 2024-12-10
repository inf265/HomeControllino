#pragma once

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMDNS.h>
#include "Identification.hpp"

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
        void setup(uint8_t *mac, String &ipaddress)
        {
            if (ipaddress == "")
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
            }
            else
            {
                IPAddress ipa;
                ipa.fromString(ipaddress);
                Ethernet.begin(mac, ipa);
                LOG(F("IP static: "));
                LOGN(Ethernet.localIP());
            }
            if (multicastUdp.beginMulticast(multicastip, multicastport))
            {
                LOGN(F("Multicast address initialized"));
            }
            memset(mdnsName, 0, 21);
            snprintf(mdnsName, 21, "controllino-%02x-%02x-%02x", mac[3], mac[4], mac[5]);
            if (mdns.begin(Ethernet.localIP(), mdnsName))
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
                int len = multicastUdp.read(incomingPacket, 1500);
                if (len > 0)
                {
                    incomingPacket[len] = 0;
                }
                LOG(F("UDP packet contents:"));
                LOGN(incomingPacket);
            }
        }

        char incomingPacket[1500]{0};
        int packetSize{0};
    };
}