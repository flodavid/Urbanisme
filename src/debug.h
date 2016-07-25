#pragma once

//#define NDEBUG 1 // pour enlever les assert()

// All or no debugs, default value : 0
#define DEBUG_ALL               0 // For all debugs ; be careful, many text
#define NO_DEBUG                0 // Not any debug

// Generic debug
#define DEBUG                   0

// Resolution
#define DEBUG_EVALUATION_LIGHT  0
#define DEBUG_EVALUATION        0
#define DEBUG_PARCOURS_COORDS	0
#define DEBUG_ROADS_DIST        0
#define DEBUG_ADD_USABLE_ROAD   0
#define DEBUG_ADD_ACCESS_ROAD   0
#define DEBUG_ADD_ACCESS_GAIN   0
#define DEBUG_HOTMAP            0

#define LOGS_GENERAL    	1
#define LOGS_ACCESS_ROAD    	1
#define LOGS_PARETO             1

/* define all debug if DEBUG_ALL defined to 1 */
#if DEBUG_ALL || NO_DEBUG
    #undef DEBUG
    #undef DEBUG_EVALUATION_LIGHT
    #undef DEBUG_EVALUATION
    #undef DEBUG_PARCOURS_COORDS
    #undef DEBUG_ROADS_DIST
    #undef DEBUG_ADD_USABLE_ROAD
    #undef DEBUG_ADD_ACCESS_ROAD
    #undef DEBUG_ADD_ACCESS_GAIN
    #undef DEBUG_HOTMAP
#endif

#if DEBUG_ALL
    #define DEBUG               	1
    #define DEBUG_EVALUATION_LIGHT  1
    #define DEBUG_EVALUATION       	1
    #define DEBUG_PARCOURS_COORDS   1
    #define DEBUG_ROADS_DIST        1
    #define DEBUG_ADD_USABLE_ROAD   1
    #define DEBUG_ADD_ACCESS_ROAD   1
    #define DEBUG_ADD_ACCESS_GAIN   1
    #define DEBUG_HOTMAP            1
#endif

#if NO_DEBUG
    #define DEBUG               	0
    #define DEBUG_EVALUATION_LIGHT  0
    #define DEBUG_EVALUATION    	0
    #define DEBUG_PARCOURS_COORDS   0
    #define DEBUG_ROADS_DIST        0
    #define DEBUG_ADD_USABLE_ROAD   0
    #define DEBUG_ADD_ACCESS_ROAD   0
    #define DEBUG_ADD_ACCESS_GAIN   0
    #define DEBUG_HOTMAP            0
#endif

