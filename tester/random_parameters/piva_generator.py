import random
import string

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class PivaGenerator() :

    def receive_random_value(self):
        return ''.join(random.choices(string.ascii_letters + string.digits, k=11))