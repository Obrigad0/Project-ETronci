import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class TakeStatus() :

    names = ["shipped"] # ## tra "not" e "shipped" in "not shipped" ?????

    def receive_random_value(self):
        return random.choice(self.names)
