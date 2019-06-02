function result = mei_image(filename, start_frame, end_frame)
    frames = double(read_video_frames(filename));
    [width, height, channels, ~] = size(frames);

    result = zeros([width, height]);

    for i = start_frame + 1 : end_frame - 1
        prev_frame = grayscale(frames(:, :, :, i - 1));
        this_frame = grayscale(frames(:, :, :, i));
        next_frame = grayscale(frames(:, :, :, i + 1));

        diff1 = abs(this_frame - prev_frame);
        diff2 = abs(next_frame - this_frame);
    
        result = result + min(diff1, diff2);
    end

    function out = grayscale(image)
        out = zeros([width, height]);
        out(:, :) = image(:, :, 1) + image(:, :, 2) + image(:, :, 3);
        out = out ./ 3;
    end
end

