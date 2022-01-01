/*
 * Open PSARC PS3 extractor
 * Copyright (C) 2011-2018 Matthieu Milan
 */

#include "psarc.h"

int main(int argc, char* argv[])
{
    PSARC psarc;

    const char* arcName = argv[2];
    psarc.unarchive(arcName);

    return 0;
}
