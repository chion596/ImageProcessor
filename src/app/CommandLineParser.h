#pragma once

#include "app/Config.h"

class CommandLineParser {
public:
    static Config Parse(int argc, char** argv);
};
