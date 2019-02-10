function [top, bottom, left, right] = find_person(file)
    % read the provided image and images +/- n frames 
    source = read_greyscale(file);
    background = read_frame(file, @(~) 0);

    % find the difference between the frames 
    diff = abs(source - background);

    % threshold the image
    thresh = 0.10;
    threshed = diff > thresh;

    % apply a 4-connected neighbor dilation
    dilation = [0, 1, 0; 1, 1, 1; 0, 1, 0];
    dilated = imdilate(threshed, dilation);
    
    % special case for the background (since this is also called by person_present)
    % if there's absolutely no difference between this frame and the background, then
    % we'll just return -1 for all dimensions and avoid any possible errors
    if all(dilated(:) == 0) 
        top = -1;
        bottom = -1;
        left = -1;
        right = -1;
        return
    end

    % find connected components 
    [labels, number] = bwlabel(dilated, 4);
    colored = label2rgb(labels, @spring, 'c', 'shuffle');
   
    % find the largest connected component, assuming it's the person 
    stats = regionprops(labels, 'Area');
    [area, id] = max([stats.Area]);
    if isempty(area)
        disp(file);
    end
    person = (labels == id);

    % find the top, bottom, left, and right-most pixels by finding all 
    % non-zero pixels, converting those indices to subscripts, then
    % finding the min/max of both the rows and columns in that
    [rows, columns] = ind2sub(size(person), find(person));
    extrema = minmax(rows');
    top = extrema(1);
    bottom = extrema(2);

    extrema = minmax(columns');
    left = extrema(1);
    right = extrema(2);

    function image = read_greyscale(file)
        image = double(imread(file)) / 255;
        image = (image(:, :, 1) + image(:, :, 2) + image(:, :, 3)) / 3;
    end

    function image = read_frame(file, frame_map)
        [seq, frame] = parse_frame_name(file);
        name = sprintf('%s%04d.tif', seq, frame_map(frame));
        image = read_greyscale(name);
        clear name;
        clear frame;
        clear seq;
    end
end

