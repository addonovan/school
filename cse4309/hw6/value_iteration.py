# Austin Donovan
# add1620

from enum import Enum
import math
import numpy as np
import os.path
from random import randint
import sys

from environment import Environment, TileType
from robot import Robot, Action

#
# Value Iteration
#

def action_utility(utilities, state, action):
    """
    Calcualtes the overall utility of performing the given `action` in the
    current `state`, knowing all of the states' previously calculated
    `utilities`.
    """
    base_robot = Robot(environment, state)

    # based on all possibilities from trying to perform the action at
    # the current position, calculate all of the resultant states'
    # utilities. Then weight them based on the probability of those
    # states being reached given the current action.
    accumulator = 0
    for (action_set, probability) in action.movement_possibilities():
        new_robot = base_robot.move(action_set[action])
        new_utility = utilities[new_robot.state()] 
        accumulator += probability * new_utility
    
    return accumulator

def state_utility(utilities, environment, state):
    """
    Calculates the utility of being in the given `state` in the
    current `environment`, knowing the states' previously calculated
    `utilities`.
    """
    tile = environment.tile_at(state)
    # terminal states end the agent's action, so their utilities are
    # simply their rewards
    if tile.terminal():
        return tile.reward()

    # If the tile can't be entered at all, then it has no utility
    # whatsoever (these are blocked tiles)
    elif not tile.enterable():
        return 0 

    base_utility = environment.tile_at(state).reward()
    best_action_utility = max([
        action_utility(utilities, state, action)
            for action in [Action.Up, Action.Down, Action.Left, Action.Right]
    ])
    return base_utility + (gamma * best_action_utility)

def value_iteration(environment, iteration_count):
    """
    Performs the value iteration algorithm `iteration_count` times for
    the given `environment`, then returns the resultant vector of utilities
    for all states.
    """
    states = environment.state_list()
    utilities = {
        state: 0
            for state in states
    }

    # run the policy update `iteration_count` times
    for _ in range(0, iteration_count):
        utilities_copy = utilities.copy()
        for state in states:
            utilities[state] = state_utility(utilities_copy, environment, state)
    
    return utilities

#
# General Overhead 
#

def read_argv():
    """
    Reads the command line arguments and returns a 4-tuple of:
    `(environment_file, default_reward, gamma, iteration_count)`
    """
    if len(sys.argv) < 3:
        print("Usage: <environment_file> <default_reward> <gamma> <iteration_count>")

        if len(sys.argv) == 1:
            print("No arguments, will run hardcoded data instead")
            environment_file = "data/environment1.txt"
            default_reward = -0.04
            gamma = 1.0
            iteration_count = 20
            print(f'value_iterations.py "{environment_file}" "{default_reward}" "{gamma}" "{iteration_count}"')
            return (environment_file, default_reward, gamma, iteration_count)

        else:
            exit(-1)

    environment_file = sys.argv[1]
    if not os.path.isfile(environment_file):
        print(f"No such file: {environment_file} (training file)")
        exit(-2)

    default_reward = float(sys.argv[2])
    gamma = float(sys.argv[3])

    iteration_count = int(sys.argv[4])
    if iteration_count < 1:
        print(f"<iteration_count> ({iteration_count}) must be at least 1")
        exit(-5)
    
    return (environment_file, default_reward, gamma, iteration_count)

#
# Script
#

(environment_file, default_reward, gamma, iteration_count) = read_argv()
environment = Environment(environment_file, default_reward)

state_utilities = value_iteration(environment, iteration_count)

def print_tile(state, more_tiles_following):
    print(f"{state_utilities[state]:6.3f}", end = "")
    if more_tiles_following:
        print(",", end = "")

environment.for_each_tile(print_tile, print)