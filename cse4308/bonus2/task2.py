from os import sys
import numpy as np

class Node():
    def __init__(self, id):
        self.__id = id

    @property
    def id(self):
        return self.__id

    def probability(self, situation):
        raise "Unimplemented"

class MarginalNode(Node):
    def __init__(self, id, probability):
        super().__init__(id)
        self.__probability = probability

    def probability(self, situation):
        if super().id in situation and situation[super().id]:
            return self.__probability
        else:
            return 1 - self.__probability

class ConditionalNode(Node):
    def __init__(self, id, parents, probability_table):
        super().__init__(id)
        self.__parents = parents
        self.__probability_table = probability_table

    def probability(self, situation):
        lookup = tuple(map(
            lambda parent: parent.id in situation and situation[parent.id],
            self.__parents
        ))
        base_probability = self.__probability_table = lookup
        # should something else happen here?
        return base_probability

# node definitions

burglary = MarginalNode("B", 0.001)
earthquake = MarginalNode("E", 0.002)

alarm = ConditionalNode("A", [burglary, earthquake], {
    (True, True):   0.950,
    (True, False):  0.940,
    (False, True):  0.290,
    (False, False): 0.001
})

john_calls = ConditionalNode("J", (alarm), {
    (True): 0.90,
    (False): 0.05
})
mary_calls = ConditionalNode("M", (alarm), {
    (True): 0.70,
    (False): 0.01
})

node_list = [burglary, earthquake, alarm, john_calls, mary_calls] 
node_lookup = {
    node.id: node
        for node in node_list
}

def parse_variable(map, arg):
    map[node_lookup[arg[0]]] = arg[1] == "t"

def parse_args():
    knowns = {
        node.id: False
            for node in node_list
    } # type => boolean
    givens = {
        node.id: False
            for node in node_list
    } # type => boolean

    is_given = False
    for arg in sys.argv[1 : ]:
        if arg == "given":
            is_given = True
            continue

        parse_variable(givens if is_given else knowns, arg)

    return (knowns, givens)

print(parse_args())
