// stdafx.cpp : source file that includes just the standard includes
// Sim City.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <iostream>

void error(const char *location, const char *msg)
{
  std::cerr << "Error at "<< location<< " : "<< msg<< std::endl;
}


void debug(const char *msg, const char *location)
{
// Generic debug
//#define DEBUG
#ifdef DEBUG
  printf("%s at : %s\n", msg, location);
#endif
}