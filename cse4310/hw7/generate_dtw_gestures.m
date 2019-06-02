function digits = generate_dtw_gestures()
    filename = 'dtw_digits.mat';
    if exist(filename)
        load(filename, 'digits');
        return
    end

    digits = {
        0, gesture(22, 91);
        1, gesture(143, 165);
        2, gesture(220, 300);
        3, gesture(350, 445);
        4, gesture(495, 544);
        5, gesture(608, 701);
        6, gesture(763, 850);
        7, gesture(906, 961);
        8, gesture(1035, 1150);
        9, gesture(1206, 1277);
    };

    save(filename, 'digits');

    function out = gesture(a, b)
        out = green_hand_trajectory('digits_model_quan_ex3.avi', a, b);
    end
end

