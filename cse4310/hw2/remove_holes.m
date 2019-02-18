function result = remove_holes(source)
    % Removes the holes from the given binary image. The input IMAGE must be
    % a binary image represented with uint8s.

    % because we're looking to fill holes, we care more about the black areas
    % than the white ones
    on = max(source, [], 'all');
    inverted_src = on - source;

    % find the labels for each connected components of black regions
    labels = bwlabel(inverted_src, 4);
    background_label = labels(1, 1);

    % fill all of the holes based on the connected component
    result = uint8(arrayfun(@(px) fill_holes(px), labels)); 

    function out = fill_holes(pixel_label)
        if pixel_label == background_label
            % if it's a member of the background set it to black
            out = uint8(0);
        
        else
            % otherwise, set it to white
            out = uint8(on);
        end
    end
end

