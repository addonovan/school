function [result, max_scales] = multiscale_correlation(image, template, scales)

% function [result, max_scales] = multiscale_correlation(image, template, scales)
%
% for each pixel (i,j), search over the specified scales, and record:
% - in result(i,j), the max normalized correlation score for that pixel
%   over all scales
% - in max_scales(i,j), the scale that gave the max score 

result = ones(size(image)) * -10;
max_scales = zeros(size(image));

for scale = scales;
    % for efficiency, we either downsize the image, or the template, 
    % depending on the current scale
    if scale >= 1
        scaled_image = imresize(image, 1/scale, 'bilinear');
        if (size(scaled_image, 1) < size(template, 1))
            continue;
        end
        if (size(scaled_image, 2) < size(template, 2))
            continue;
        end
        temp_result = normalized_correlation(scaled_image, template);
        temp_result = imresize(temp_result, size(image), 'nearest');
    else
        scaled_template = imresize(template, scale, 'bilinear');
        temp_result = normalized_correlation(image, scaled_template);
    end
    
    higher_maxes = (temp_result > result);
    max_scales(higher_maxes) = scale;
    result(higher_maxes) = temp_result(higher_maxes);
end



