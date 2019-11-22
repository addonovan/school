#include <ev3.h>
#include <time.h>
#include <math.h>

#include "util.h"
#include "constants.h"

void clear_screen() {
	LcdClean();
	LcdPrintf(1, "\f");
	LcdPrintf(1, "\r               ");
	for (int i = 0; i < 11; i++) {
		LcdPrintf(1, "\n               ");
	}
	LcdPrintf(1, "\f");
}

template<typename IN, typename OUT, int N>
OUT interpolate(IN x, const IN X[N], const OUT Y[N]) {
#define IN_DEBUG DEBUG_INTERP
	double output;
	if (x <= X[0]) {
		output = Y[0];
	} else if (x >= X[N - 1]) {
		output = Y[N - 1];
	} else {
		int i = 1;
		while (i < N - 1 && x > X[i]) i++;

		double x0 = static_cast<double>(X[i - 1]);
		double x1 = static_cast<double>(X[i]);

		double y0 = static_cast<double>(Y[i - 1]);
		double y1 = static_cast<double>(Y[i]);

		double slope = (y1 - y0) / (x1 - x0);
		output = slope * (x - x0) + y0;
	}

	DEBUG2 {
		static int count = 0;
		if (count % 18 == 0) clear_screen();

		print("\n%6.4lf => %6.4lf", x, output);
		count++;
	}

	return output;
#undef IN_DEBUG
}

char lookup_power(double in) {
	return interpolate<double, char, LOOKUP_LENGTH>(in, SPEED_LOOKUP, POWER_LOOKUP);
}

double lookup_speed(char in) {
	return interpolate<char, double, LOOKUP_LENGTH>(in, POWER_LOOKUP, SPEED_LOOKUP);
}

void norm(double& x, double& y) {
	double norm = dist(0, 0, x, y);
	x /= norm;
	y /= norm;
}

double dist(double x0, double y0, double x1, double y1) {
	double xdiff = x1 - x0;
	double ydiff = y1 - y0;
	return sqrt(xdiff * xdiff + ydiff * ydiff);
}

double clamp_angle(double angle) {
	while (angle > 180 deg) angle -= 360 deg;
	while (angle < -180 deg) angle += 360 deg;

	return angle;
}

double angle_diff(double source, double target) {
	double diff = target - source;
	if (diff > 180 deg) diff -= 360 deg;
	if (diff < -180 deg) diff += 360 deg;

	return diff;
}

void unit_vector(double angle, double& x, double& y) {
	x = cos(angle);
	y = sin(angle);
}

double rotate_to(double sx, double sy, double tx, double ty) {
	double source_angle = atan2(sy, sx);
	double source_x, source_y;
	unit_vector(source_angle, source_x, source_y);

	double target_angle = atan2(ty, tx);
	double target_x, target_y;
	unit_vector(target_angle, target_x, target_y);

	return acos((source_x * target_x) + (source_y * target_y));
}

