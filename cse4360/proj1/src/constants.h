#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//
// World Properties
//

/** The height/width of a tile [cm] */
const double TILE_SIZE = 30.5;
const double HALF_TILE_SIZE = TILE_SIZE / 2;

/** The width of the world [half-tiles] */
const int WORLD_WIDTH = 16 * 2;
const double WORLD_WIDTH_CM = WORLD_WIDTH * HALF_TILE_SIZE;

/** The height of the world [half-tiles] */
const int WORLD_HEIGHT = 10 * 2;
const double WORLD_HEIGHT_CM = WORLD_HEIGHT * 2 * HALF_TILE_SIZE;

//
// Wiring Definitions
//

#define MOTOR_LEFT 	OUT_D
#define MOTOR_RIGHT OUT_A
#define MOTOR_BOTH  MOTOR_LEFT | MOTOR_RIGHT


//
// Robot Properties
//

/**
 * Number of millimeters traveled in 1,000 encoder ticks.
 * Determined experimentally.
 */
const double CM_PER_TICK = (
		( 76.5 / ((26219 + 26573) / 2)) +
		( 75.0 / ((26438 + 26854) / 2)) +
		( 75.5 / ((26541 + 26809) / 2)) +
		( 75.0 / ((26354 + 26646) / 2)) +
		(124.5 / ((43756 + 44432) / 2))
) / 5;

/** Width between the center of the two wheels [cm] */
const double AXIAL_WIDTH = 10.2;

/** The reciprocal of `AXIAL_WIDTH` [1/cm] */
const double INVERSE_AXIAL_WIDTH = 1 / AXIAL_WIDTH;

/**
 * A lookup table for speeds, stored in [cm/s].
 *
 * Each entry corresponds to a power value in `POWER_LOOKUP`.
 */
static const double SPEED_LOOKUP[] = {
		-85.9 / 30,		// -100
		-81.0 / 30,		// -75
		-47.5 / 30,		// -50
		0,				//  0
		47.5 / 30,		//  50
		81.0 / 30,		//  75
		85.9 / 30		//  100
};

/**
 * A lookup table for powers, stored in...power units? These
 * are values that can be supplied to the motors.
 *
 * Each entry corresponds to a speed value in `SPEED_LOOKUP`.
 */
static const char POWER_LOOKUP[] = {
		-100,
		-75,
		-50,
		0,
		50,
		75,
		100
};

const int LOOKUP_LENGTH = sizeof(POWER_LOOKUP) / sizeof(char);

//
// Mathematical Constants
//

const double RAD_TO_DEG = 180.0 / 3.14159;

const double DEG_TO_RAD = 3.14159 / 180.0;

#define deg * DEG_TO_RAD


//
// World Definition
//

#define MAX_OBSTACLES 25

const int num_obstacles = 13;

const double obstacle[MAX_OBSTACLES][2] = {
		{0.61, 2.743},
		{0.915, 2.743},
		{1.219, 2.743},
		{1.829, 1.219},
		{1.829, 1.524},
		{ 1.829, 1.829},
		{1.829, 2.134},
		{2.743, 0.305},
		{2.743, 0.61},
		{2.743, 0.915},
		{2.743, 2.743},
		{3.048, 2.743},
		{3.353, 2.743},
		{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},
		{-1,-1},{-1,-1},{-1,-1}
};

const double start[2] = {0.305, 1.219};
const double goal[2] = {3.658, 1.829}; /* goal location */


#endif /* CONSTANTS_H_ */
