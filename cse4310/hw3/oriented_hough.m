function votes = oriented_hough(grayscale, thetas, rhos, thr1, thr2)
    [~, theta_count] = size(thetas);
    [~, rho_count] = size(rhos);
    votes = zeros([rho_count, theta_count]);

    % go find all of the edges in the image
    edges = canny(double(grayscale), thr1); % canny requires a double-based image to do `.^ 0.5`

    % get a list of all of the edge orientations of each pixel in the image
    orientations = calculate_orientations(edges);

    % this closesly follows the pseudocode from slide 66
    [rows, cols] = size(grayscale);
    for j = 1 : cols
        for i = 1 : rows
            % skip any non-edge pixels
            if edges(j, i) == 0
                continue;
            end

            % find the orientation for this pixel
            orientation = orientations(i, j);

            for theta_i = 1 : theta_count
                theta = thetas(theta_i);

                % don't even bother wasting time on thetas outside of the range
                % of possible answers
                if orientation_difference(orientation, theta) > thr2
                    continue;
                end

                % find the rho and votes for this
                rho_i = nearest_rho(rhos, j, i, theta);
                votes(rho_i, theta_i) = votes(rho, theta) + 1;
            end
        end
    end

    function out = calculate_orientations(image)
        % Calculates the gradient orientation angles for each pixel in the given image
        % by first applying the required x and y filters (see instructions).

        dx = [0,  0, 0; -1, 0, 1; 0, 0, 0];
        dy = [0, -1, 0;  0, 0, 0; 0, 1, 0];
        gradient_x = imfilter(image, dx, 'same', 'symmetric');
        gradient_x = imfilter(image, dy, 'same', 'symmetric');
        out = gradient_orientations(gradient_x, gradient_y);
    end

    function out = orientation_difference(angle1, angle2)
        % Finds the smallest angle difference between lines of the given angles.

        function out = denegativize(angle_diff)
            % Reforms negative angles as their positive counterparts
            out = angle_diff;
            if out < 0
                out = 180 + out;
            end
        end

        out = min(denegativize(angle1 - angle2), denegativize(angle2 - angle1));
    end

    function out = nearest_rho(rhos, x, y, theta)
        % Calculates the rho for the given pixel in the hough transform, then finds
        % the nearest rho value's index to that value.

        real_rho = (x * cos(theta)) + (y * sin(theta));
        [~, out] = min(abs(real_rho - rhos));
    end

end

