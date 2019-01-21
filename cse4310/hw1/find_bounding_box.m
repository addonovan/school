function [top, bottom, left, right] = find_bounding_box(file)
    % read the provided image and images +/- n frames 
    source = read_greyscale(file);
    past = read_relative_frame(file, -10);
    future = read_relative_frame(file, 10);

    % find the difference between the frames 
    diff = min( ...
        abs(source - past), ...
        abs(future - source) ...
    );

    % threshold the image
    thresh = 0.05;
    threshed = diff > thresh;

    % find connected components 
    [labels, number] = bwlabel(threshed, 4);
    colored = label2rgb(labels, @spring, 'c', 'shuffle');
   
    % find the largest connected component, assuming it's the person 
    sizes = zeros(1, number);
    for i = 1:number
        component_image = (labels == i);
        counters(i) = sum(component_image(:));
    end
    [area, id] = max(counters);
    person = (labels == id);

    % find the top and bottom of the person 
    % (the first and last rows with a nonzero sum) 
    nonempty_rows = find(sum(person'));
    [~, length] = size(nonempty_rows);
    top = nonempty_rows(1);
    bottom = nonempty_rows(length);

    % find th left and right of the person
    % (the first and last columns with a nonzero sum)
    nonempty_cols = find(sum(person));
    [~, length] = size(nonempty_cols);
    left = nonempty_cols(1);
    right = nonempty_cols(length);

    % draw the bounding box
    original = imread(file);
    original = draw_rectangle(original, [255, 255, 0], top, bottom, left, right);
    figure(1);
    imshow(original);

    function image = read_greyscale(file)
        image = double(imread(file)) / 255;
        image = (image(:, :, 1) + image(:, :, 2) + image(:, :, 3)) / 3;
    end

    function image = read_relative_frame(file, diff)
        [seq, frame] = parse_frame_name(file);
        name = sprintf('%s%04d.tif', seq, frame + diff);
        image = read_greyscale(name);
        clear name;
        clear frame;
        clear seq;
    end
end

