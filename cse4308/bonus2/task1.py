from os import sys

def seq_probability(sequence):
    sum = 0
    for hypothesis in hypotheses:
        base = hypothesis[0]
        for item in sequence:
            base *= hypothesis[item]
        sum += base

    return sum

def seq_prob_given_hypothesis(sequence, hypothesis):
    base = 1
    for item in sequence:
        base *= hypothesis[item]
    return base

def posterior(hypothesis, sequence):
    return seq_prob_given_hypothesis(sequence, hypothesis) * hypothesis[0] / seq_probability(sequence)

def to_indices(sequence):
    lookup = {
        "C": CHERRY,
        "L": LIME 
    }

    return list(map(
        lambda c: lookup[c],
        sequence
    ))

CHERRY = 1
LIME = 2

hypotheses = [
    (0.10, 1.00, 0.00, 'h1'),
    (0.20, 0.75, 0.25, 'h2'),
    (0.40, 0.50, 0.50, 'h3'),
    (0.20, 0.25, 0.75, 'h4'),
    (0.10, 0.00, 1.00, 'h5')
]

seq = sys.argv[1]
print(f"Observation sequence Q: {seq}")
print(f"Length of Q: {len(seq)}\n")

for i in range(0, len(seq)):
    observations = to_indices(seq[0 : i + 1]) # upper bound is exclusive

    print(f"After Observation {i + 1} = {seq[i]}:")
    print()

    cherry_prob = 0
    lime_prob = 0

    for hypothesis in hypotheses:
        name = hypothesis[3]
        prob = posterior(hypothesis, observations)
        print(f"P({name} | Q) = {prob:7.5f}")

        cherry_prob += prob * hypothesis[CHERRY]
        lime_prob += prob * hypothesis[LIME]

    print()
    print(f"Probability that the next candy we pick will be C, given Q: {cherry_prob:7.5f}")
    print(f"Probability that the next candy we pick will be L, given Q: {lime_prob:7.5f}")
    print()
