function result = pinhole_location(correspondences)
    % CORRESPONDENCES is an Nx5 matrix of the following columns:
    %   [x, y, z,   u, v]
    camera_matrix = perspective_calibration(correspondences); 
    result = estimate_3D_point(camera_matrix, camera_matrix, 0, 0, 1, 1); 
end

