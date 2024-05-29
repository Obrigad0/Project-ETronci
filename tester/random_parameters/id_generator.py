import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class IdGenerator() :

    def receive_random_value(self):
        return random.randint(1, 10)