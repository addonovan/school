function [tp, fp, fn] = dtw_spot_accuracy(ground_truth, gestures, iou_threshold)
    ground_truth = dlmread(ground_truth);

    % see hw5/check_boxes.m for comments as to what's happening here

    [N, ~] = size(gestures);
    [M, ~] = size(ground_truth);
    iou_scores = zeros([N, M]);

    % little hack here, we can use bboxOverlapRatio to compare time ranges
    % if the "boxes" are 1 high and are at the same y value.
    for n = 1 : N
        class = gestures(3);
        gest_box = [gestures(1), 1, gestures(2), 1];
        
        for m = 1 : M
            % if the classes don't match, then don't check the bounding boxes
            if ground_truth(1) ~= gestures(3)
                iou_scores(n, m) = 0;
            else
                true_box = [ground_truth(2), 1, ground_truth(3), 1];
                iou_scores(n, m) = bboxOverlapRatio(gest_box, true_box);
            end
        end
    end

    tp = nnz(iou_scores);

    fp = 0;
    for n = 1 : N
        if nnz(iou_scores(n, :)) == 0
            fp = fp + 1;
        end
    end

    fn = 0;
    for m = 1 : M
        if nnz(iou_scores(:, m)) == 0
            fn = fn + 1;
        end
    end
end

