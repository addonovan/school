#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include "robot.h"

class RobotNavigator {

private:
	Robot& m_robot;

	long m_prev_left_ticks;
	long m_prev_right_ticks;

public:

	RobotNavigator(Robot& robot);

private:

	void estimate_update(double& xdiff, double& theta_diff);

public:

	void estimate_state();

};

#endif /* NAVIGATOR_H_ */
