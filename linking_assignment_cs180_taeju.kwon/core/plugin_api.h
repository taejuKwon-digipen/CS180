// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

#include "plugin_visibility.h"

class PluginFactory;

PLUGIN_FUNC bool PluginInit(PluginFactory& plugin_factory);

PLUGIN_FUNC const char* PluginDisplayName();
