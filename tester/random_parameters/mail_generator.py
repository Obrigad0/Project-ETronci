import random
import string

RANDOM_SEED = 5
random.seed(RANDOM_SEED)

class MailGenerator() :

    mails = ["@gmail.com", "@yahoo.com", "@live.it"]

    def __init__(self):
        self.length = random.randint(4, 12)

    def receive_random_value(self):
        random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=self.length))
        random_mail_domain = random.choice(self.mails)
        return random_string + random_mail_domain