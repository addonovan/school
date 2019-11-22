#include <ev3.h>
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "control.h"
#include "util.h"

#define KP 1
#define KI 0
#define KD 0

RobotController::RobotController(Robot& robot)
: m_robot(robot),
  m_left_pid(KP, KI, KD),
  m_right_pid(KP, KI, KD) {
}

//
// Intrinsics
//

void RobotController::move(char motor, char speed) {
	if (speed >= 0) {
		OnFwdReg(motor, speed);
	} else {
		OnRevReg(motor, -speed);
	}
}

//
// Control
//

void RobotController::command_state(const RobotState& commanded_state) {
#define IN_DEBUG DEBUG_CONTROL
	// get the change in state that we need to produce to get to the commanded state
	double delta_theta = angle_diff(m_robot.state.heading, commanded_state.heading);
	double delta_speed = commanded_state.velocity - m_robot.state.velocity;

	if (fabs(delta_theta) < 1 deg) {
		delta_theta = 0.0;
	}

	if (fabs(delta_speed) < 0.2) {
		delta_speed = 0.0;
	}

	// from mechanics above:
	// 1. delta_theta = 1/d (v_r - v_l)
	// 2. delta_v = 1/2 (v_r + v_l)
	//
	// 3. (1) => v_r = d * delta_theta + v_l
	// 4. (2) => v_l = 2 * delta_v - v_r
	//
	// substitute (4) into (3) and simplify
	// v_r = (d * delta_theta) + (2 * delta_v - v_r)
	// 2v_r = (d * delta_theta) + (2 * delta_v)
	// v_r = (d * delta_theta)/2 + delta_v
	//
	// Then use (4) to solve for v_l


	// remember: if there's no changes necessary, then we just need to maintain
	// the current course MEANING the equations above are the CHANGES in the left & right
	// velocities, so here we added the current velocities to those changes
	double delta_translational_right = ((AXIAL_WIDTH * delta_theta) / 2.0) + delta_speed;
	double delta_translational_left = 2 * delta_speed - delta_translational_right;

	double new_translational_right = m_robot.state.velocity + delta_translational_right;
	double new_translational_left = m_robot.state.velocity + delta_translational_left;

	// normalize the speed values
//	norm(new_translational_right, new_translational_left);
//	new_translational_right *= POWER_LOOKUP[LOOKUP_LENGTH - 2];
//	new_translational_left *= POWER_LOOKUP[LOOKUP_LENGTH - 2];

	// put the speeds through the map to translate [cm/s] to % power
	char new_right_power = lookup_power(new_translational_right);
	char new_left_power = lookup_power(new_translational_left);

	// now put these values through the pid to get what we should actually
	// power the wheels
	char right_power = m_right_pid.apply(new_right_power);
	char left_power = m_left_pid.apply(new_left_power);

	// average the estimated speed values together to get an estimation
	// of our robot's current speed
	m_robot.state.velocity = 0.5 * (lookup_speed(new_right_power) + lookup_speed(new_left_power));

	DEBUG {
		print("\rx = %4.1lf  y = %4.1lf", m_robot.x, m_robot.y);
		print("\ndtheta = %-6.2lf", delta_theta deg);
		print("\ndspeed = %-6.2lf", delta_speed);
		println();
		print("\nv_r = %-6.2lf", new_translational_right);
		print("\nv_l = %-6.2lf", new_translational_left);
		println();
		print("\np:  r = %3d  l = %3d", new_right_power, new_left_power);
		print("\npid r = %3d  l = %3d", right_power, left_power);
		println();
		print("\nvel = %-6.2lf", m_robot.state.velocity);
	}

	// finally, move the robot with the new values
	move(MOTOR_RIGHT, new_right_power);
	move(MOTOR_LEFT, new_left_power);

#undef IN_DEBUG
}

void RobotController::rotate90(int dir) {
	move(MOTOR_LEFT, -dir * 100);
	move(MOTOR_RIGHT, dir * 100);
}

void RobotController::stop() {
	move(MOTOR_LEFT, 0);
	move(MOTOR_RIGHT, 0);
}


