function [x_vel, y_vel] = person_speed(first_frame, last_frame)
    [start_x, start_y] = find_person_center(first_frame);
    [end_x, end_y] = find_person_center(last_frame);

    x_vel = end_x - start_x;
    y_vel = end_y - start_y;

    function [x, y] = find_person_center(frame)
        [top, bottom, left, right] = find_person(frame);
        x = mean([left, right]);
        y = mean([top, bottom]);
        clear right;
        clear left;
        clear bottom;
        clear top;
    end
end

