function [accuracy, confusion] = pca_classifier_stats(data_file, d)

    confusion = zeros(10);
    correct = 0;

    for digit = 0 : 9
        for seq = 1 : 10
            image = vectorize_image(read_test_image(digit, seq));
            class = pca_classifier(image, data_file, d);

            if digit == class
                correct = correct + 1;
            end
    
            [r, c] = index_for(class, digit); 
            confusion(r, c) = confusion(r, c) + 1;
        end
    end

    % divide occurrences by sample size (100 by coincidence)
    accuracy = correct / 100;

    % divide each row by the number of times that class was tested (which
    % happen to all be the same frequencey)
    confusion = confusion / 10;

    function [row, column] = index_for(actual_class, expected_class)
        row = expected_class;
        if row == 0
            row = 10;
        end

        column = actual_class;
        if column == 0
            column = 10;
        end
    end

    function image = read_test_image(class, sequence)
        name = sprintf('label%d_test%d.png', class, sequence);
        image = imread(name);
    end
end

