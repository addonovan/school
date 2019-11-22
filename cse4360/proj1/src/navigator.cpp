#include <ev3.h>
#include <math.h>

#include "navigator.h"
#include "constants.h"
#include "util.h"

RobotNavigator::RobotNavigator(Robot& robot)
: m_robot(robot),
  m_prev_left_ticks(),
  m_prev_right_ticks() {
	ResetRotationCount(MOTOR_BOTH);
}

//
// Odometry
//

void RobotNavigator::estimate_update(
		double& x_diff,
		double& theta_diff
) {
#define IN_DEBUG DEBUG_NAVIGATOR
	long current_left_ticks = MotorRotationCount(MOTOR_LEFT);
	long current_right_ticks = MotorRotationCount(MOTOR_RIGHT);

	// calculate update times
	double left_tick_diff = (double) current_left_ticks - m_prev_left_ticks;
	double right_tick_diff = (double) current_right_ticks - m_prev_right_ticks;


	// calculate the left & right translational velocities
	double left_translational = CM_PER_TICK * left_tick_diff;
	double right_translational = CM_PER_TICK * right_tick_diff;


	// calculate differences in body coordinates
	double delta_x = 0.5 * (left_translational + right_translational);
	double delta_theta = 0.85 * INVERSE_AXIAL_WIDTH // leading constant pulled from my ass
			* (right_translational - left_translational);


	// integrate change in body coordinates to get relative change
	x_diff = delta_x;
	theta_diff = delta_theta;


	DEBUG {
		print("\rld = %-6.2lf", left_tick_diff);
		print("\nrd = %-6.2lf", right_tick_diff);

		print("\nlt = %-6.2lf", left_translational);
		print("\nrt = %-6.2lf", right_translational);

		println();
		print("\navgds = %-6.2lf", delta_x);
		print("\nomega = %-6.2lf", delta_theta);

		println();
		print("\nxd   = %-6.4lf", x_diff * 1000);
		print("\ntd   = %-6.4lf", theta_diff * 1000);
	}

	// update the last values
	m_prev_left_ticks = current_left_ticks;
	m_prev_right_ticks = current_right_ticks;

#undef IN_DEBUG
}

void RobotNavigator::estimate_state() {
	// get the changes since the last update
	double x_diff, theta_diff;
	estimate_update(x_diff, theta_diff);

	// accumulate the changes over time
	// while also converting to world coordinates
	m_robot.x += x_diff * cos(m_robot.state.heading);
	m_robot.y += x_diff * sin(m_robot.state.heading);
	m_robot.state.heading = clamp_angle(m_robot.state.heading + theta_diff);
}

