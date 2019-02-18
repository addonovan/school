function [] = soccer_analysis(soccer_image)
    %soccer_image = imfilter(soccer_image, ones(5, 5) / 25, 'conv');
    r = soccer_image(:, :, 1);
    g = soccer_image(:, :, 2);
    b = soccer_image(:, :, 3);

    field = g - r > 25;

    red_team = (r - b > 80) & (r - g > 80);
    red_team = imerode(red_team, ones(2, 2));
    red_team = ~field & red_team;
    red_team = imdilate(red_team, ones(2, 2));

    blue_team = (b - r > 50) & (b - g > 50);
    blue_team = imdilate(blue_team, ones(2, 2));
    blue_team = ~imerode(field, ones(5, 5)) & blue_team;
    blue_team = imdilate(blue_team, ones(2, 2));

    field = remove_holes(field);
    field = imerode(field, ones(7, 7));

    red_team = red_team & field;
    blue_team = blue_team & field;

    figure(1); imshow(field, []);
    figure(2); imshow(red_team, []);
    figure(3); imshow(blue_team, []);
end

