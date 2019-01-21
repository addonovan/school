function [top, bottom, left, right] = find_bounding_box(file)
    background = read_sequence_frame(file, 0); 
    source = read_greyscale(file);

    % find the difference between those frames and our frame
    diff = abs(source - background);
    figure(1);
    imshow(diff);

    % threshold the image
    thresh = 0.1;
    threshed = diff > thresh;
    figure(2);
    imshow(threshed, [0, 1.0]);

    % find connected components 
    [labels, number] = bwlabel(threshed, 4);
    colored = label2rgb(labels, @spring, 'c', 'shuffle');
    figure(2);
    imshow(colored);
   
    % find the largest connected component, then assume its the person 
    sizes = zeros(1, number);
    for i = 1:number
        component_image = (labels == i);
        counters(i) = sum(component_image(:));
    end
    [area, id] = max(counters);
    person = (labels == id);
    figure(3);
    imshow(person);

    % find the bounding box for the person
    nonempty_rows = find(sum(person'));
    [~, length] = size(nonempty_rows);
    top = nonempty_rows(1);
    bottom = nonempty_rows(length);

    nonempty_cols = find(sum(person));
    [~, length] = size(nonempty_cols);
    left = nonempty_cols(1);
    right = nonempty_cols(length);

    % draw the bounding box
    original = imread(file);
    original = draw_rectangle(original, [255, 255, 0], top, bottom, left, right);
    figure(4);
    imshow(original);

    function image = read_greyscale(file)
        image = double(imread(file)) / 255;
        image = (image(:, :, 1) + image(:, :, 2) + image(:, :, 3)) / 3;
    end

    function image = read_sequence_frame(file, frame)
        [seq, ~] = parse_frame_name(file);
        name = sprintf('%s%04d.tif', seq, frame);
        image = read_greyscale(name);
        clear name;
        clear seq;
    end

    function image = read_frame_offset(file, offset)
        [~, frame] = parse_frame_name(file);
        image = read_sequence_frame(file, frame + offset);
        clear frame;
    end
end

