#!/bin/python3
import io
import sys

class Graph():
    def __init__(self):
        self.__connections = {}
    
    def __add_destination(self, start, end, weight):
        if start not in self.__connections:
            self.__connections[start] = {}
        
        self.__connections[start][end] = weight

    def add_connection(self, start, end, weight):
        self.__add_destination(start, end, weight)
        self.__add_destination(end, start, weight)

    def get_weight(self, start, end):
        if end in self.__connections[start]:
            return self.__connections[start][end]
        else:
            return None

    def edges(self, node):
        edges = []
        destinations = self.__connections[node]
        for destination in destinations:
            edges.append(Edge(destination, destinations[destination]))

        return edges

class Edge():
    def __init__(self, destination, weight):
        self.destination = destination
        self.weight = weight
    def __repr__(self):
        return self.destination + "=" + self.weight

def uninformed_search(start, end, graph):
    """Performs an uninformed tree search strategy (uniform-cost search)"""

    class SearchNode():
        def __init__(self, step_cost, name, predecessor):
            self.path_cost = predecessor.path_cost + step_cost if predecessor is not None else 0
            self.step_cost = step_cost
            self.name = name
            self.predecessor = predecessor
        def __repr__(self):
            return self.predecessor.name + "->" + self.name + "=" + self.path_cost

    class Problem():
        def __init__(self, start, end, graph, goal_predicate):
            self.start = start
            self.end = end
            self.graph = graph
            self.is_goal = goal_predicate
            self.visited_nodes = []

    nodes_expanded = 0
    nodes_generated = 0
    max_nodes_in_memory = 0

    def tree_search(problem, fringe):
        nonlocal nodes_generated
        nonlocal nodes_expanded
        nonlocal max_nodes_in_memory

        # create the initial node
        nodes_generated = 1
        fringe = [SearchNode(0, problem.start, None)]

        while len(fringe) > 0:
            # keep track of some metrics
            max_nodes_in_memory = max(max_nodes_in_memory, len(fringe))
            nodes_expanded += 1

            node = fringe.pop(0)
            while node.name in problem.visited_nodes:
                # ran out of nodes in the fringe
                if len(fringe) == 0:
                    return None

                node = fringe.pop(0)

            if problem.is_goal(node):
                return node
            
            # make sure we never visit this node again, since we'll be expanding it
            problem.visited_nodes.append(node.name)

            # keep the fringe sorted by the path cost
            fringe.extend(expand(node, problem))
            fringe = sorted(
                fringe, 
                key=lambda node: node.path_cost
            )

        return None

    def expand(node, problem):
        nonlocal nodes_generated
        nodes = []
        for edge in problem.graph.edges(node.name):
            nodes.append(SearchNode(edge.weight, edge.destination, node))
        
        nodes_generated += len(nodes)
        return nodes

    initial_problem = Problem(start, end, graph, lambda x: x.name == end)
    result = tree_search(initial_problem, [])

    # convert the resulting nested structure into an actual path of (start, end, cost)
    def walk(node):
        pred = node.predecessor
        if pred is None:
            return []
        
        path = walk(pred)
        path.append((pred.name, node.name, node.step_cost))
        return path

    path = walk(result) if result is not None else None
    return (path, nodes_expanded, nodes_generated, max_nodes_in_memory)

#
# General Overhead
#

def parse_file(f):
    graph = Graph()

    for line in f:
        # as per requirments, just skip this last line
        if "END OF INPUT" in line:
            break

        (start, end, weight) = line.split(" ")
        graph.add_connection(start, end, int(weight))

    return graph

if len(sys.argv) >= 3:
    file_name = sys.argv[1]
    source = sys.argv[2]
    target = sys.argv[3]
else:
    print("Usage: python3 ./find_route.py <file_name> <start_node> <end_node>")
    sys.exit(1)

# parse the file
graph = None
with open(file_name, "r") as f:
    graph = parse_file(f)


# perform the search function
(path, nodes_expanded, nodes_generated, max_nodes_in_memory) = uninformed_search(source, target, graph)

# calculate the total cost of the path
distance = sum(x[2] for x in path) if path is not None else "infinity"

# print the metrics
print("nodes expanded: " + str(nodes_expanded))
print("nodes generated: " + str(nodes_generated))
print("max nodes in memory: " + str(max_nodes_in_memory))
print("distance: " + str(distance) + ".0 km")
print("route:")
if path is not None:
    for edge in path:
        print(edge[0] + " to " + edge[1] + ", " + str(edge[2]) + ".0 km")
else:
    print("none")