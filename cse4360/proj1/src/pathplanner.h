#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include <queue>
#include <stack>
#include <cmath>
#include "robot.h"
#include "guidance.h"
#include "constants.h"

typedef float Cost;
const Cost OBSTACLE_COST = INFINITY;
const Cost OBSTACLE_PROXIMITY_FACTOR = 10.0;

struct Tile {
	Cost cost;
	bool on_path;
};


class PathPlanner {

private:
	/** The cost of this tile. Lower is better. */
	Tile m_world[WORLD_WIDTH + 2][WORLD_HEIGHT + 2];

	int m_goal_x, m_goal_y;

	Robot& m_robot;

public:
	PathPlanner(Robot& robot);

public:
	void add_waypoints(RobotGuidance& guidance);

	void print_map(const Waypoint& wp);

};

#endif /* PATHPLANNER_H_ */
