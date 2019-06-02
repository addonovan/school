function digits = generate_mei_gestures()
    filename = 'mei_digits.mat';
    if exist(filename)
        load(filename, 'digits');
        return
    end

    zero = gesture(22, 91); 
    [width, height, ~] = size(zero);

    digits = zeros([width, height, 10]);
    digits(:, :,  1) = zero;
    digits(:, :,  2) = gesture(143, 165);
    digits(:, :,  3) = gesture(220, 300);
    digits(:, :,  4) = gesture(350, 445);
    digits(:, :,  5) = gesture(495, 544);
    digits(:, :,  6) = gesture(608, 701);
    digits(:, :,  7) = gesture(763, 850);
    digits(:, :,  8) = gesture(906, 961);
    digits(:, :,  9) = gesture(1035, 1150);
    digits(:, :, 10) = gesture(1206, 1277);

    save(filename, 'digits');

    function out = gesture(a, b)
        out = mei_image('digits_model_quan_ex3.avi', a, b);
        out = out - mean(out(:));
        out = out / std(out(:));
    end

end

