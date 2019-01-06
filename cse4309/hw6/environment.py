from random import randint

class TileType():
    @staticmethod
    def create_open(reward):
        """ Creates a new open tile with the `default_reward`. """
        return TileType(".", True, False, reward)

    @staticmethod
    def create_closed():
        """ Creates a new closed tile. """
        return TileType("X", False, False, None)

    @staticmethod
    def create_terminal(reward):
        """ Creates a new terminal tile with the given `reward`. """
        return TileType(str(reward), True, True, reward)

    def __str__(self):
        return self._text

    def __init__(self, text, enterable, terminal, reward):
        self._text = text
        self._enterable = enterable
        self._terminal = terminal
        self._reward = reward

    def enterable(self):
        """ If the robot is able to enter this tile. """
        return self._enterable

    def terminal(self):
        """ If this is a terminal tile. """
        return self._terminal

    def starting_tile(self):
        """ If this tile qualifies as a possible starting tile. """
        return self._enterable and not self._terminal

    def reward(self):
        """ The reward for reaching this tile. """
        return self._reward

class Environment():
    def __init__(self, file, default_reward):
        def tiletype_for(text):
            """
            Parses the given `text` into the TileType it represents. 
            """
            if text == ".":
                return TileType.create_open(default_reward)
            elif text == "X":
                return TileType.create_closed()
            else:
                return TileType.create_terminal(float(text))

        with open(file) as f:
            self._tiles = list(map(
                lambda line: [
                    tiletype_for(tile_text)
                        for tile_text in line.strip().split(",")
                ],
                f.readlines()
            ))

        self._max_y = len(self._tiles) - 1
        self._max_x = len(self._tiles[0]) - 1

    def __str__(self):
        output = ""
        for row in self._tiles:
            for tile in row:
                output += f"{str(tile):5s} "
            output += "\n"
        return output

    def random_start_state(self):
        """ Finds a random valid starting position """

        # build a list of all starting states
        starting_states = []
        for x in range(0, self._max_x + 1):
            for y in range(0, self._max_y + 1):
                state = (x, y)
                if self.tile_at(state).starting_tile():
                    starting_states.append(state)

        # choose one at random
        choice = randint(0, len(starting_states) - 1)
        return starting_states[choice]

    def state_list(self):
        """ 
        Creates a list of all states (position tuples) that 
        are valid for a robot to be in.
        """
        states = []
        self.for_each_tile(lambda state, _: states.append(state))
        return states

    def for_each_tile(self, action, on_row_end = None):
        """
        Performs the given `action` on all tuples which each represent
        a tuple in this environment. This iteration is always performed
        from top-->bottom and left-->right. This receiver is also 
        optionally passed a parameter which is `True` if there's another
        tile in this row, and `False` otherwise.

        If `on_row_end` is not `None`, then it will be called (with no
        parameters) after each row in the environment has been processed
        with `action`.
        """
        for j in range(0, self._max_y + 1):
            for i in range(0, self._max_x + 1):
                action((i, j), i != self._max_x)
            if on_row_end is not None:
                on_row_end()

    def tile_at(self, coordinate):
        """
        Gets the tile type at the given (x, y) `coordinate` pair. If the
        coordinate is out of bounds, then a closed tile will be returned
        in its place.
        """
        (x, y) = coordinate
        if x < 0 or x > self._max_x or y < 0 or y > self._max_y:
            return TileType.create_closed() 
        return self._tiles[y][x]
