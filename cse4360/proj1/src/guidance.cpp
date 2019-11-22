#include <math.h>

#include "guidance.h"
#include "control.h"
#include "util.h"

const double LATCH_ANGLE = 10 deg;
const double ACCEPTABLE_ANGLE = 5 deg;

RobotGuidance::RobotGuidance(Robot& robot)
: m_robot(robot),
  m_heading_last_rotating(),
  m_was_rotating(false),
  m_waypoints() {
}

void RobotGuidance::add_waypoint(double x, double y) {
	Waypoint waypoint;
	waypoint.x = x;
	waypoint.y = y;
	m_waypoints.push(waypoint);
}

Waypoint RobotGuidance::current_waypoint() {
	return m_waypoints.top();
}

int RobotGuidance::waypoint_count() {
	return m_waypoints.size();
}

void RobotGuidance::clear_waypoints() {
	while (!m_waypoints.empty()) {
		m_waypoints.pop();
	}
}

bool RobotGuidance::at_goal() {
	return m_waypoints.empty();
}

RobotState RobotGuidance::next_state() {
#define IN_DEBUG DEBUG_GUIDANCE
	if (m_waypoints.empty()) {
		RobotState state;
		state.heading = m_robot.state.heading;
		state.velocity = 0.0;

		DEBUG {
			print("[At goal]", "");
			print("\nx = %5.1lf  gx = %5.1lf", m_robot.x, m_robot.x);
			print("\ny = %5.1lf  gy = %5.1lf", m_robot.y, m_robot.y);
			println();
			print("\nlst hdg = %-6.2lf", m_heading_last_rotating * RAD_TO_DEG);
		}

		return state;
	}

	bool on_last_waypoint = m_waypoints.size() == 1;

	Waypoint waypoint = m_waypoints.top();
	double required_heading = atan2(
			waypoint.y - m_robot.y,
			waypoint.x - m_robot.x
	);

	RobotState state;
	state.heading = required_heading;
	state.velocity = 2.83;

	double diff_to_heading = angle_diff(state.heading, m_robot.state.heading);
	if (diff_to_heading < 0) {
		state.heading += ACCEPTABLE_ANGLE;
	} else {
		state.heading -= ACCEPTABLE_ANGLE;
	}

	// we'll only rotate when:
	// 1. we're further than 5deg off course
	// 2. the desired heading has changed more than 10deg since we last rotated
	bool outside_acceptable_angle =
			fabs(diff_to_heading) > ACCEPTABLE_ANGLE;

	bool outside_latch_angle =
			fabs(angle_diff(state.heading, m_heading_last_rotating)) > LATCH_ANGLE;

	if (outside_acceptable_angle && outside_latch_angle) {
		state.velocity = 0;
		m_was_rotating = true;

		DEBUG {
			print("[Rot ", "");
		}
	} else {
		// prevent immediately rotating after angle barely crosses bounds
		if (m_was_rotating) {
			m_heading_last_rotating = required_heading;
			m_was_rotating = false;
		}

		state.heading = m_robot.state.heading;
		DEBUG {
			print("[Drv ", "");
		}
	}

	double dist_to_goal = dist(waypoint.x, waypoint.y, m_robot.x, m_robot.y);

	if ((on_last_waypoint && dist_to_goal < GOAL_RADIUS)
			|| (!on_last_waypoint && dist_to_goal < WAYPOINT_RADIUS)) {
		m_waypoints.pop();

		// we're at the goal! stop.
		if (at_goal()) {
			DEBUG2 {
				print("at Goal]", "");
			}

			state.heading = m_robot.state.heading;
			state.velocity = 0;
		} else {
			DEBUG {}
			return next_state();
		}
	} else if (dist_to_goal < SLOWDOWN_RADIUS) {
		DEBUG2 {
			print("near WP%d]", m_waypoints.size());
		}

		state.velocity /= 2;
	} else {
		DEBUG2 {
			print("to WP%d]", m_waypoints.size());
		}
	}

	DEBUG2 {
		print("\nx = %5.1lf  gx = %5.1lf", m_robot.x, waypoint.x);
		print("\ny = %5.1lf  gy = %5.1lf", m_robot.y, waypoint.y);
		print("\nd2g = %3d d2h = %-6.1lf", (int) dist_to_goal, diff_to_heading * RAD_TO_DEG);
		println();
		print("\nlst hdg = %-6.2lf", m_heading_last_rotating * RAD_TO_DEG);
		print("\ncom hdg = %-6.2lf", state.heading * RAD_TO_DEG);
		print("\ncom vel = %-6.2lf", state.velocity);
		println();
		print("\nact hdg = %-6.2lf", m_robot.state.heading * RAD_TO_DEG);
		print("\nact vel = %-6.2lf", m_robot.state.velocity);
	}

	return state;
#undef IN_DEBUG
}

