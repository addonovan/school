#include <ev3.h>
#include "constants.h"
#include "util.h"
#include "robot.h"

#include "pathplanner.h"
#include "navigator.h"
#include "guidance.h"
#include "control.h"
#include "constants.h"

#define IN_DEBUG DEBUG_DEFAULT

#define CALIBRATION false

int main() {
	InitEV3();
	DEBUG { print("\rInitializing...", ""); }

	Robot robot;
	if (CALIBRATION) {
		robot.x = robot.y = 0;
	} else {
		robot.x = start[0] * 100;
		robot.y = start[1] * 100;
	}
	robot.state.heading = robot.state.velocity = 0;

	RobotNavigator nav(robot);
	RobotGuidance guidance(robot);
	RobotController controller(robot);
	DEBUG2 { print("done", ""); }

	DEBUG2 { print("\n Pathing...", ""); }
	PathPlanner planner(robot);
	if (CALIBRATION) {
		guidance.add_waypoint(4 * 30.5, 4 * 30.5);
//		guidance.add_waypoint(4 * 30.5, 0);
	} else {
		planner.add_waypoints(guidance);
	}
	DEBUG2 { print("done", ""); }

	DEBUG2 { print("\n  %d waypoints", guidance.waypoint_count()); }

	DEBUG2 { print("\nReady!", ""); }
	Wait(1000);

	// print the map for orientation purposes
	if (!CALIBRATION) {
		clear_screen();
		planner.print_map(guidance.current_waypoint());

		// wait until the user presses the go button
		while (ButtonIsUp(BTNCENTER)) Wait(10);
		while (ButtonIsDown(BTNCENTER)) Wait(10);
	}

	int display_page = 0;
	int disp_counter = 0;
	int last_page_update = 0;

//	for (int i = 0; i < 50000 / 100 && !ButtonIsDown(BTNCENTER); i++) {
	while (!guidance.at_goal() && !ButtonIsDown(BTNCENTER)) {

		nav.estimate_state();
		RobotState commanded_state = guidance.next_state();
		controller.command_state(commanded_state);

		DEBUG {
			disp_counter++;

			if (display_page < 0) display_page = 1;
			else if (display_page > 1) display_page = 0;

			switch (display_page) {
			case 0: {
				long left_ticks = MotorRotationCount(MOTOR_LEFT);
				long right_ticks = MotorRotationCount(MOTOR_RIGHT);

				print("\rt = %-6ld  %-6ld",	left_ticks, right_ticks);
				print("\nx = %-5.1lf cm", robot.x);
				print("\ny = %-5.1lf cm", robot.y);
				println();
				print("\ncmd hdg = %-6.2lf", commanded_state.heading * RAD_TO_DEG);
				print("\ncmd vel = %-6.2lf", commanded_state.velocity);
				println();
				print("\nact hdg = %-6.2lf", robot.state.heading * RAD_TO_DEG);
				print("\nact vel = %-6.2lf", robot.state.velocity);
				println();
				break;
			}

			case 1:
				planner.print_map(guidance.current_waypoint());
				break;
			}

			if (disp_counter - last_page_update > 5) {
				if (ButtonIsDown(BTNRIGHT)) {
					display_page++;
					last_page_update = disp_counter;
				} else if (ButtonIsDown(BTNLEFT)) {
					display_page--;
					last_page_update = disp_counter;
				}
			}
		}

		Wait(100);
	}

	DEBUG2 { print("\nDone!", ""); }
	controller.stop();

	// wait until the button is pressed again
	while (ButtonIsDown(BTNCENTER)) Wait(10);
	while (ButtonIsUp(BTNCENTER)) Wait(10);

	FreeEV3();
	return 0;
}
