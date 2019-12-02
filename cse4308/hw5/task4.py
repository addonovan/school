import numpy as np

def count(predicate, values):
    return len(list(filter(predicate, values)))

def percentage(predicate, values):
    return count(predicate, values) / len(values)

def c(text, val):
    return f"~{text}" if val == 0 else text

def main():
    training = read_input("training.txt")
    baseball_game_on = 0
    george_watches_tv = 1
    out_of_cat_food = 2
    george_feeds_cat = 3

    # first, calculate the marginal probabilities
    prob_game_on = percentage(lambda row: row[baseball_game_on] == 1, training)
    print(f"P(baseball_game_on_TV)  = {prob_game_on:6.4}")

    prob_no_food = percentage(lambda row: row[out_of_cat_food] == 1, training)
    print(f"P(out_of_cat_food)  = {prob_no_food:6.4}")

    # now, the conditional probability of watching TV
    prob_watches_tv = {}
    for game_on in [0, 1]:
        p = percentage(
            lambda row: row[baseball_game_on] == game_on and row[george_watches_tv] == 1,
            training
        ) / prob_game_on
        print(f"P(George_watches_TV | {c('baseball_game_on_TV', game_on)} = {p:6.4}")
        prob_watches_tv[game_on] = p

    # finally, the probabilities of george feeding the cat
    prob_feeds_cat = {}
    for watches_tv in [0, 1]:
        for no_food in [0, 1]:
            p = percentage(
                lambda row: row[george_watches_tv] == watches_tv and row[out_of_cat_food] == no_food and row[george_feeds_cat],
                training
            ) / percentage(
                lambda row: row[george_watches_tv] == watches_tv and row[out_of_cat_food] == no_food,
                training
            )

            print(f"P(George_feeds_cat | {c('George_watches_TV', watches_tv)}, {c('out_of_cat_food', no_food)}) = {p:6.4}")
            prob_feeds_cat[(watches_tv, no_food)] = p


read_input = lambda file: np.loadtxt(file)
main()
