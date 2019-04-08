function camera_matrix = perspective_calibration(correspondences)
    % CORRESPONDENCES is an Nx5 matrix of the following columns:
    %   [x, y, z,   u, v]
    [N, ~] = size(correspondences);

    % generate all the entries in the A matrix (slide 64)
    %   as well as the b column vector
    A = zeros([N * 2, 11]);
    b = zeros([N * 2, 1]);

    for n = 1 : N
        row = correspondences(n, :);
        x = row(1);
        y = row(2);
        z = row(3);
        u = row(4);
        v = row(5);

        A(2 * n - 1, :) = [ x, y, z, 1, 0, 0, 0, 0, -x * u, -y * u, -z * u ];
        A(2 * n, :)     = [ 0, 0, 0, 0, x, y, z, 1, -x * v, -y * v, -z * v ];

        b(2 * n - 1) = u;
        b(2 * n)     = v;
    end

    % find x, the vectorized form of the camera system matrix 
    x = A \ b;

    % convert it into the camera matrix form
    camera_matrix = [x(1), x(2), x(3), x(4);
                     x(5), x(6), x(7), x(8);
                     x(9), x(10), x(11), 1];
end

