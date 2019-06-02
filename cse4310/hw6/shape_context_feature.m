function bins = shape_context_feature(edge_image, row, col, r1)
    edge_image = standardize_edge_image(edge_image);

    ring_count = 5;
    sector_count = 12;

    bins = zeros([ring_count, sector_count]);

    [max_u, max_v] = size(edge_image);

    radii = r1 * (2 .^ [0 : ring_count - 1]);

    % the overall +/- range of the entire searchable area
    span = [-radii(ring_count) : radii(ring_count)];

    for u = row + span
        for v = col + span
            % skip zero-entries
            if u <= 0 || v <= 0 ...
                || u > max_u || v > max_v 
                continue;
            end

            if edge_image(u, v) == 0
                continue;
            end

            ring = find_ring(u, v);

            % skip values outside of all radii
            if isnan(ring)
                continue;
            end

            sector = find_sector(u, v);
            bins(ring, sector) = bins(ring, sector) + 1;
        end
    end

    % as per specifications, make sure all bins have at least one
    % entry to prevent divide-by-zero errors later
    bins(bins == 0) = 1;

    function sector = find_sector(u, v)
        angle = atan2d(u - row, v - col); % [-180, 180)

        % convert negatives to their positive counterparts
        if angle < 0
            angle = angle + 360;
        end

        boundary = 360 / sector_count;
        sector = 1 + floor(angle / boundary);
    end

    function ring = find_ring(u, v)
        % finds the ring to which the given pixel coordinates belong
        % or NaN if it's outside of all radii

        % special case, ring 1 with no inner radius
        if within_radius(u, v, radii(1))
            ring = 1;
            return;
        end

        % other rings, must be within outer ring and outside inner ring
        for ring = 2 : ring_count 
            if within_radius(u, v, radii(ring)) && ...
              ~within_radius(u, v, radii(ring - 1))
                return;
            end
        end

        % not within any rings
        ring = NaN(1);
    end

    function result = within_radius(u, v, r)
        distance = sqrt((row - u) ^ 2 + (col - v) ^ 2);
        result = distance <= r;
    end

    function result = standardize_edge_image(edge_image)
        % standardizes the EDGE_IMAGE so that it's all `double` values
        % where nonzeros values are all set to 1.
        result = double(edge_image);
        result(result ~= 0) = 1;
    end
end

