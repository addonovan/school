Austin Donovan
add1620
1001311620

# Task 2 Solution

## Solution
To detect if a person is present, this algorithm simply finds the bounding box 
returned by `find_person` and then calculates the total number of pixels that
bounding box encompasses (via `abs(left - right) * abs(top - bottom)`), if the
area is larger than 7000, then it's a person.

## Accuracy
This method detects frames 24-107 (inclusive) as having people, while all other
frames do not. This is pretty good, as all of these frames at least have half
of a person visible in them.

While it works, this is a pretty horrible metric and could easily be fooled by
anything large coming onto the frame, or just a very small person.

# Task 3 Solution

## Solution
To detect a person's walking speed between two frames, this algorithm will
call the `find_person` function with each of the frames. It will then calculate
the center of each bounding box (by averaging top & bottom, and left & right)
and subtract the center of the first frame from the second frame.

## Accuracy
It seems to accurately measure the distance a person has traveled as long as
there are actually people in both frames.

