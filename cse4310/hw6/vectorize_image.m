function out = vectorize_image(data)
    [width, height, count] = size(data);
    out = nan([width * height, count]);
    for i = 1 : count
        image = data(:, :, i);
        out(:, i) = image(:); 
    end
end

