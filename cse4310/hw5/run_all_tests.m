function [] = run_all_tests(iou_thresh)
    template_normal = imread('average_face.png');
    template_cropped = imread('average_face_cropped.png');
    scales = make_scales_array(1, 5, 1.1);

    fprintf('iou_thresh = %4.2f\n', iou_thresh);
    fprintf('Thresh Template Type      tp    fp    fn\n');
    for detection_thr = 0 : 0.1 : 1
        [norm_tp, norm_fp, norm_fn] = detection_accuracy(...
            'ground_truth.txt', template_normal, scales, detection_thr, iou_thresh);
        
        [crop_tp, crop_fp, crop_fn] = detection_accuracy(...
            'ground_truth.txt', template_cropped, scales, detection_thr, iou_thresh);

        fprintf('%4.2f    Normal          %5d %5d %5d\n', detection_thr, norm_tp, norm_fp, norm_fn);
        fprintf('%4.2f    Cropped         %5d %5d %5d\n', detection_thr, crop_tp, crop_fp, crop_fn);
        fprintf('\n');
    end
end

