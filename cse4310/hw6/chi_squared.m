function result = chi_squared(sc1, sc2)
    result = sum(sum((sc1 - sc2) .^ 2 ./ (sc1 + sc2))) / 2;
end

