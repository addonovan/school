function out = normalize_image(image_vector)
    count = size(image_vector, 2);
    dimensions = size(image_vector, 1);

    out = zeros([dimensions, count]);
    for index = 1 : count
        vec = image_vector(:, index);

        vec = double(vec);
        vec = vec(:);
        vec = vec - mean(vec);
        vec = vec / std(vec);
        vec(isnan(vec)) = 0;

        out(:, index) = vec;
    end
end

