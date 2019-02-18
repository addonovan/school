function [] = ocean_segmentation(image)
    sky = uint8(image > 190);
    sky = imerode(sky, ones(5));
    figure(1); imshow(sky, []);

    near_land = uint8(image < 100);
    near_land = imerode(near_land, ones(2));
    near_land = imdilate(near_land, ones(9));
    ocean = uint8((image & ~sky) & ~near_land);

    far_land = uint8(image > 160);
    far_land = imdilate(far_land, ones(3));
    far_land(:, 1) = 0;
    far_land = remove_holes(far_land);

    ocean = ocean & ~far_land;
    ocean = imdilate(ocean, ones(3));

    that_one_mountain = uint8(image < 120);
    that_one_mountain = imdilate(that_one_mountain, ones(4));

    ocean = ocean & ~that_one_mountain;
    ocean = uint8(ocean);

    figure(2); imshow(ocean, []);
end

