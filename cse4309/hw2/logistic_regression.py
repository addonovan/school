import numpy as np
import os.path
import sys

def read_input_file(file_name):
	"""
	Reads the input file and splits it into the component `matrix`, which
	contains all of the input data, and the `classes`, a column-vector
	which contains all of the classes. The classes are then processed to
	be restricted to 1 (if the class was of class 1) or 0 otherwise.
	"""
	matrix = np.asmatrix(np.loadtxt(file_name))
	classes = matrix[:, [-1]]
	matrix = matrix[:, :-1]

	with np.nditer(classes, op_flags=['readwrite']) as it:
		for x in it:
			x[...] = 1 if x == 1 else 0

	(_, attribute_count) = np.shape(matrix)
	return (matrix, classes, attribute_count)

def get_weights(attribute_count):
	"""
	Returns the initial weights column vector, given the number of attributes
	in the dataset.
	"""
	size = attribute_count + 1 if degree == 1 else (attribute_count * 2) + 1
	return np.zeros((size, 1))

def phi(input_data):
	"""
	Transforms the `input_data` row vector into another row vector by
	applying the phi functions related to the current `degree`.
	"""
	def degree1(input_data):
		output = np.asarray([1])
		for x in np.nditer(input_data):
			output = np.append(output, x)

		return output

	def degree2(input_data):
		output = np.asarray([1])
		for x in np.nditer(input_data):
			output = np.append(output, x)
			output = np.append(output, x ** 2)

		return output

	return degree1(input_data) if degree == 1 else degree2(input_data)

def calculate_probability(attributes, weights):
	"""
	Makes a prediction based on the described `attributes` and `weights` about
	which class the data belongs to.
	"""
	y = np.dot(phi(attributes), weights)
	y = 1 / (1 + np.exp(-y))
	return y

def train(training_data, classes, weights):
	"""
	Trains the model by applying the Iterative Reweighted Least Squares algorithm.
	This looks at all of the data at one time, and computes the differences between
	the current predictions and its real classes in order to compute the difference
	in weight values, which it then returns.
	"""
	size = np.shape(classes)[0]

	# the probabilities of each set of attributes describing a member
	# of class zero
	probabilities = np.asarray([
		calculate_probability(training_data[x], weights) 
			for x in range(0, size)
	])

	# big boy phi is just a matrix whose rows are the output of the phi
	# function applied to each of the values of an attribute.
	big_phi = np.asarray([
		phi(training_data[x])
			for x in range(0, size)
	])

	# create a diagonal matrix whose diagonal is (y * (1 - y))
	R = np.zeros((size, size))
	for i in range(0, size):
		R[i, i] = probabilities[i] * (1 - probabilities[i])

	# compute the hessian, `H = PHI' * R * PHI`
	hessian = np.matmul(
		np.matmul(np.transpose(big_phi), R), 
		big_phi
	)

	# compute the difference between what we've predicted and the
	# actual outputs `D = y - t`
	predicted_differences = np.subtract(probabilities, classes)

	# compute the overall change in the values of w:
	# `inv(H) * PHI' * D`
	delta_w = -np.matmul(
		np.matmul(np.linalg.pinv(hessian), np.transpose(big_phi)),
		predicted_differences
	)

	return delta_w

def test(row_id, testing_row, weights, real_class):
	"""
	Tests (and prints the required testing output) the given `testing_row` using
	the supplied `weights` and knowing the items `real_class`. This will calculate
	the accuracy value of this individual test, which can then be averaged to get
	the overall accuracy of the entire computed weights.
	"""

	# we'll assume we've predicted class one by default
	predicted_class = 1
	class_probability = calculate_probability(testing_row, weights)[0]

	# if the probability that it's class 1 is under 50%, then we should
	# guess that it's class 0, which involves "inverting" the probability
	# of the class as well (because this is a binary problem!).
	if class_probability < 0.5:
		predicted_class = 0
		class_probability = 1 - class_probability

	# We get 1 point for accuracy if we got the right class, or 0.5 points if it's
	# an dead tie. Otherwise (i.e. if we got it wrong), we'll get 0 points.
	accuracy = predicted_class == real_class
	if class_probability == 0.5:
		accuracy /= 2.0
		predicted_class = real_class # do this, because we aren't wrong

	elif predicted_class != real_class:
		accuracy = 0

	# nasty print statement that was real annoying to debug
	print(str.format(
		"ID={:5d}, predicted={:3d}, probability = {:.4f}, true={:3d}, accuracy={:4.2f}",
		row_id + 1,
		predicted_class,
		class_probability,
		real_class,
		accuracy
	))

	return accuracy

def cross_entropy_error(training, classes, weights):
	"""
	Computes and returns the cross entropy error of the current prediction model.

	This will recompute the probability that it belongs to class one, then make a 
	prediction of the class based on that. Then, if the class is correct, we'll add 
	`-log(class_prob)` to our accumulated error, else we'll add `-log(1 - class_prob)`.
	"""
	total_error = 0

	for (training, clazz) in zip(training, classes):
		# find out which class the model currently predicts
		probability = calculate_probability(training, weights)

		if clazz == 1:
			total_error -= np.log(probability)
		else:
			total_error -= np.log(1 - probability)

	return total_error

def read_argv():
	"""
	Reads the command line arguments and returns a 3-tuple of:
	`(training_file, degree, testing_file)`
	"""
	if len(sys.argv) < 4:
		print("Usage: <training_file> <degree> <testing_file>")
		print("	where degree is either:")
		print("		1")
		print("		2")
		exit(-1)

	training_file = sys.argv[1]
	if not os.path.isfile(training_file):
		print(f"No such file: {training_file} (training file)")
		exit(-2)

	degree = int(sys.argv[2])
	if degree < 1 or degree > 2:
		print(f"Unsupported degree: {degree}")
		print(f"Please choose either 1 or 2 as a degree")
		exit(-3)

	testing_file = sys.argv[3]
	if not os.path.isfile(testing_file):
		print(f"No such file: {testing_file} (testing file)")
		exit(-4)

	return (training_file, degree, testing_file)

#
# Main Script
#

(training_file, degree, testing_file) = read_argv()

# Get the information about the training data, and calculate/create initial
# values
(training_data, classes, attribute_count) = read_input_file(training_file)
weights = get_weights(attribute_count)
ce_error = cross_entropy_error(training_data, classes, weights)

# start the main body of training
while True:
	delta_w = train(training_data, classes, weights)
	weights += delta_w

	# stop if none of the weights have changed significantly
	# (we can do this before, because cross entropy is never referenced outside
	# of here, so eh)
	if np.sum([np.abs(x) for x in delta_w]) < 0.001:
		break

	# update the cross entropy error stuffs
	new_ce_error = cross_entropy_error(training_data, classes, weights)
	ce_diff = ce_error - new_ce_error
	ce_error = new_ce_error

	# If there was an insignificant difference in cross entropy, then we've
	# trained well enough
	if ce_diff < 0.001:
		break

# print our weights out
for (i, w) in np.ndenumerate(weights):
	print(f"w{i[0]}={w:.4f}")

# Read the testing data, then compute the overall accuracy and print it
(testing_data, classes, _) = read_input_file(testing_file)
accuracy = np.average([
	test(i, testing_data[i], weights, int(classes[i, 0]))
		for i in range(0, np.shape(testing_data)[0])
])
print(f'classification accuracy={accuracy:6.4f}')
