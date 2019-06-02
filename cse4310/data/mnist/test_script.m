%% An example of using load_mnist

clear all;

load_mnist;

%example value for i
i = 434; 

% get the i-th image out of test digits
var = mnist_digits(:,:,i);

% show the i-th image in a figure
imshow(var, []);  

% the label is a number between 0 and 9, telling us
% which digit is contained in the i-th image.
label = mnist_labels(i);  

% print the label.
disp(label); 

%% An example of using load_mnist_selection

clear all;

%example value for indices
indices = 431:440;

% get the i-th image out of test digits
[labels, digits] = load_mnist_selection('scrambled_mnist10000.bin', indices);
var = digits(:,:,4); % this should be digit 434 of original dataset

% show the i-th image in a figure
imshow(var, []);  

% the label is a number between 0 and 9, telling us
% which digit is contained in the i-th image.
label = labels(4);  

% print the label.
disp(label); 
