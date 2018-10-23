import math
import numpy as np
import os.path
import sys

class Layer:
    """
    A layer in a neural network. This has only backwards connections,
    so no layer has any idea if it's an output layer or not. If the
    layer has no previous layer (i.e. `previous_layer is None`), then
    it'll function as an input layer, and must have its `values` property
    set before testing on a given input.

    Attributes:
    - id = a unique id for this layer (for debugging)
    - node_count = N
    - previous_layer = The layer that came before this (None if input layer)
    - values = 1xN row vector of the testing data (input layer only)
    - weights = MxN matrix of weights, each column is the weights for a
        single perceptron in this layer.
    - biases = 1xN row vector of bias weights for each perceptron in the network

    Some notation used in this class:
    - M = the number of perceptrons in the *previous* layer
    - N = the number of perceptrons in this layer
    """

    def __init__(self, id, node_count, previous_layer):
        self.id = id
        self.node_count = node_count
        self.previous_layer = previous_layer
        self.next_layer = None

        # input layers have nothing but input values
        if previous_layer is None:
            self.values = np.zeros((1, node_count))

        # other layers have a MxN matrix of random values between -0.05
        # and 0.05 for initial weights
        else:
            # each column is a list of weights for a single perceptron in this layer
            # the *last* row in each is the bias weight
            self.weights = np.random.uniform(
                -0.05,
                0.05,
                (previous_layer.node_count, node_count)
            )
            self.biases = np.random.uniform(
                -0.05,
                0.05,
                node_count
            )

    def __feedforward(self, inputs):
        """
        Calculates the outputs for this layer given the overall input values (which
        will be passed down to the input layer, before the outputs are propagated
        back up to the front of the network).
        """

        if self.previous_layer is None:
            self.outputs = inputs

        else:
            self.previous_layer.__feedforward(inputs)
            
            # take the inputs and multiply them against their respective weights
            inputs = self.previous_layer.outputs
            outputs = np.matmul(inputs, self.weights)

            # add the biases in element-wise, (because each column is a dot product,
            # so it's just adding a 1 * bias to the sum)
            outputs += self.biases

            # apply the activation function
            self.outputs = sigmoid(outputs)

    def __backpropagate(self, expected):
        """
        Uses the stored outputs from this layer and forward layers to updates the
        weights of this layer and all previous layers.
        """
        if self.previous_layer is None:
            return
        
        if self.next_layer is None:
            # weight updates for the output layer is pretty simply, just
            # (zj - tj) * zj * (1-zj)
            #   where   zj = output for unit j
            #           tj = corresponding expected output
            self.weight_updates = np.asarray([
                (self.outputs[j] - expected[j]) * d_sigmoid(self.outputs[j])
                    for j in range(0, self.node_count)
            ])

        else:
            # weight updates for hidden layers depending on the next layer
            # sum(deltau * wuj) * zj * (1 - zj)
            #   where   zj = ouput for unit j
            #           deltau = the update for the weights of unit u in the next layer
            #           wuj = the weight for unit u corresponding to this unit j
            self.weight_updates = np.zeros(self.node_count)
            for j in range(0, self.node_count):
                sum = 0
                for u in range(0, self.next_layer.node_count):
                    sum += self.next_layer.weight_updates[u] * self.next_layer.weights[j, u]

                self.weight_updates[j] = sum * d_sigmoid(self.outputs[j])

        # compute & apply the rest of the network's weight updates
        self.previous_layer.__backpropagate(expected)

        # finally apply the weight updates to the current layer
        for j in range(0, self.node_count):
            update = self.weight_updates[j]
            for i in range(0, self.previous_layer.node_count):
                delta = learning_rate * update * self.previous_layer.outputs[i]
                self.weights[i, j] -= delta

            # apply the update to the bias weight. The 1 is the stand-in for the previous
            # value because the bias input is fixed at one
            self.biases[j] -= learning_rate * update * 1

    def train(self, inputs, expected_class):
        """
        Trains the neural network (this must be called on an output layer) by giving
        it a sample input and an expected output.
        """

        if self.next_layer is not None:
            raise "Layer.train() can only be invoked on the output layer!"

        expected = [
            1 if expected_class == i else 0
                for i in range(0, self.node_count)
        ]
        inputs = np.squeeze(np.asarray(inputs))
        self.__feedforward(inputs)
        self.__backpropagate(expected)

    def predict(self, inputs):
        """
        Predicts the class given the inputs. This will return a list of the classes
        each with the highest weight.
        """
        if self.next_layer is not None:
            raise "Layer.predict() can only be invoked on the output layer!"

        inputs = np.squeeze(np.asarray(inputs))
        self.__feedforward(inputs)
        
        max = np.amax(self.outputs)
        classes = []
        for i in range(0, self.node_count):
            if self.outputs[i] == max:
                classes.append(i)
        
        return classes

def build_network(layers, class_count, attribute_count, units_per_layer):
    """
    Builds the network and fully attaches all of the layers. Then returns
    a tuple of the input and output layers *only*.
    """
    input_layer = Layer("input", attribute_count, None)
    
    prev_layer = input_layer
    for i in range(1, layers + 1):
        new_layer = Layer(i, units_per_layer, prev_layer)
        prev_layer.next_layer = new_layer
        prev_layer = new_layer
    
    output_layer = Layer("output", class_count, prev_layer)
    prev_layer.next_layer = output_layer
    return output_layer

def test(id, output, row, rclass, shouldPrint=False):
    rclass_index = class_labels.index(rclass)
    pclass = output.predict(row)

    accuracy = 1
    if rclass_index in pclass:
        accuracy /= len(pclass)
        pclass = rclass

    else:
        accuracy = 0
        pclass = class_labels[pclass[0]] # just take the first class, it doesn't matter

    if shouldPrint:
        print(f"ID={int(id):5d} predicted={int(pclass):3d} true={int(rclass):3d} accuracy={accuracy:4.2f}")

    return accuracy

#
# Uninteresting Stuff
#

def read_input_file(file_name):
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

    # normalize input data
    matrix = matrix / matrix.max()

    (rows, attribute_count) = np.shape(matrix)
    return (matrix, classes, rows, attribute_count)

def sigmoid(x):
    """
    Computes the sigmoid of a numpy array, x.
    """
    fun = np.vectorize(lambda x: 1 / (1 + math.exp(-x)))
    return fun(x)

def d_sigmoid(s):
    return s * (1 - s)

def read_argv():
    """
    Reads the command line arguments and returns a 5-tuple of:
    `(training_file, test_file, layers, units_per_layer, rounds)`
    """
    if len(sys.argv) == 1:
        print("No arguments, will run hardcoded data instead")
        return ("data/pendigits_training.txt", "data/pendigits_test.txt", 2, 20, 50)

    if len(sys.argv) < 4:
        print("Usage: <training_file> <test_file> <layers> <units_per_layer> <rounds>")
        exit(-1)

    training_file = sys.argv[1]
    if not os.path.isfile(training_file):
        print(f"No such file: {training_file} (training file)")
        exit(-2)

    testing_file = sys.argv[2]
    if not os.path.isfile(testing_file):
        print(f"No such file: {testing_file} (testing file)")
        exit(-4)

    layers = int(sys.argv[4])
    units_per_layer = int(sys.argv[5])
    rounds = int(sys.argv[6])
    
    return (training_file, testing_file, layers, units_per_layer, rounds)

#
# Main Script
#

# hard coded properties for now
(training_file, test_file, layers, units_per_layer, rounds) = read_argv()
learning_rate = 0.98 ** (rounds - 1)

(data, classes, rows, attribute_count) = read_input_file(training_file)
(testing_data, test_classes, testing_rows, _) = read_input_file(test_file)

class_labels = list(set(classes))
class_count = len(class_labels)

output_layer = build_network(layers, class_count, attribute_count, units_per_layer)

# train the network
for i in range(0, rounds):
    # go through a round of training
    for j in range(0, rows):
        output_layer.train(data[j, :], class_labels.index(classes[j]))

# print the total accuracy afterwards
accuracy = np.average([
    test(i, output_layer, testing_data[i, :], class_labels.index(test_classes[i]), shouldPrint=True)
        for i in range(0, testing_rows)
])
print(f"classification accuracy={accuracy:6.4f}")
