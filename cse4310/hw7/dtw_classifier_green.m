function class_label = dtw_classifier_green(filename, start_frame, end_frame)
    classes = generate_dtw_gestures();
    source = green_hand_trajectory(filename, start_frame, end_frame);
    [n, ~] = size(source);

    best_score = Inf(1);
    class_label = NaN(1);
    for digit = 0 : 9
        score = compute_dtw(source, digit);
        if score < best_score
            best_score = score;
            class_label = digit;
        end
    end

    function out = compute_dtw(source, digit)
        class = classes{digit + 1, 2};
        [m, ~] = size(class);

        scores = NaN([m, n]);
        scores(1, 1) = cost(class(1, :), source(1, :));

        for i = 2 : m
            scores(i, 1) = scores(i - 1, 1) + cost(class(i, :), source(1, :));
        end
        for j = 2 : n
            scores(1, j) = scores(1, j - 1) + cost(class(1, :), source(j, :));
        end

        for i = 2 : m
            for j = 2 : n
                scores(i, j) = cost(class(i, :), source(j, :)) + min([ ...
                    scores(i - 1, j    ), ...
                    scores(i - 1, j - 1), ...
                    scores(i,     j - 1), ...
                ]);
            end
        end

        out = scores(m, n);
    end

    function out = cost(x1, x2)
        out = sqrt(sum((x1 - x2) .^2));
    end
end

