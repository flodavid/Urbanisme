// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include <stdio.h>
// #include <tchar.h>

// Others
#include "targetver.h"

// TODO: reference additional headers your program requires here

// To represent infinity
#define UNSIGNED_INFINITY std::numeric_limits<unsigned>::max()

// For error and debug output

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT TOSTRING(__FILE__) ":l" TOSTRING(__LINE__)

void error(const char *location, const char *msg);
void debug(const char *msg, const char *location);

// For debugging logs
#include "debug.h"
