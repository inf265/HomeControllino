#pragma once

using Identification = char[16];
static Identification myId{0};

#define ITSFORME(remoteId) (strcmp(remoteId, myId) == 0)
