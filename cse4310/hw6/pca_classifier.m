function class_label = pca_classifier(image, data_file, d)
    persistent prev_data_file;
    persistent prev_d;
    persistent prev_eigenvectors;
    persistent prev_averages;

    eigenvectors = calculate_metadata(data_file, d);

    image = vectorize_image(image); 
    image = normalize_image([image]);

    min_error = Inf(1);
    class_label = NaN(1);

    for digit = 0 : 9
        evs = eigenvectors(:, :, digit + 1);
        average = prev_averages(digit + 1, :);

        backproj_error = find_backproj_error(image, average, evs);

        if backproj_error < min_error
            min_error = backproj_error;
            class_label = digit;
        end
    end

    function backproj_error = find_backproj_error(image, average, eigenvectors)
        % project, then unproject our image and then find the changes
        projection = project(image, average, eigenvectors);
        backprojection = backproject(projection, average, eigenvectors);

        % measure the error using sum of squared differences
        backproj_error = sum((image - backprojection) .^ 2);
    end

    function result = backproject(projection, average, eigenvectors)
        centered = eigenvectors * projection;
        result = centered + average;
    end

    function result = project(image, average, eigenvectors)
        centered = image - average; 
        result = eigenvectors' * centered;
    end

    function average = average_values(data_file, class_label)
        digits = load_mnist_by_label(data_file, class_label);
        digits = vectorize_image(digits);
        average = [mean(digits')]';
    end

    function [evs] = calculate_metadata(data_file, d)
        % always keep track of the previous metadata, if this changes
        % then we'll have to update our eigenvectors
        if isempty(prev_data_file)
            prev_data_file = data_file;
            prev_d = d;
        end

        file_changed = ~strcmp(prev_data_file, data_file);

        % if we need to update the eigenvectors list, then do that
        if isempty(prev_eigenvectors) || file_changed
            computed = [];
            for digit = 0 : 9
                [evs, ~] = mnist_pca(data_file, digit, NaN(1));

                if isempty(computed)
                    [n, m] = size(evs);
                    computed = zeros([n, m, 10]);
                end

                computed(:, :, digit + 1) = evs(:, :);
            end

            prev_eigenvectors = computed;
        end

        % select just the top `d` vectors (we're given all back)
        evs = prev_eigenvectors(:, 1 : d, :);

        if isempty(prev_averages) || file_changed 
            prev_averages = [];
            for digit = 0 : 9
                average = average_values(data_file, digit);
                prev_averages = [prev_averages; average];
            end
        end
        
        prev_data_file = data_file;
        prev_d = d;
    end
end

