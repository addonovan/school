function [scores, max_scales] = ssd_bn_multiscale(image, template, scales)
	result = -ones(size(image));
	max_scales = zeros(size(image));

	for scale = scales
		% for efficiency, we either downsize the image, or the template, 
		% depending on the current scale
		if scale >= 1
			scaled_image = imresize(image, 1/scale, 'bilinear');
			temp_result = ssd_bn_search(scaled_image, template);
			temp_result = imresize(temp_result, size(image), 'bilinear');
		else
            scaled_template = resize_template(template, scale);
			temp_result = ssd_bn_search(image, scaled_template);
		end
		
		higher_maxes = (temp_result > result);
		max_scales(higher_maxes) = scale;
		result(higher_maxes) = temp_result(higher_maxes);
	end

    function result = resize_template(template, scale)
		% function result = resize_template(template, scale)
		%
		% Resizes the template by the specific scale, calling imresize.
		% It ensures that the number of rows and columns of the result are 
		% both odd numbers.

		[rows, cols] = size(template);
		new_rows = floor(rows * scale);
		if (mod(floor(new_rows), 2) == 0)
			new_rows = new_rows + 1;
		end

		new_cols = floor(cols * scale);
		if (mod(floor(new_cols), 2) == 0)
			new_cols = new_cols + 1;
		end

		result = imresize(template, [new_rows, new_cols], 'bilinear');
    end
end

