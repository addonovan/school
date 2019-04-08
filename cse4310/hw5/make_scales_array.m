function result = make_scales_array(low_scale, high_scale, step_factor)

% function result = make_scales_array(low_scale, high_scale, step_factor)
%
% create an array such that:
% - the first element is low_scale
% - each subsequent element is the previous element multipled by step factor
% - the last element is >= high_scale

ratio = high_scale / low_scale;
number_of_steps = ceil(log(ratio)/log(step_factor));
result = low_scale * (step_factor .^ (0:number_of_steps));


