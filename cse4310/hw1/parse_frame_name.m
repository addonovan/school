function [sequence_name, frame] = parse_frame_name(filename)
    length = size(filename, 2);
    sequence_name = filename(1:(length-8));
    frame_string = filename((length-7):(length-4));
    frame = strread(frame_string);
end
