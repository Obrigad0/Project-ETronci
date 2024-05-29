import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class NumberGenerator() :

    def receive_random_value(self):
        return random.randint(1, 50)