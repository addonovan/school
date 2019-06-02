function out = green_hand_trajectory(filename, start_frame, end_frame)
    frames = double(read_video_frames(filename));

    % if we're only given 1 argument, then we'll do the whole video
    if nargin == 1 
        start_frame = 2;
        [~, ~, ~, end_frame] = size(frames);
        end_frame = end_frame - 1;
    end

    frame_count = end_frame - start_frame + 1;
    out = NaN([frame_count, 2]);

    for i = 1 : frame_count 
        [~, ~, pos] = green_hands(frames, i + start_frame - 1, [40, 30]);
        out(i, 1) = pos(2);
        out(i, 2) = pos(1);
    end
end

