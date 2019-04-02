function class_label = nnc_euclidean(test_image)
    min_distance = Inf(1);
    class_label = NaN(1);
    for digit = 0 : 9
        for sequence = 1 : 15
            training_image = read_image(digit, sequence); 
            distance = chamfer_distance(test_image, training_image);

            if distance < min_distance
                min_distance = distance;
                class_label = digit;
            end
        end
    end

    function out = read_image(digit, sequence)
        file_name = sprintf('digits_training/label%d_training%d.png', digit, sequence);
        out = imread(file_name);
    end
end

