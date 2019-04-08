function [tp, fp, fn] = detection_accuracy(ground_truth_file, template, scales, ...
                                           detection_thr, iou_thr)

[filenames, numbers, gt_boxes] = read_ground_truth(ground_truth_file);
number = numel(filenames);

tp = 0;
fp = 0;
fn = 0;

for n = 1:number
    disp(sprintf('processing image %d out of %d...', n, number));
    filename = filenames{n};
    pathname = sprintf('%s', filename);
    test_image = read_gray(pathname);
    [result, boxes] = template_detector(test_image, template, scales, detection_thr);

    % shows the detected bounding boxes.
    figure(1); imshow(result);
    
    current_gt = gt_boxes{n};
    [current_tp, current_fp, current_fn] = check_boxes(boxes, current_gt, iou_thr);
    
    tp = tp + current_tp;
    fp = fp + current_fp;
    fn = fn + current_fn;
end

disp('done');


