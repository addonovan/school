#include <algorithm>
#include <cmath>

#include "pathplanner.h"
#include "constants.h"
#include "util.h"

double round(double x) { return std::floor(x + 0.5); }

Cost max(Cost a, Cost b) {
	if (a > b) return a;
	else return b;
}

int to_grid_pos(double x) {
	return static_cast<int>(round(x / HALF_TILE_SIZE)) + 1;
}

double to_world_pos(int x) {
	return static_cast<double>((x - 1) * HALF_TILE_SIZE);
}


struct Point {
	unsigned int x;
	unsigned int y;
};

PathPlanner::PathPlanner(Robot& robot)
: m_world(),
  m_goal_x(),
  m_goal_y(),
  m_robot(robot) {

	// initialize the world
	for (int i = 0; i <= WORLD_WIDTH + 1; i++) {
		for (int j = 0; j <= WORLD_HEIGHT + 1; j++) {
			Tile tile;
			tile.cost = HALF_TILE_SIZE;
			tile.on_path = false;

			if (i < 1 || j < 1 || i > WORLD_WIDTH || j > WORLD_HEIGHT) {
				tile.cost = OBSTACLE_COST;
			}

			m_world[i][j] = tile;
		}
	}

	// initialize the obstacles
	for (int i = 0; i <= MAX_OBSTACLES + 1; i++) {
		for (int j = 0; j <= MAX_OBSTACLES + 1; j++) {
			if (i == 0 || j == 0 || i == WORLD_WIDTH || j == WORLD_HEIGHT) {
				m_world[i][j].cost = OBSTACLE_COST;
			}
		}
	}

	// fill out all of the obstacles as bad
	const Cost near_obstacle_cost = HALF_TILE_SIZE * OBSTACLE_PROXIMITY_FACTOR;

	for (int i = 0; i < MAX_OBSTACLES; i++) {
		int x = to_grid_pos(obstacle[i][0] * 100);
		int y = to_grid_pos(obstacle[i][1] * 100);

		for (int xdiff = x; xdiff <= x + 1; xdiff++) {
			for (int ydiff = y; ydiff <= y + 1; ydiff++) {
				m_world[xdiff][ydiff].cost = OBSTACLE_COST;

				// set all neighboring tiles to be near-obstacles
				for (int idiff = xdiff - 1; idiff <= xdiff + 1; idiff++) {
					for (int jdiff = ydiff - 1; jdiff <= ydiff + 1; jdiff++) {

						// skip over this obstacle
						if (xdiff == idiff && ydiff == jdiff) continue;

						// skip over other obstacles
						if (m_world[idiff][jdiff].cost == OBSTACLE_COST) continue;

						// if any 8-neighbor is an obstacle, then penalize it
						m_world[idiff][jdiff].cost = near_obstacle_cost;
					}
				}
			}
		}
	}

	m_goal_x = to_grid_pos(goal[0] * 100);
	m_goal_y = to_grid_pos(goal[1] * 100);
}

struct Node {
	int i;
	int j;
	Cost path_cost;
	Cost heuristic;

	Node(int i_, int j_, Cost path_cost_)
	: i(i_),
	  j(j_),
	  path_cost(path_cost_),
	  heuristic(OBSTACLE_COST)
	{}

	Node()
	: i(-1),
	  j(-1),
	  path_cost(OBSTACLE_COST),
	  heuristic(OBSTACLE_COST)
	{}

	bool operator<(const Node& other) const {
		return path_cost + heuristic > other.path_cost + other.heuristic;
	}

	bool operator==(const Node& other) const {
		return i == other.i && j == other.j;
	}
};

void PathPlanner::add_waypoints(RobotGuidance& guidance) {
#define IN_DEBUG DEBUG_PATHING

	for (int i = 1; i <= WORLD_WIDTH; i++) {
		for (int j = 1; j <= WORLD_HEIGHT; j++) {
			m_world[i][j].on_path = false;
		}
	}

	int start_x = to_grid_pos(m_robot.x);
	int start_y = to_grid_pos(m_robot.y);

	Node predecessors[WORLD_WIDTH + 2][WORLD_HEIGHT + 2];
	Cost costs[WORLD_WIDTH + 2][WORLD_HEIGHT + 2];
	std::fill_n(
			reinterpret_cast<Cost*>(costs),
			sizeof(costs)/sizeof(Cost),
			OBSTACLE_COST
	);

	double goal_pos_x = to_world_pos(m_goal_x);
	double goal_pos_y = to_world_pos(m_goal_y);

	std::priority_queue<Node> fringe;

	Node start_node(start_x, start_y, 0);
	predecessors[start_node.i][start_node.j] = start_node;
	fringe.push(start_node);

	int max_fringe_size = 0;
	int iterations = 0;

	while (!fringe.empty()) {
		if (iterations % 100 == 0) {
			DEBUG {
				print("\riters = %d", iterations);
				print("\nmax fringe:", "");
				print("\n   count = %d", max_fringe_size);
				print("\n    size = %d B", max_fringe_size * sizeof(Node));
				println();
				print("\nmem = %d B", max_fringe_size * sizeof(Node) + sizeof(PathPlanner));
				Wait(10);
			}
		}

		iterations++;
		max_fringe_size = std::max(max_fringe_size, (int) fringe.size());

		Node current = fringe.top();
		fringe.pop();

		// we hit the goal!
		if (current.i == m_goal_x && current.j == m_goal_y)
			break;

		Cost current_cost = current.path_cost;

		// get information about our predecessor (if any)
		Node pred = predecessors[current.i][current.j];
		int i_change = current.i - pred.i;
		int j_change = current.j - pred.j;

		// for all of the neighboring tiles...
		for (int i = current.i - 1; i <= current.i + 1; i++) {
			for (int j = current.j - 1; j <= current.j + 1; j++) {
				// don't consider staying stationary
				if (i == current.i && j == current.j) continue;

				Tile tile = m_world[i][j];
				Cost new_cost = current_cost + tile.cost;

				// 2.5x tile cost for any change in direction
				if (current.i != start_x && current.j != start_y) {
					// sqrt(2)x tile cost for diagonal movements
					if (i != current.i && j != current.j) {
						new_cost += tile.cost * 0.41;
					}

					if (i - current.i != i_change) new_cost += 1.5 * tile.cost;
					if (j - current.j != j_change) new_cost += 1.5 * tile.cost;
				}

				// if we already have a better path through this node, skip this one
				if (new_cost > costs[i][j]) continue;

				Node new_node(i, j, new_cost);
				new_node.heuristic = std::sqrt(
						std::pow(to_world_pos(i) - goal_pos_x, 2.0) +
						std::pow(to_world_pos(j) - goal_pos_y, 2.0)
				);

				costs[i][j] = new_cost;
				predecessors[i][j] = current;
				fringe.push(new_node);
			}
		}
	}

	DEBUG {
		print("\riters = %d", iterations);
		print("\nmax fringe:", "");
		print("\n   count = %d", max_fringe_size);
		print("\n    size = %d B", max_fringe_size * sizeof(Node));
		println();
		print("\nmem = %d B", max_fringe_size * sizeof(Node) + sizeof(PathPlanner));
		Wait(5000);
	}

	// backtrack through the path and push them onto the guidance
	// system's waypoint stack
	Node current(m_goal_x, m_goal_y, 0);

	do {
		m_world[current.i][current.j].on_path = true;
		double x = to_world_pos(current.i);
		double y = to_world_pos(current.j);
		guidance.add_waypoint(x, y);

		Node pred = predecessors[current.i][current.j];
		current = pred;
	} while (!(current.i == start_x && current.j == start_y));
#undef IN_DEBUG
}

const int COLS = 21;
const int COL_CENTER = COLS / 2;
const int ROWS = 13;
const int ROW_CENTER = (ROWS / 2) - 1; // 1 row reserved for pos stats

void PathPlanner::print_map(const Waypoint& wp) {
	const int pos_i = to_grid_pos(m_robot.x);
	const int pos_j = to_grid_pos(m_robot.y);

	print(
			"(%3.0f, %3.0f)  (%3.0f, %3.0f)",
			m_robot.x, m_robot.y,
			wp.x, wp.y
	);
	for (
		int j = pos_j + ROW_CENTER;
		j >= pos_j - ROW_CENTER;
		j--
	) {
		for (
			int i = pos_i - COL_CENTER;
			i < pos_i + COL_CENTER;
			i++
		) {
			if (i < 0 || j < 0 || i >= WORLD_WIDTH + 2 || j >= WORLD_HEIGHT + 2) {
				print(".", "");
			} else if (i == pos_i && j == pos_j) {
				print("@", "");
			} else if (m_world[i][j].on_path) {
				print("*", "");
			} else if (m_world[i][j].cost > HALF_TILE_SIZE * OBSTACLE_PROXIMITY_FACTOR * 2) {
				print("X", "");
			} else if (m_world[i][j].cost > HALF_TILE_SIZE) {
				print("+", "");
			} else {
				print(" ", "");
			}
		}
		print("\n", "");
	}
}
