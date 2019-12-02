#include <stdio.h>
#include <math.h>
#include "explore.h"

#define STATES  82                          /* Number of states        */
#define ACTIONS 2                           /* Number of actions       */
#define GAMMA 0.99                          /* Discount factor         */
#define BETA  0.25                          /* Learning rate           */

extern double Q_values[ACTIONS][STATES];    /* Q value table            */

extern int action;                          /* Last action taken        */
extern int state;                           /* Current state            */
extern int old_state;                       /* Previous state           */



/* Q-learning function                                                  */
/* Inputs: reinf   :   reinforcement received                           */
/*         explore :   1 if random actions should be taken              */
/*                     0 if always the action with the highest          */
/*                     Q value should be taken                          */
/* Return value:  action number ( 0 or 1 )                              */
int Q_learn(reinf, rand_act)
double reinf;
int rand_act;
{

}
