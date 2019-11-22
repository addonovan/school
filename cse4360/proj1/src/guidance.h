#ifndef GUIDANCE_H_
#define GUIDANCE_H_

#include <stack>

#include "robot.h"
#include "state.h"

const double SLOWDOWN_RADIUS 	= 0;  // cm
const double WAYPOINT_RADIUS 	= 5;  // cm
const double GOAL_RADIUS        = 1;  // cm

struct Waypoint {
	double x;
	double y;
};

class RobotGuidance {

private:
	Robot& m_robot;
	double m_heading_last_rotating;
	bool m_was_rotating;

	std::stack<Waypoint> m_waypoints;

public:
	RobotGuidance(Robot& robot);

public:
	/**
	 * Adds a waypoint, located at (x, y) (in cm world coordinates),
	 * to the guidance system.
	 */
	void add_waypoint(double x, double y);

	Waypoint current_waypoint();

	int waypoint_count();

	void clear_waypoints();

	bool at_goal();

	RobotState next_state();

};

#endif

