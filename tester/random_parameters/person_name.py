import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class PersonName() :

    names = ["filiberto", "gianCarlo salvadora", "io", "serpente", "marco", "professore", "mr mauro", "cbciSVFL", "nessuno", "lady gaga", "ulisse", "gattomante"]

    def receive_random_value(self):
        return random.choice(self.names)
