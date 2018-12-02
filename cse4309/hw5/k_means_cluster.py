# Austin Donovan
# add1620

import math
import numpy as np
import os.path
from random import randint
import sys

#
# Clustering
#

def dist(mean, example):
    """
    Calculates the Euclidean distance between the given cluster `mean` and the
    `example`.
    """
    return np.linalg.norm(example.row - mean)

def partition(examples):
    """
    Partitions the examples into a list where each cluster index contains
    a list of all examples in that cluster.
    """

    cluster_examples = [[] for _ in range(0, cluster_count)]
    for example in examples:
        cluster_examples[example.type].append(example)

    return cluster_examples

def class_average(examples):
    """
    Computes the average of all of the data points given. This will
    return an n-tuple (where n is `attribute_count`) of the averages
    of each corresponding feature in the set.

    This will return a `(1, attribute_count)` np array.
    """
    averages = np.zeros((1, attribute_count))

    # if we have no examples, then we'll just end early
    if len(examples) == 0:
        return averages

    for ex in examples:
        averages += ex.row

    return averages / len(examples)

def closest_cluster(cluster_averages, example):
    """
    Takes in a (python) list of `cluster_averages`, which are 
    `(1, attribute_count)` np arrays, and an example and classifies
    the `example` with the index of the cluster average which it most
    nearly matches.
    """

    # just do a normal search for a minimum
    best_index = -1
    best_distance = 10000000
    for (i, cluster_average) in enumerate(cluster_averages):
        distance = dist(cluster_average, example)

        # easy trick to get a single comparison and negation to check for
        # invalid number too
        if distance < best_distance:
            best_index = i
            best_distance = distance

    return best_index

def update_clusters(examples, cluster_averages):
    """
    Takes the given `examples` and goes through a single iteration step of
    reclustering the data. The `cluster_examples` is a list where each cluster
    index contains a list of examples which are members of that cluster.
    """
    # find the closest cluster and move the example to that one
    for example in examples:
        example.type = closest_cluster(cluster_averages, example)

def clustering_error(examples):
    """
    Computes the new `partition` of the given examples, and then the
    corresponding error rate (the sum of all distances of examples from their
    corresponding cluster's mean).

    This will return `(error_rate, cluster_averages)`, so that the average computed
    here may be fed back into `update_clusters`.
    """

    cluster_examples = partition(examples)
    cluster_averages = [
        class_average(cluster_examples[class_id])
            for class_id in range(0, cluster_count)
    ]

    # accumulate the distances between each example and its cluster's average
    # into an error value
    error = 0
    for example in examples:
        error += dist(cluster_averages[example.type], example)

    return (error, cluster_averages)

#
# Cluster Assurance
#

class RandomizedClusterAssigner:
    def __init__(self, examples):
        self.example_count = len(examples)
        self.chosen_examples = set([])

    def next(self):
        while True:
            index = randint(0, self.example_count - 1)

            if index not in self.chosen_examples:
                self.chosen_examples.add(index)
                return index

class SequentialClusterAssigner:
    def __init__(self):
        self.offset = 0

    def next(self):
        value = self.offset
        self.offset += 1
        return value

def ensure_all_clusters(examples):
    """
    Ensures that all clusters are present in the given example set.
    """

    # determine the type of assigner we want, if there are a lot
    # of classes, relative the the example count, then we're going
    # to want to be sequential; otherwise, we can afford the perf.
    # hit and take a better random sample
    assigner = None
    example_count = len(examples)

    if cluster_count > example_count / 2:
        assigner = SequentialClusterAssigner()

    else:
        assigner = RandomizedClusterAssigner(examples)

    # choose `cluster_count` examples at random (without replacement)
    # and assign them to each to a cluster
    for i in range(0, min(example_count, cluster_count)):
        next_index = assigner.next()
        ex = examples[next_index]
        ex.type = i
    
#
# Example Class
#

class Example():
    """
    An example input from either the training or testing data sets.
    This just makes it a little bit nicer to work with a class type instead
    of a matrix/column vector/row vector from numpy.
    """

    def __init__(self, row):
        """
        Creates a new example item given the row from the matrix and
        will randomly assign it to a cluster.
        """
        self.row = row
        self.type = randint(0, cluster_count - 1)
    
    def attr(self, index):
        """
        Selects only the value of the attribute with the given index.
        """
        return self.row[0, index]

#
# Uninteresting Stuff
#

def read_input_file(file_name):
    """
    Reads the input file and splits it into a list of `examples`, along with
    the `attribute_count`, i.e. the number of attributes/features each example
    has.
    """
    matrix = np.asmatrix(np.loadtxt(file_name))
    matrix = matrix[:, :-1]

    (rows, attribute_count) = np.shape(matrix)

    # convert data into an list of Examples
    examples = [
        Example(matrix[i, :])
            for i in range(0, rows)
    ]

    return (examples, attribute_count)

def read_argv():
    """
    Reads the command line arguments and returns a 3-tuple of:
    `(data_file, cluster_count, iteration_count)`
    """
    if len(sys.argv) < 3:
        print("Usage: <data_file> <cluster_count> <iteration_count>")

        if len(sys.argv) == 1:
            print("No arguments, will run hardcoded data instead")
            data_file = "data/yeast_test.txt"
            cluster_count = 20
            iteration_count = 2
            print(f'k_means_cluster.py "{data_file}" "{cluster_count}" "{iteration_count}"')
            return (data_file, cluster_count, iteration_count)

        else:
            exit(-1)

    data_file = sys.argv[1]
    if not os.path.isfile(data_file):
        print(f"No such file: {data_file} (training file)")
        exit(-2)

    cluster_count = int(sys.argv[2])
    if cluster_count < 0:
        print(f"<cluster_count> ({cluster_count}) must be at least 0")
        exit(-3)

    iteration_count = int(sys.argv[3])
    if iteration_count < 0:
        print(f"<iteration_count> ({iteration_count}) must be at least 0")
        exit(-3)
    
    return (data_file, cluster_count, iteration_count)

#
# Main Script
#

# read stuff from stdin and the data file
(data_file, cluster_count, iteration_count) = read_argv()
(examples, attribute_count) = read_input_file(data_file)

# sanity check: make sure we don't have too many clusters
if cluster_count > len(examples):
    raise Exception(f'Cannot cluster {len(examples)} points into {cluster_count} clusters!')

# make sure all clusters are represented
ensure_all_clusters(examples)

# Calculate the initial error rate
(error, cluster_averages) = clustering_error(examples)
print(f'After initialization: error = {error:.4f}')

# Run the algorithm through the given number of iterations
for i in range(0, iteration_count):
    update_clusters(examples, cluster_averages)
    (error, cluster_averages) = clustering_error(examples)
    print(f'After iteration {i + 1}: error = {error:.4f}')
