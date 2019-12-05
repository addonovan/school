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
int Q_learn(double reinf, int rand_act) {
    // find the best action based on our current state
    // argmax(Q_values[x][state])
    int best_action = 0;
    for (int i = 1; i < ACTIONS; i++) {
        if (Q_values[i][state] > Q_values[best_action][state]) {
            best_action = i;
        }
    }

    // update the Q value
    double old_q = Q_values[action][old_state];
    double future_utility = Q_values[best_action][state];
    double new_q = ((1 - BETA) * old_q) 
                 + (BETA * (reinf + GAMMA * future_utility));
    Q_values[action][old_state] = new_q;

    // now, return the action we're supposed to
    if (rand_act == 1) {
        return bolzman_exploration(Q_values);
    } else {
        return best_action;
    }
}

