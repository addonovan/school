/*
 * pid.h
 *
 *  Created on: Oct 18, 2019
 *      Author: uvs4
 */

#ifndef PID_H_
#define PID_H_

class ControllerPid {
private:
	const double m_proportional;
	const double m_integral;
	const double m_derivative;

	int m_prev_error;
	int m_prev_power;
	int m_integrator;

public:
	ControllerPid(double p, double i, double d);

public:
	int apply(int target);

};

#endif /* PID_H_ */
