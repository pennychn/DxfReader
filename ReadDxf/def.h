#pragma once

#ifndef DEF_H
#define DEF_H
#include "logger.h"

enum { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN,
	     ORIGIN, DESTINATION };
enum { CLOCKWISE, COUNTER_CLOCKWISE };
enum PinType { INPIN, OUTPIN, UK };
enum DIRECTION {N, NW, W, SW, S, SE, E, NE, 
	              SAME, UNKNOWN};

const unsigned int MIRROR_WIDTH  = 200;
const unsigned int MIRROR_HEIGHT = 200;
const double FEPSILON = 0.001;

#define FLOAT_EQ(x, v) (((v - FEPSILON) < x) && (x <( v + FEPSILON)))

extern Logger *pLog;
/*#define M_PIl 3.1415926535897932384626433832795029L *//* pi */
/*#define M_PI 3.14159265358979323846*/

#endif
