#ifndef CONTROL_H_
#define CONTROL_H_

#include "constants.h"
#include "robot.h"
#include "state.h"

class RobotController {

private:
	Robot& m_robot;

	ControllerPid m_left_pid;
	ControllerPid m_right_pid;

public:

	RobotController(Robot& robot);

	//
	// Movement Intrinsics
	//

public:

	/**
	 * Moves the given `motor` forward at the given `speed`.
	 *
	 * If the `speed` is negative, then the motor will rotate
	 * backwards. These values will NOT be passed to the PID
	 * controller for the respective wheel! That needs to be
	 * done before calling this member function.
	 *
	 * @param motor
	 * 			One of `MOTOR_LEFT`, `MOTOR_RIGHT`
	 * @param speed
	 * 			A speed value, [-100, 100]
	 */
	void move(char motor, char speed);

public:

	void command_state(const RobotState& commanded_state);

	void rotate90(int dir);

	void stop();


};


#endif /* CONTROL_H_ */
