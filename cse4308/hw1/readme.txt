Austin Donovan
1001311620

Programming Language: Python 3

## Running

As this is a normal python3 file, it can be run like so:
    python3 find_route.py <input_file> <start> <end>

## Structure

At the bottom of the file is the main script, which handles parsing 
command-line arguments and the input file.

At the top of the file is the `Graph` class which just handles the
edges and makes them a bit easier to work with, by having every node
have a list to all of its adjacent nodes and the cost of the path to
them.

The search function is entirely written in the `uninformed_search`
method, which implements a uniform-cost graph search by using the following
nested methods:
- `tree_search` is the equivalent of the `Tree-Search` method from the slides
- `expand` is the equivalent of the `Expand` method from the slides
- `walk` will convert the recursive `SearchNode` result into a 
  list of actions to take, making printing easier.



