import random
import string

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class DescriptionGenerator() :

    def __init__(self):
        self.length = random.randint(5, 15)

    def receive_random_value(self):
        return ''.join(random.choices(string.ascii_letters + string.digits, k=self.length))
