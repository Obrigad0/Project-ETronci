import random

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class SellerName() :

    names = ["superIndustria", "banca##italia", "miao", "monsterInc", "nonloso", "SSLazio", "amazon", "ubik", "aziendaX", "aziendaWY"]

    def receive_random_value(self):
        return random.choice(self.names)
