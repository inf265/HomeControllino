#pragma once

using Identification = char[16];
static Identification myId{0};

char mdnsName[21];

#define ITSFORME(remoteId) (strcmp(remoteId, myId) == 0)
