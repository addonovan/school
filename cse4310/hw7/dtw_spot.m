function result = dtw_spot(filename, class_thresholds)
    source = read_video_frames(filename);
    [~, ~, ~, frames] = size(source);

    % I give up, just randomly generate an entry and be done with it so the 
    % testing thing can be written
    rng(42); % just make sure it'll get the same results every time
    class = randi(10);
    start_frame = randi([1, frames - 1]);
    end_frame = randi([start_frame, frames]);
    result = [start_frame, end_frame, class];

    return;
    classes = generate_dtw_gestures();
    result = compute_dtw(source, 0, 3000);

    function out = compute_dtw(source, digit, threshold)
        gesture = classes{digit + 1, 2};
        [m, ~] = size(gesture);
        [n, ~] = size(source);
        
        table = setup_dtw_table(n, m);

        % records [start, end] frames for each gesture
        out = zeros([0, 2]);

        for j = 2 : n + 1

            % if the score at the bottom of the previous column triggered
            % the threshold, then we'll just invalidate this current column
            % so that the algorithm starts again
            if table{m + 1, j - 1}{2, 2} < threshold
                table = invalidate_dtw_table_col(table, j);

                out = [out; table{m + 1, j - 1}{1, 2}, j - 1];

                fprintf('score @ frame %4d = %4.0f\n', j - 1, table{m + 1, j - 1}{2, 2});
            % otherwise, we'll just compute this column like normal
            else
                for i = 2 : m + 1
                    additional_cost = cost(gesture(i - 1, :), source(j - 1, :));
                    [best_prev_cost, ind] = min([ ...
                        table{i - 1, j    }{2, 2}, ...
                        table{i - 1, j - 1}{2, 2}, ...
                        table{i    , j - 1}{2, 2}, ...
                    ]);
                    score = additional_cost + best_prev_cost; 

                    % find which predecessor
                    pred = NaN([1, 2]);
                    if ind == 1
                        pred = table{i - 1, j};
                    elseif ind == 2
                        pred = table{i - 1, j - 1};
                    elseif ind == 3
                        pred = table{i, j - 1};
                    end

                    table{i, j} = table_entry(i, j, score, pred);
                end
            end
        end
    end

    function out = setup_dtw_table(n, m)
        out = cell([m + 1, n + 1]);
        out = invalidate_dtw_table_col(out, 1);

        for j = 1 : n + 1
            out{1, j} = table_entry(1, j, 0, []);
        end
    end

    function table = invalidate_dtw_table_col(table, col)
        [m, ~] = size(table);
        for i = 1 : m
            table{i, col} = table_entry(i, col, Inf(1), []);
        end
    end

    function out = table_entry(i, j, score, pred)
        if isempty(pred)
            out = {
                'start', j - 1,
                'score', score,
                'history', [i, j]
            };
            return;
        end

        out = {
            'start', pred{1, 2},
            'score', score,
            'history', [i, j; pred{3, 2}]
        };
    end

    function out = cost(x1, x2)
        out = sqrt(sum((x1 - x2) .^2));
    end
end

