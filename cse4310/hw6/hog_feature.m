function result = hog_feature(image, top, left, block_size)
    bins = 9;
    [max_row, max_col] = size(image);
    [grad_norm, orientations] = image_gradient(image);

    subblock_width = floor(block_size / 2);
    right = left + subblock_width;
    bottom = top + subblock_width;

    % compute and concatenate the sub histograms
    result = [
        o_histo(top,    left,   subblock_width);
        o_histo(top,    right,  subblock_width);
        o_histo(bottom, left,   subblock_width);
        o_histo(bottom, right,  subblock_width)
    ]; 

    % finally, normalize the vector acoording to the process in the slides
    result = result / norm(result);
    result(result > 0.2) = 0.2;
    result = result / norm(result);

    function histogram = o_histo(top, left, width)
        histogram = zeros([9, 1]);

        span = [0 : width - 1];
        for u = top + span
            for v = left + span
                if u < 0 || v < 0 || u > max_row || v > max_col
                    continue;
                end

                bin = bin_for_angle(orientations(u, v));
                histogram(bin) = histogram(bin) + grad_norm(u, v);
            end
        end 
    end

    function bin = bin_for_angle(angle)
        boundary = 180 / bins;
        bin = 1 + floor(angle / boundary);
    end

    function [grad_norm, orientations] = image_gradient(image)
        dx = [-1, 0, 1];
        dy = dx';
        grad_x = imfilter(image, dx);
        grad_y = imfilter(image, dy);

        grad_norm = sqrt(grad_x .^ 2 + grad_y .^ 2);
        orientations = atan2d(grad_y, grad_x);

        % make sure all angles are within [0, 180)
        negatives = orientations < 0;
        orientations(negatives) = orientations(negatives) + 180;
        orientations(orientations == 180) = 0; 
    end
end

