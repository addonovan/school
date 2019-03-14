function result = template_search(image, template, normalize)
    % initialize all values to an invalid state
    result = 150000000 * ones(size(image));

    % normalize template for its brightness
    template = double(normalize(template)); 

    % compute the boundaries of the image
    [t_rows, t_cols] = size(template);
    [im_rows, im_cols] = size(image);

    half_cols = floor(t_cols / 2);
    half_rows = floor(t_rows / 2);

    left_bound = half_cols + 1;
    right_bound = im_cols - half_cols;
    top_bound = half_rows + 1;
    bottom_bound = im_rows - half_rows;

    boundaries = @(i, j) [j - half_cols, i - half_rows, t_cols - 1, t_rows - 1];

    % build the result data from the sum-of-square-differences at each pixel 
    for col = left_bound : right_bound 
        for row = top_bound : bottom_bound
            bounds = boundaries(row, col);
            sources = imcrop(image, bounds);
            sources = double(normalize(sources));
            result(row, col) = sum(sum((sources - template) .^ 2));
        end
    end
end

