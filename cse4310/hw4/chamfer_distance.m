function distance = chamfer_distance(image1, image2)
    distance = directed_diff(image1, image2) + directed_diff(image2, image1); 

    function out = directed_diff(v1, v2)
        v1_binary = (v1 ~= 0);
        n1 = sum(sum(v1_binary));
        dt2 = bwdist(v2);
        out = sum(sum(v1_binary .* dt2)) / n1;
    end
end

