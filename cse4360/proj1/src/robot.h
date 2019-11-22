#ifndef ROBOT_H_
#define ROBOT_H_

#include "pid.h"
#include "state.h"

struct Robot {
	/** The robot's current x coordinate in world coordinates [cm]. */
	double x;

	/** The robot's current y coordinate in world coordinates [cm]. */
	double y;

	/** The robot's current/last known state */
	RobotState state;
};

#endif /* ROBOT_H_ */
