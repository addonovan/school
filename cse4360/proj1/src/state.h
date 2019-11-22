#ifndef STATE_H_
#define STATE_H_

struct RobotState {
	/** The current angle in world coordinates [rad] */
	double heading;

	/** The velocity, [mm/s], along the direction of `heading` */
	double velocity;
};



#endif /* STATE_H_ */
