function result = detection_maxima(template, scores, scales, thr);

% function result = detection_maxima(template,  scores, scales, thr);
%
% This function processes detection scores and detection scales that
% were obtained by calling the multiscale_correlation function.
% This function decides which of those scores correspond to detection
% results, and returns a matrix describing those bounding boxes.
%
% Detection results are chosen using these rules:
% - Any score above thr (where thr is the fifth input argument) is 
%   considered good enough to be selected as a detection, unless it is
%   suppressed by a higher score close to it.
% - If we select a detection result, we suppress all other potential
%   detection results whose center is within the bounding box of this
%   detection result.

[image_rows image_cols] = size(scores);
[template_rows, template_cols] = size(template);
result = [];

while 1
    % Get next best score, see if it is above the detection threshold.
    % If not, we are done.
    current_max = max(scores(:));
    if (current_max < thr)
        break;
    end
    
    % Get the location and scale of the current best score.
    [rows cols] = find(scores == current_max);
    center_row = rows(1);
    center_col = cols(1);    
    scale = scales(center_row, center_col);
    
    % Create the bounding box correspoding to the location and scale of the
    % current best score.
    half_box_rows = round(scale*template_rows/2);
    half_box_cols = round(scale*template_cols/2);
    top = max(center_row - half_box_rows + 1, 1);
    bottom = min(center_row + half_box_rows - 1, image_rows);
    left = max(center_col - half_box_cols + 1, 1);
    right = min(center_col + half_box_cols - 1, image_cols);

    % supress all scores within this bounding box.
    scores(top:bottom, left:right) = -10;
    
    % add to the result a row corresponding to this bounding box 
    result = [result; [top, bottom, left, right, current_max, scale]];
end

