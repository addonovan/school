# Austin Donovan
# add1620

import math
import numpy as np
import os.path
from random import randint
import sys

class DecisionForest():
    """
    A collection of DecisionTrees which will interact with all of its contained trees.
    """

    def __init__(self, attribute_chooser, tree_count, examples):
        """
        Constructs `tree_count` trees using the given `attribute_chooser` and training on
        the given `examples`.
        """
        self.trees = [
            DecisionTree(i, attribute_chooser, examples, distribution(examples))
                for i in range(0, tree_count)
        ]

    def classify(self, item):
        # add up all individual distributions into a single distribution
        dist = distribution([])
        for tree in self.trees:
            dist += tree.classify(item)

        # there's no need to average any of these values because the magnitude of
        # the distribution will be ordered even if we divide all by `n`.

        # return that and let the caller decide what's best (because some stuff
        # needs to be checked before output and accuracy classifications)
        return dist

class DecisionTree():
    """
    A single DecisionTree which will utilize the given attribute choice function
    and training inputs to determine which questions to ask.
    """

    def __init__(self, id, choose_attribute, examples, dist, depth = 1):
        """
        Constructs a new decision tree with the given id, attribute choice function,
        and example set. The `dist` is the distribution of the examples which precede
        it, in case this becomes a leaf node.
        """

        self.id = id
        self.depth = depth
        
        # this becomes a leaf if:
        #   we're too deep and need to be pruned
        #   we're out of examples
        if (pruning_threshold > 0 and self.depth > pruning_threshold) or len(examples) == 0:
            self.leaf = True
            self.dist = dist

        # otherwise, it'll be a branch
        else:
            self.leaf = False
            (self.gain, self.attribute, self.threshold) = choose_attribute(examples)
            examples_left = list(filter(
                lambda ex: ex.attr(self.attribute) < self.threshold,
                examples
            ))
            examples_right = list(filter(
                lambda ex: ex.attr(self.attribute) >= self.threshold,
                examples
            ))
            dist = distribution(examples)

            self.left = DecisionTree(
                id, 
                choose_attribute, 
                examples_left,
                dist,
                self.depth + 1
            )
            self.right = DecisionTree(
                id,
                choose_attribute,
                examples_right,
                dist,
                self.depth + 1
            )

        if depth == 1:
            self.print()

    def print(self):
        """
        Prints this tree in breadth-first order.
        """
        if self.depth != 1:
            raise "Only the root of each tree may be printed!"
        
        id = 1
        nodes = [self]
        while len(nodes) > 0:
            node = nodes.pop()
            if node.leaf:
                continue
            
            nodes.append(node.left)
            nodes.append(node.right)
            
            print(f"tree={self.id:2d}, node={id:3d}, feature={node.attribute:2d}, thr={node.threshold:6.2f}, gain={node.gain}")
            id += 1

    def classify(self, item):
        """
        Classifies an item using the model this decision tree has built up.
        This returns the distribution the leaf this tree traces down to for this
        example.
        """

        # if its a leaf, then just classify based on the distribution of the examples given
        if self.leaf:
            return self.dist

        # if its a branch, then we'll send it down the right branch
        else:
            if item.attr(self.attribute) < self.threshold:
                return self.left.classify(item)
            else:
                return self.right.classify(item)

def distribution(examples):
    """
    Calculates the distribution of classes for the given list of examples.
    The distribution is always an np row vector of `attribute_count` length.
    """

    dist = np.zeros(class_count)

    for ex in examples:
        dist[ex.type] += 1

    return dist

#
# Choosing attributes
#

def entropy(examples):
    """
    Calculates the entropy of the given example set.
    """

    # count up the number of each type in the set given
    type_counts = {}
    for ex in examples:
        if ex.type not in type_counts:
            type_counts[ex.type] = 1
        else:
            type_counts[ex.type] += 1

    # calculate the entropy by iterating over the partition
    size = len(examples)

    return sum([
        -(part_size / size) * np.log2(part_size / size)
            for part_size in type_counts.values()
    ])

def information_gain(examples, attribute_id):
    """
    Finds the threshold which gives the highest information gain
    for the given attribute with the given examples.
    """
    
    # if there are no examples, then there can be no information gain
    # at all
    if len(examples) == 0:
        raise "Cannot compute information gain when there is no information!"

    total_size = len(examples)
    base_entropy = entropy(examples)

    values = [
        ex.attr(attribute_id)
            for ex in examples
    ]

    lo = min(values)
    hi = max(values)
    step = (hi - lo) / (threshold_count + 1)
    
    max_gain = -1
    best_threshold = -1
    for j in range(0, threshold_count):
        threshold = lo + j * step
        
        examples_left = list(filter(
            lambda ex: ex.attr(attribute_id) < threshold,
            examples
        ))
        examples_right = list(filter(
            lambda ex: ex.attr(attribute_id) >= threshold,
            examples
        ))
        gain = base_entropy
        gain -= len(examples_left) / total_size * entropy(examples_left)
        gain -= len(examples_right) / total_size * entropy(examples_right)

        if gain > max_gain:
            max_gain = gain
            best_threshold = threshold

    return (max_gain, best_threshold)

def optimized_choice(examples):
    """
    Finds the best attribute with the best threshold from all possibilities
    at the moment, in order to maximize the information gain.
    """
    max_gain = -1
    best_attribute = -1
    best_threshold = -1

    for i in range(0, attribute_count):
        (gain, threshold) = information_gain(examples, i)
        if gain > max_gain:
            max_gain = gain
            best_attribute = i
            best_threshold = threshold

    return (max_gain, best_attribute, best_threshold)

def randomized_choice(examples):
    """
    Chooses an attribute at random, but selects the threshold which yields
    the highest information gain.
    """
    best_threshold = -1

    # choose an attribute at random, but that's the only difference
    i = randint(1, attribute_count) - 1
    (gain, best_threshold) = information_gain(examples, i)

    return (gain, i, best_threshold)

#
# Example Class
#

class Example():
    """
    An example input from either the training or testing data sets.
    This just makes it a little bit nicer to work with a class type instead
    of a matrix/column vector/row vector from numpy.
    """

    def __init__(self, row, typeid):
        """
        Creates a new example item given the row from the matrix and its
        corresponding type.
        """
        self.row = row
        self.type = int(typeid)
    
    def attr(self, index):
        """
        Selects only the value of the attribute with the given index.
        """
        return self.row[0, index]

    def label(self):
        """
        Gets the label of the class using the global `class_labels` array.
        """
        return class_labels[self.type]

#
# Uninteresting Stuff
#

def read_input_file(file_name, class_labels = None):
    """
    Reads the input file and splits it into the component `matrix`, which
    contains all of the input data, and the `classes`, a row-vector
    which contains all of the classes.
    """
    matrix = np.asmatrix(np.loadtxt(file_name))
    classes = matrix[:, [-1]]
    matrix = matrix[:, :-1]

    # arcane magic to make it an array instead of a matrix
    classes = np.squeeze(np.asarray(np.transpose(classes)))

    # the labels of the classes are used to assign each label to a number
    # for the entirety of the program
    if class_labels is None:
        class_labels = np.ndarray.tolist(np.unique(classes, axis=None))

    # convert data into an list of Examples
    examples = [
        Example(matrix[i, :], class_labels.index(classes[i]))
            for i in range(0, len(classes))
    ]

    (rows, attribute_count) = np.shape(matrix)
    return (examples, rows, attribute_count, class_labels)

def read_argv():
    """
    Reads the command line arguments and returns a 4-tuple of:
    `(training_file, test_file, option, pruning_threshold)`
    """
    if len(sys.argv) < 3:
        print("Usage: <training_file> <test_file> <option> <pruning_threshold>")

        if len(sys.argv) == 1:
            print("No arguments, will run hardcoded data instead")
            training_file = "data/yeast_training.txt"
            testing_file = "data/yeast_test.txt"
            option = "randomized"
            pruning_depth = 50
            print(f'decision_tree.py "{training_file}" "{testing_file}" "{option}" {pruning_depth}')
            return (training_file, testing_file, option, pruning_depth)

        else:
            exit(-1)

    training_file = sys.argv[1]
    if not os.path.isfile(training_file):
        print(f"No such file: {training_file} (training file)")
        exit(-2)

    testing_file = sys.argv[2]
    if not os.path.isfile(testing_file):
        print(f"No such file: {testing_file} (testing file)")
        exit(-3)

    option = sys.argv[3]
    supported_options = ["optimized", "randomized", "forest3", "forest15"]
    if option not in supported_options:
        print(f"<option> ({option}) must be one of:")
        for supported_option in supported_options:
            print(f"    {supported_option}")

        exit(-4)

    pruning_threshold = int(sys.argv[4])
    if pruning_threshold < 0:
        print(f"<pruning_threshold> ({pruning_threshold}) must be at least 0")
        exit(-5)
    
    return (training_file, testing_file, option, pruning_threshold)

#
# Main Script
#

threshold_count = 50
(train_file, test_file, option, pruning_threshold) = read_argv()
(training_examples, training_rows, attribute_count, class_labels) = read_input_file(train_file)
(testing_examples, testing_rows, _, _) = read_input_file(test_file, class_labels=class_labels)

class_count = len(class_labels)

# Choose the correct attribute-choice function
attribute_chooser = optimized_choice
if option != "optimized":
    attribute_chooser = randomized_choice

# choose the correct number of trees in our forest
tree_count = 1
if option == "forest3":
    tree_count = 3
    
elif option == "forest15":
    tree_count = 15

# construct the decision forest from our parameters
forest = DecisionForest(attribute_chooser, tree_count, training_examples)

# run our test data against it
points = 0
id = 0
for ex in testing_examples:
    classification = forest.classify(ex)
    max_size = np.max(classification)
    pclass = np.argmax(classification)

    current_points = 0
    if classification[ex.type] == max_size:
        current_points += 1 / (classification == max_size).sum()
        pclass = ex.type
    
    points += current_points
    print(f"ID={id:5d}, predicted={pclass:3d}, true={ex.type:3d}, accuracy={current_points:4.2f}")
    id += 1

print(f"classification accuracy={points / len(testing_examples):6.4f}")
