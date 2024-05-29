import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class ProductName() :

    names = ["croccantini##buoni", "pomodoro", "annaffiatoio", "acqua", "mattone", "forno", "action##figure", "schiacciamosche", "vaso", "latte", "statuina", "zaino", "matita"]

    def receive_random_value(self):
        return random.choice(self.names)
