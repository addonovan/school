#include "pid.h"
#include "util.h"

ControllerPid::ControllerPid(double p, double i, double d)
: m_proportional(p),
  m_integral(i),
  m_derivative(d),
  m_prev_error(),
  m_prev_power(),
  m_integrator() {
}

int ControllerPid::apply(int target) {
#define IN_DEBUG DEBUG_PID
	int error = target - m_prev_power;
	int derivative = error - m_prev_error;
	m_integrator += error;

	int power = (int) (m_proportional * error)
			+ (m_integral * m_integrator)
			+ (m_derivative * derivative);

	if (power > 100) power = 100;
	else if (power < -100) power = -100;

	DEBUG {
		print("\rt = %3d  p = %3d", target, m_prev_power);
		print("\nerr = %3d  p = %3d", error, m_prev_error);
		println();
		print("\nder = %5d", derivative);
		print("\nint = %5d", m_integrator);
		println();
		print("\npower = %3d", power);
	}

	m_prev_power = power;
	m_prev_error = error;
	return power;
#undef IN_DEBUG
}
