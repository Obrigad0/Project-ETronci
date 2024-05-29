import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class UpdateStatus() :

    names = ["shipped", "delivered"]

    def receive_random_value(self):
        return random.choice(self.names)