#ifndef _UTIL_H__
#define _UTIL_H__

#include <ev3.h>

//
// Debugging Macros
//

#define DEBUG_PATHING	 	false
#define DEBUG_GUIDANCE 		false
#define DEBUG_NAVIGATOR		false
#define DEBUG_CONTROL 		false
#define DEBUG_PID			false
#define DEBUG_INTERP		false
#define DEBUG_DEFAULT 		!(DEBUG_PATHING || DEBUG_GUIDANCE || DEBUG_CONTROL || DEBUG_PID || DEBUG_INTERP)

#define DEBUG2 \
	if (IN_DEBUG)

#define DEBUG \
	if (IN_DEBUG) clear_screen();\
	DEBUG2

#define print(x, ...) LcdPrintf(1, x, __VA_ARGS__)

#define println() LcdPrintf(1, "\n")

//
// Display
//

void clear_screen();

//
// Power Lookup
//

char lookup_power(double speed);

double lookup_speed(char power);

//
// Math
//

void norm(double& x, double& y);

double dist(double x0, double y0, double x1, double y1);

double clamp_angle(double angle);

/**
 * The angle measured between a1 and a2.
 *
 * Precondition: both angles are already clamped.
 */
double angle_diff(double source, double target);

double rotate_to(double sx, double sy, double tx, double ty);

#endif
