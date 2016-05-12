#pragma once

//#define NDEBUG 1 // pour enlever les assert()

// For all debugs ; be careful, many text
#define DEBUG_ALL               0

// Generic debug
#define DEBUG                   0

// Resolution
#define DEBUG_EVALUATION_LIGHT  0
#define DEBUG_EVALUATION        0
#define DEBUG_PARCOURS_COORDS	0
#define DEBUG_ROADS_DIST        0
#define DEBUG_ADD_USABLE_ROAD   0
#define DEBUG_ADD_ACCESS_ROAD   1
#define DEBUG_PARETO            1

/* define all debug if DEBUG_ALL defined to 1 */
#if DEBUG_ALL or NDEBUG
    #undef DEBUG
    #undef DEBUG_EVALUATION
    #undef DEBUG_EVALUATION_LIGHT
    #undef DEBUG_PARCOURS_COORDS
    #undef DEBUG_ROADS_DIST
    #undef DEBUG_ADD_USABLE_ROAD
    #undef DEBUG_ADD_ACCESS_ROAD
    #undef DEBUG_PARETO
#endif

#if DEBUG_ALL
    #define DEBUG               	1
    #define DEBUG_EVALUATION_LIGHT  1
    #define DEBUG_EVALUATION       	1
    #define DEBUG_PARCOURS_COORDS   1
    #define DEBUG_ROADS_DIST        1
    #define DEBUG_ADD_USABLE_ROAD   1
    #define DEBUG_ADD_ACCESS_ROAD   1
    #define DEBUG_PARETO            1
#endif

#ifdef NDEBUG
    #define DEBUG               	0
    #define DEBUG_EVALUATION_LIGHT  0
    #define DEBUG_EVALUATION    	0
    #define DEBUG_PARCOURS_COORDS   0
    #define DEBUG_ROADS_DIST        0
    #define DEBUG_ADD_USABLE_ROAD   0
    #define DEBUG_ADD_ACCESS_ROAD   0
    #define DEBUG_PARETO            0
#endif

