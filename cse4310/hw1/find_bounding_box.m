function [top, bottom, left, right] = find_bounding_box(file)
    [top, bottom, left, right] = find_person(file);    

    % draw the bounding box
    original = imread(file);
    original = draw_rectangle(original, [255, 255, 0], top, bottom, left, right);
    figure(1);
    imshow(original);
end

