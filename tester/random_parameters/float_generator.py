import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class FloatGenerator() :

    def receive_random_value(self):
        return random.uniform(0.49, 9.99)