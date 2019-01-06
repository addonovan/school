# Austin Donovan
# add1620

from enum import Enum
import math
import numpy as np
import os.path
from random import randint
import sys

from environment import Environment, TileType
from robot import Robot, Action, ActionSet

#
# Q Learning 
#

def q_learning(environment):
    q_values = {} # State => (ActionSet: Action => Number)
    visit_count = {} # State => (ActionSet: Action => Count)

    # Initialize all values in the tables to zero 
    def add_entries(state):
        robot = Robot(environment, state)
        q_values[state] = robot.action_set()
        visit_count[state] = robot.action_set()

    environment.for_each_tile(lambda state, _: add_entries(state))

    total_moves = 0
    while total_moves < move_count:
        old_robot = None
        action = None
        robot = Robot(environment, state=None)

        while total_moves < move_count:
            # make sure that we don't go over our move_count
            total_moves += 1

            # update our q_values and visit_count
            q_values_update(old_robot, action, robot, q_values, visit_count)

            # if we reached a terminal tile, then we stop this mission
            if robot.tile().terminal():
                break 

            # if we haven't reached a terminal position yet, then we'll make a decision
            # about how to move, then go in that direction
            action = choose_action(robot, q_values, visit_count)
            old_robot = robot
            robot = old_robot.try_move(action)

    return q_values

def q_values_update(old_robot, action, robot, q_values, visit_count):
    state = robot.state()
    current_tile = robot.tile()

    if current_tile.terminal():
        q_values[state][None] = current_tile.reward()

    if old_robot is not None:
        old_state = old_robot.state()

        # increment the number of times we've been at the previous state
        # and chosen to perform the given action
        visit_count[old_state][action] += 1

        # update our Q values with the following parameters:

        # eta(times we've performed this action in this state)
        c = 1 / visit_count[old_state][action]

        # our old value for this state
        old_q = q_values[old_state][action]

        # the base reward for being in this state
        base_reward = old_robot.tile().reward()

        # the utility of performing the best action we know of in 
        # the next state
        future_utility = q_values[state].max()

        # the expected utility for this state is the reward for being
        # in this state, along with the utility for being in the next
        # state, weighted by how much we care about the future
        expected_utility = base_reward + (gamma * future_utility)

        # calculate the new value and set it
        new_q = ((1 - c) * old_q) + (c * expected_utility)
        q_values[old_state][action] = new_q

def choose_action(robot, q_values, visit_count):
    """
    Given the `robot` and the knowledge of the environment via the `q_values`
    and `state_action_table`, this will choose an ideal action to perform, while
    also giving the ability to learn more while the agent is in its infancy.
    """
    state = robot.state()

    def f(action):
        u = q_values[state][action]
        n = visit_count[state][action]
        return 1 if n < learning_boundary else u

    # get a list of all the utilites of going to each state
    actions = visit_count[state].actions()
    utilities = [
        f(action) 
            for action in actions
    ]

    # then get the action we should take based off of that index
    action_index = int(np.argmax(utilities))
    return actions[action_index]

#
# General Overhead 
#

def read_argv():
    """
    Reads the command line arguments and returns a 5-tuple of:
    `(environment_file, default_reward, gamma, move_count, learning_boundary)`
    """
    if len(sys.argv) < 3:
        print("Usage: <environment_file> <default_reward> <gamma> <move_count> <learning_boundary>")

        if len(sys.argv) == 1:
            print("No arguments, will run hardcoded data instead")
            environment_file = "data/environment2.txt"
            default_reward = -0.04
            gamma = 0.9
            move_count = 1000
            learning_boundary = 20
            print(f'value_iterations.py "{environment_file}" "{default_reward}" "{gamma}" "{move_count}" "{learning_boundary}""')
            return (environment_file, default_reward, gamma, move_count, learning_boundary)

        else:
            exit(-1)

    environment_file = sys.argv[1]
    if not os.path.isfile(environment_file):
        print(f"No such file: {environment_file} (training file)")
        exit(-2)

    default_reward = float(sys.argv[2])
    gamma = float(sys.argv[3])

    move_count = int(sys.argv[4])
    if move_count < 1:
        print(f"<move_count> ({move_count}) must be at least 1")
        exit(-5)

    learning_boundary = int(sys.argv[5])
    if learning_boundary < 1:
        print(f"<learning_boundary> ({learning_boundary}) must be at least 1")
    
    return (environment_file, default_reward, gamma, move_count, learning_boundary)

#
# Script
#

(environment_file, default_reward, gamma, move_count, learning_boundary) = read_argv()
environment = Environment(environment_file, default_reward)
q_values = q_learning(environment)

def print_tile(state, more_tiles_following):
    # the utility of being in this state is the utility of performing
    # the BEST action while in this state. So just find that one from
    # our action list
    utility = q_values[state].max()
    print(f"{utility:6.3f}", end = "")
    if more_tiles_following:
        print(",", end = "")

environment.for_each_tile(print_tile, print)
#print()

# def print_policy(state, _):
#     tile = environment.tile_at(state)

#     char = "X"
#     if tile.enterable():
#         best_action = q_values[state].max_action()
#         char = ActionSet.action_chars[best_action]

#     print(f"{char}\t", end = "")

# environment.for_each_tile(print_policy, print)
# print()
