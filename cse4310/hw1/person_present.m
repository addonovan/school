function present = person_present(image)
    % find the bounding box of the "person", and if the given bounding box
    % encompasses an area greater than our arbitrary value, then it's a person
    % this cannot possibly go wrong.
    [top, bottom, left, right] = find_person(image);
    area = abs(bottom - top) * abs(right - left);
    present = area > 7000;
end

