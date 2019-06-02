function digits = load_mnist_by_label(data_file, class_label)
    % load all digits and labels in the dataset
    [digits, labels] = load_mnist(data_file);

    % select only the ones with the given class label
    predicate = labels == class_label;
    digits = digits(:, :, predicate);
end

