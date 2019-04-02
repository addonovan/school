function dist = euclidean_distance(image1, image2)
    dist = sqrt(sum((image1(:) - image2(:)) .^ 2));
end

