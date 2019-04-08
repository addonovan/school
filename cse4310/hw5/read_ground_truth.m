function [filenames, numbers, boxes] = read_ground_truth(filename)

% function [filenames, numbers, boxes] = read_ground_truth(filename)
%
% filenames is a cell array. filenames{i} is the filename of the i-th image
%    in the dataset.
% numbers is a 1-column matrix. numbers(i) is the number of faces in the i-th image
% boxes is a cell array. boxes{i} is a matrix of four columns. Each row
%    specifies the bounding box of a face, in the format [left_col,
%    top_row, width, height]. The number of rows of boxes{i} is equal to
%    the number of faces of the i-th image, so it is equal to numbers(i);


% read all lines of the file
fid = fopen(filename, 'r');
contents = textscan(fid, '%s');
contents = contents{1};
fclose(fid);

% count number of images
number = 0;

for i = 1:numel(contents)
    positions = findstr(contents{i}, '.jpg');
    if (numel(positions) == 1)
        number = number + 1;
    end
end
    
% initialize return values
filenames = cell(number, 1);
numbers = zeros(number, 1);
boxes = cell(number, 1);

index = 0;
for i = 1:number
    % read filename
    index = index + 1;
    current = contents{index};
    position = findstr(current, '/');
    filenames{i} = current((position+1):numel(current));
    filenames{i};
    
    % read number of faces
    index = index + 1;
    numbers(i) = str2num(contents{index});
    
    % read bounding boxes
    boxes{i} = zeros(numbers(i), 4);
    temp_boxes = zeros(numbers(i), 4);
    
    for j = 1:numbers(i)
        for k = 1:4
            index = index+1;
            temp_boxes(j, k) = str2num(contents{index});
        end
        boxes{i}(j,1) = temp_boxes(j, 2); % top
        boxes{i}(j,2) = temp_boxes(j, 2) + temp_boxes(j, 4) - 1; % bottom
        boxes{i}(j,3) = temp_boxes(j, 1); % left
        boxes{i}(j,4) = temp_boxes(j, 1) + temp_boxes(j, 3) - 1; % right
        index = index + 6;
    end
end