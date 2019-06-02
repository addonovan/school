function class_label = mei_classifier(filename, start_frame, end_frame)
    source = mei_image(filename, start_frame, end_frame);
    source = source - mean(source(:));
    source = source / std(source(:));

    classes = generate_mei_gestures();

    diff = classes - source;
    dist = sum(sum(diff .^ 2));
    [~, ind] = min(dist);

    class_label = ind - 1; % index [1, 10], classes [0, 9], so shift left 1
end

