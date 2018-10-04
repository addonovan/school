Iterative Reweighted Least Squares
===

This assignment implements the IRLS algorithm for linear classification.

The program will read in all of the training data at one time and process
all of the data at one time, to produce a set of changes to the weights
of our functions. This process will be repeated until we hit one of our
error conditions:  
1. The absolute change in weight values is less than 0.001
2. The change in cross-entropy error is less than 0.001

This program will also take in a parameter called `degree` which controls
which set of basis functions the program will use:  
1. `phi(x) = (1, x1, x2, ..., xD)'`
2. `phi(x) = (1, x1, x1^2, x2, x2^2, ..., xD, xD^2)`'
(where `a'` means the transpose of a).

For ease of programming, `x`, `phi(x)`, and `weights` are represented as
row vectors, even though they are meant to be represented as column vectors
mathemtically.

This is designed to support any training and test file pair from the UCI
Machine Learning dataset. Specifically, this was tested on `pendigits`,
`satellite`, and `yeast`.


