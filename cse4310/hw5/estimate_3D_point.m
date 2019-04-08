function result = estimate_3D_point(c1, c2, u1, v1, u2, v2)
    % c1, c2 is a 3x4 camera matrix for the calibration of two cameras
    % u1, v1 are the pixel coordinates from the camera using c1
    % u2, v2 are the pixel coordinates from the camera using c2
    % 
    % result is a 3x1 column vector

    [a, b] = find_points_on_line(c1, u1, v1);
    point1 = a;
    vec1 = b - a;

    [a, b] = find_points_on_line(c2, u2, v2);
    point2 = a;
    vec2 = b - a;

    result = find_intersection(point1, vec1, point2, vec2);

    function out = find_intersection(p1, u1, p2, u2)
        A = [u1' * u1,      -u2' * u1; ...
             u1' * u2,      -u2' * u2];
        B = [(p2' * u1) - (p1' * u1); ...
             (p2' * u2) - (p1' * u2)];
       
        % find the intersection points 
        W = A \ B;
        a1 = W(1);
        a2 = W(2);

        % calculate the midpoint of the segment
        out = ((p1 + (a1 * u1)) + (p2 + (a2 * u2))) / 2;
    end

    function [a, b] = find_points_on_line(C, u, v)
        a = find_point_on_line(C, u, v, 0);
        b = find_point_on_line(C, u, v, 1);
    end    

    function point = find_point_on_line(C, u, v, x)
        A = [C(1, 2) - (C(3, 2) * u),     C(1, 3) - (C(3, 3) * u); ...
             C(2, 2) - (C(3, 2) * v),     C(2, 3) - (C(3, 3) * v)];
        B = [(C(3, 1) * x * u) + u - (C(1, 1) * x) - C(1, 4); ...
             (C(3, 1) * x * v) + v - (C(2, 1) * x) - C(2, 4)];

        W = A \ B;
        point = [x; W(1); W(2)];
    end 
end

