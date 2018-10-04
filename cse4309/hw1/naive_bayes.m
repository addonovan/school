% Austin Donovan
% add1620
% 21 September, 2018
%
% Naive Bayesian Classifier

function [] = naive_bayes(training_file, test_file)
    training = dlmread(training_file);
    testing = dlmread(test_file);

    [class_names, class_count, attribute_count] = find_size(training);
    overviews = calculate_class_overviews(class_names, class_count, attribute_count, training);
    return

    % print out data overview from training
    for class = 1:class_count
        for attr = 1:attribute_count
            disp(sprintf( ...
                'Class %d, attribute %d, mean = %.2f, std = %.2f', ...
                class_names(class), ...
                attr, ...
                overviews(class, attr, 2), ...
                overviews(class, attr, 1) ...
            ));
        end
    end

    % print out prediction overview
    [testing_rows, ~] = size(testing);
    total_accuracy = 0;
    for row = 1:testing_rows
        [pclass, p, rclass, accuracy] = predict(class_names, overviews, testing(row, :));
        total_accuracy = total_accuracy + accuracy;

        disp(sprintf( ...
            'ID=%5d, predicted=%3d, probability = %.4f, true=%3d, accuracy=%4.2f', ...
            row, pclass, p, rclass, accuracy ...
        ));
    end

    disp(sprintf( ...
        'classification accuracy=%6.4f', ...
        total_accuracy / testing_rows ...
    ));

    function [pclass, p, rclass, accuracy] = predict(class_names, overviews, test_row)
        % Tries to predict what to which class the attributes described in the
        % test_row belong. This function references the class_count and attribute_count
        % variables from the outer scope and will not affect them in any way.
        %   pclass = the predicted CLASS ID
        %   p = the probability that pclass is correct
        %   rclass = the real CLASS ID of the test row
        %   accuracy = the value we scored

        rclass = test_row(end); % this is a CLASS ID
        attributes = test_row(1, 1:end-1);

        probs = zeros([1, class_count]);
        for class = 1:class_count
            probs(class) = probability(overviews, class, attributes, attribute_count);
        end

        % have the scale the probabilities down by all of the evidence
        % we don't have to do anything with the probabilities of any of the classes here
        % because we've assumed that all classes are equiprobable, and thus the p(Ck) on
        % the numerator cancels out with all of the p(Ci)s in the denominator 
        probs = probs / sum(probs);

        % find the highest probability, default to the first one
        p = probs(1);
        pclass = [class_names(1)];
        for class = 1:class_count
            if probs(class) > p
                p = probs(class);
                pclass = [class_names(class)];

            % if the difference in probabilimatches is less than'd be displayed in the
            % output, then we'll count that as a tie (because floating point 
            % comparison ain't gonna work right).
            %elseif abs(probs(class) - p) < 0.0001
            %    pclass = [pclass, class_names(class)];
            %end

            elseif probs(class) == p
                pclass = [pclass, class_names(class)];
            end
        end

        accuracy = 0;

        % if ANY of the matches are the real class, then we're good!
        % (remembering to make pclass an array and do this check bumped up all of
        % my accuracies a whole bunch, I wonder why 🤔)
        if any(pclass == rclass) 
            pclass = rclass;
            [~, ties] = size(pclass);
            accuracy = 1 / ties;
            clear ties;

        % if it's wrong, then just choose one of the ties randomly
        else
            pclass = pclass(1);
        end

        clear probs;
    end

    function p = probability(overviews, class, attributes, attribute_count)
        % Calculates the probability of the given CLASS INDEX based on the test
        % data's attributes.
        %   p = probability of the attributes given the overview data.
        %
        % This relies on the assumption that the attributes are conditionally
        % indepdendent of each other, given the training data. Therefore, we
        % can just multiply all of the probabilities together to get the probability
        % making this naive.
   
        % instead of multiplying everything, we'll LOG the probabilities and then
        % add those up, for both ease and stability. 
        p = 0;

        for i = 1:attribute_count
            x = attributes(i);
            sigma = overviews(class, i, 1);
            mu = overviews(class, i, 2);

            % I kept getting random infs and stuff when I did this by hand, so
            % I'll just trust the built-in function a bit more
            tmp = normpdf(x, mu, sigma);

            % I'd rather this error here than fail silently. Hopefully it won't
            % happen during grading.
            if isnan(tmp) || isinf(tmp) || tmp < 0
                error(sprintf( ...
                    '[c=%d, attr=%d] normpdf(%f, %f, %f) = %f', ...
                    class, i, x, mu, sigma, tmp ...
                ));
            end

            p = p + log(tmp);

            clear tmp;
            clear mu;
            clear sigma;
            clear x;
        end

        p = exp(p);

    end

    function overviews = calculate_class_overviews(class_names, class_count, attribute_count, training)
        % Calculates the statisical overview for all of the attributes across
        % all of the classes.
        %   overviews(CLASS INDEX, attribute, 1) = standard deviation
        %   overviews(CLASS INDEX, attribute, 2) = mean

        overviews = zeros([class_count, attribute_count, 2]);
       
        for i = 1:class_count
            overviews(i, :, :) = calculate_overview( ...
                class_names(i), ...
                attribute_count, ...
                training ...
            );
        end

        clear i;
    end

    function overview = calculate_overview(class_id, attribute_count, training)
        % splits the data into their CLASS IDS and then calculates the statistical
        % overview for each section, such that:
        %   overview(attribute, 1) = standard deviation
        %   overview(attribute, 2) = mean 

        overview = zeros([attribute_count, 2]);
        data = training(training(:, end) == class_id, :);
        
        [rows, ~] = size(data);
        disp(sprintf('Class %d (rows = %d)', class_id, rows));
        %data

        for i = 1:attribute_count
            overview(i, :) = summarize(data(:, i));
        end

        clear data;
    end

    function summary = summarize(column)
        % generates a row vector summarizing the stats of the given column:
        %   summary(1) = standard deviation of column 
        %   summary(2) = mean of column 

        summary = [max(std(column), 0.01), mean(column)];
    end

    function [class_names, class_size, attributes] = find_size(training)
        % Determines the dimensions of the training data
        %   class_names = the names/labels of the classes (maps CLASS INDEX --> CLASS ID)
        %   class_size = the number of classes
        %   attributes = the number of attributes of the data
        %
        % Classes have to be split into CLASS INDICES and CLASS IDS specifically
        % because of the satellite training data, which has 6 classes, but they're
        % confusingly labelled [1, 2, 3, 4, 5, 7]. So, most of the work is done on
        % CLASS INDICES and the output is shown by indexing into the class_names.

        [~, attributes] = size(training);
        attributes = attributes - 1;
        class_names = unique(training(:, attributes + 1))';
        [~, class_size] = size(class_names); 
    end
end
    

