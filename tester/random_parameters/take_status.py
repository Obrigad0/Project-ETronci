import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class TakeStatus() :

    names = ["not shipped", "shipped"]

    def receive_random_value(self):
        return random.choice(self.names)