#pragma once

using Identification = char[16];
Identification myId{0};

const char *mdnsName = "poolcontrollino";

#define ITSFORME(remoteId) (strcmp(remoteId, myId) == 0)
