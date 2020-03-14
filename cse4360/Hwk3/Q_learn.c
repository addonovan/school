#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "explore.h"

#define STATES  82                          /* Number of states        */
#define ACTIONS 2                           /* Number of actions       */
#define GAMMA 0.99                          /* Discount factor         */
#define BETA  0.25                          /* Learning rate           */

extern double Q_values[ACTIONS][STATES];    /* Q value table            */

extern int action;                          /* Last action taken        */
extern int state;                           /* Current state            */
extern int old_state;                       /* Previous state           */


double max(double a, double b);


/* Q-learning function                                                  */
/* Inputs: reinf   :   reinforcement received                           */
/*         explore :   1 if random actions should be taken              */
/*                     0 if always the action with the highest          */
/*                     Q value should be taken                          */
/* Return value:  action number ( 0 or 1 )                              */
int Q_learn(double reinf,int rand_act){

//QLearning algorithim
//double currentstate = Q_values[action][state];
double qval_old = (1-BETA) * Q_values[action][old_state];
double value_learned = BETA * (reinf + GAMMA * max(Q_values[0][state],Q_values[1][state]));
Q_values[action][old_state] = qval_old + value_learned;

if (rand_act == 0) {
    //Action 0 has the higher QVal
    if(Q_values[0][state] > Q_values[1][state]){return 0;}
    //Action 1 has the higher QVal
    else{return 1;}
}
    //Use random evaluation
if (rand_act == 1) {
  return bolzman_exploration(Q_values);
  }

}
//Max value calculator

double max(double a, double b){
  if (a >b){return a; }
  else{ return b; }
}