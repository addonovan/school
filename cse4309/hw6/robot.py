import numpy as np
from random import randint

from enum import Enum
import environment

class Action(Enum):
    """
    An action that the robot might try to perform in its environment.
    """
    Up = 0
    Left = 1
    Down = 2
    Right = 3

    def movement_possibilities(self):
        left = {
            Action.Up: Action.Left,
            Action.Left: Action.Down,
            Action.Down: Action.Right,
            Action.Right: Action.Up
        }
        right = {
            Action.Up: Action.Right,
            Action.Right: Action.Down,
            Action.Down: Action.Left,
            Action.Left: Action.Up
        }
        forward = {
            Action.Up: Action.Up,
            Action.Left: Action.Left,
            Action.Down: Action.Down,
            Action.Right: Action.Right,
        }
        return [
            (forward, 0.80),
            (left, 0.10),
            (right, 0.10)
        ]

    def try_delta(self):
        """
        Returns a possible result from the robot moving with this action.
        """
        choice = randint(1, 100)
        accumulator = 0
        for (actions, probability) in self.movement_possibilities():
            accumulator += probability * 100
            if accumulator >= choice:
                return actions[self]
        
        raise Exception("Failed to choose an action!")

    def delta(self):
        """
        The underlying mapping from an action to its `(delta_x, delta_y)`
        tuple. This is deterministic and will always result in the same 
        values.
        """
        if self == Action.Up:
            return (0, -1)
        elif self == Action.Down:
            return (0, 1)
        elif self == Action.Left:
            return (-1, 0)
        elif self == Action.Right:
            return (1, 0)
        raise Exception("Invalid action type!")

class ActionSet():
    action_chars = {
        Action.Up: "↑",
        Action.Down: "↓",
        Action.Left: "←",
        Action.Right: "→",
        None: "."
    }

    def __init__(self, actions):
        self._actions = actions
        self._utilities = {
            action: 0
                for action in actions
        }

    def __str__(self):
        output = "("
        for action in self._actions:
            output += f"{ActionSet.action_chars[action]} {self._utilities[action]:6.3f}, "
        return output[:-2] + ")"

    def __getitem__(self, key):
        return self._utilities[key]

    def __setitem__(self, key, value):
        self._utilities[key] = value

    def actions(self):
        """ A list of actions in this set """
        return self._actions

    def max(self):
        """ Returns the max of the associated utility values """
        return max(self._utilities.values())

    def max_action(self):
        """ Returns the action which yields the highest utility """
        best_utility = 0
        best_action = self._actions[0]
        for action in self._actions:
            if self[action] > best_utility:
                best_utility = self[action]
                best_action = action
        return best_action

class Robot():
    def __init__(self, environment, state):
        self._environment = environment
        if state is not None:
            self._state = state 
        else:
            self._state = environment.random_start_state()

    def __str__(self):
        output = ""

        def add_feature(state, _):
            nonlocal output
            tile = self._environment.tile_at(state)

            text = "  .  "
            if tile.terminal():
                text = f"{tile.reward():5.2f}"
            elif not tile.enterable():
                text = "  X  "

            if state == self._state:
                output += f"[{text}]"
            else:
                output += f" {text} "

        def add_newline():
            nonlocal output
            output += "\n"

        self._environment.for_each_tile(add_feature, add_newline)
        return output

    def state(self):
        """The current position/state of the robot"""
        return self._state

    def tile(self):
        """ The tile in the robot's environment """
        return self._environment.tile_at(self._state)

    def move(self, action):
        """
        Tries to move the robot from its current state with the
        given `action`. If the resuling change in position yields
        a tile that is possible to enter, then it will be entered. 

        This will return a completely new robot without modifying
        this robot's state at all.

        This action is entirely DETERMINISTIC, it will ALWAYS move
        with the specified `action` if it's possible. For the
        nondeterministic version, use `try_move`.
        """
        (x, y) = self.state()
        (dX, dY) = action.delta()
        prospective_state = (x + dX, y + dY)

        # check if we can actually move into the given tile,
        # and if we can, then update the state to reflect the
        # action.
        tile = self._environment.tile_at(prospective_state)
        if tile.enterable():
            return (type(self))(self._environment, prospective_state)
        else:
            return (type(self))(self._environment, self.state())

    def try_move(self, action):
        """
        Tries to move this robot using the given `action`; however, this
        is nondeterministic and might result in the robot moving either
        to the left or right of the given heading.
        """
        return self.move(action.try_delta()) 

    def action_set(self):
        tile = self.tile()
        if tile.terminal() or not tile.enterable():
            return ActionSet([None])
        else:
            return ActionSet([Action.Up, Action.Left, Action.Right, Action.Down])
