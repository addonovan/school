function [tp, fp, fn] = check_boxes(boxes, ground_truth, iou_thr)
    [N, ~] = size(boxes);
    [M, ~] = size(ground_truth);
    iou_scores = zeros([N, M]);

    %
    % Calculate all of the IoU values
    %

    for n = 1 : N
        box = boxes(n, 1 : 4);

        for m = 1 : M
            real = ground_truth(m, :);
            iou_scores(n, m) = bboxOverlapRatio(box, real);
        end
    end
    iou_scores(iou_scores <= iou_thr) = 0;

    %
    % Calculate True & False Positives
    %

    % true positives are those which were successfully matched
    tp = nnz(iou_scores);

    % false positives are boxes which weren't successfully matched
    fp = 0;
    for n = 1 : N
        % if it didn't match any of the ground truths sufficiently, it was
        % a false positive
        if nnz(iou_scores(n, :)) == 0
            fp = fp + 1;
        end
    end

    %
    % Calculate False Negatives
    %

    % false negatives are ground truths which weren't successfully matched
    fn = 0;
    for m = 1 : M
        % if there are no non-zero scores for this ground truth face, then
        % they've all been filtered out and the face wasn't detected
        if nnz(iou_scores(:, m)) == 0
            fn = fn + 1;
        end
    end
end

