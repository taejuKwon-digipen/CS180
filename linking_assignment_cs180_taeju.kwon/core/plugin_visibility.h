// Rudy Castan
// Linking Assignment
// CS180
// Fall 2021
#pragma once

// Based off of https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
#    define VISIBILITY_DLL_IMPORT __declspec(dllimport)
#    define VISIBILITY_DLL_EXPORT __declspec(dllexport)
#else
#    define VISIBILITY_DLL_IMPORT __attribute__((visibility("default")))
#    define VISIBILITY_DLL_EXPORT __attribute__((visibility("default")))
#endif


#ifdef CORE_DLL_EXPORTS
#    define CORE_API VISIBILITY_DLL_EXPORT
#    define PLUGIN_API VISIBILITY_DLL_IMPORT
#else
#    define CORE_API VISIBILITY_DLL_IMPORT
#    define PLUGIN_API VISIBILITY_DLL_EXPORT
#endif

#define CORE_FUNC extern "C" CORE_API
#define PLUGIN_FUNC extern "C" PLUGIN_API
