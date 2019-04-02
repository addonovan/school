function [accuracy, confusion] = nnc_euclidean_stats()
    confusion = zeros([10, 10]);
    correct = 0;

    for digit = 0 : 9
        for sequence = 1 : 10
            actual = nnc_euclidean(read_image(digit, sequence));

            if actual == digit
                correct = correct + 1;
            end

            [r, c] = index_for(actual, digit);
            confusion(r, c) = confusion(r, c) + 1;
        end
    end

    % divide the occurences by the sample size (which just happens to be 100) to get the frequency 
    accuracy = correct / 100;

    % divide each row by the number of times that class was tested (which are all equal), 10
    confusion = confusion / 10;

    function [row, col] = index_for(actual, expected)
        row = expected;
        if row == 0
            row = 10;
        end 

        col = actual;
        if col == 0
            col = 10;
        end
    end
    
    function out = read_image(digit, sequence)
        file_name = sprintf('digits_test/label%d_test%d.png', digit, sequence);
        out = imread(file_name);
    end
end

