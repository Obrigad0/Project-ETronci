import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class StarsGenerator() :

    def receive_random_value(self):
        return random.randint(1, 5)