function [result, boxes] =  template_detector(image, template, scales, thr)

% function [result, boxes] =  template_detector(image, template, scales, thr)
%
% Detects image windows that give high normalized cross-correlation scores
% with the template, and returns the bounding boxes of those windows.
%
% Arguments:
% - image: the input image
% - template: the template describing the object that we want to detect
% - scales: a 1D array of scale values that we need to search over. 
%   Detecting at scale s means that we search over windows whose size is
%   s times the size of the template.
% - thr: this is the detection threshold. Every normalized
%   cross-correlation score above thr will be included in the result,
%   unless it gets suppressed when we do non-maxima suppression.
%
% Return values:
% - result: an image that is a copy of the input image, with 
%   the bounding boxes drawn for each of detection results. 
% - boxes: a 2D array of detection results. Each row of boxes corresponds
%   to a bounding box, and has the format: [top, bottom, left, right, score, scale].
%   Among those six values, score is the normalized cross-correlation score
%   between that window and the template, and scale is the scale that 
%   corresponds to the size of the window.

[scores, scales] = multiscale_correlation(image, template, scales);
boxes = detection_maxima(template, scores, scales, thr);
result = image;
result_number = size(boxes, 1);

for number = 1:result_number
    result = draw_rectangle1(result, boxes(number, 1), boxes(number, 2), ...
                             boxes(number, 3), boxes(number, 4));
end

result = uint8(result);

