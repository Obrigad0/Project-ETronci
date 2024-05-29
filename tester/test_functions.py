from random_parameters.person_name import PersonName
from random_parameters.seller_name import SellerName
from random_parameters.product_name import ProductName

from random_parameters.take_status import TakeStatus
from random_parameters.update_status import UpdateStatus

from random_parameters.description_generator import DescriptionGenerator
from random_parameters.piva_generator import PivaGenerator
from random_parameters.mail_generator import MailGenerator

from random_parameters.id_generator import IdGenerator
from random_parameters.number_generator import NumberGenerator
from random_parameters.stars_generator import StarsGenerator
from random_parameters.float_generator import FloatGenerator


requests = {
    "register-seller" : [[("name", SellerName)],
                        [("piva", PivaGenerator)],
                        [("description", DescriptionGenerator)],
                        [("mail", MailGenerator)]],

    "add-product" : [[("name", ProductName)],
                    [("description", DescriptionGenerator)],
                    [("price_tag", FloatGenerator)],
                    [("seller", IdGenerator)],
                    [("warehouse", IdGenerator)]],

    "update-product" : [[("id", IdGenerator)],
                        [("description", DescriptionGenerator)]],

    ####

    "register-courier" : [[("name", PersonName)],
                        [("mail", MailGenerator)],
                        [("warehouse", IdGenerator)]],
    
    "take-delivery" : [[("orderId", IdGenerator)],
                        [("courier", IdGenerator)],
                        [("status", TakeStatus)]],
    
    "update-delivery-status" : [[("id", IdGenerator)],
                                [("status", UpdateStatus)]],

    "view-pending-orders" : [[("courier_warehouse", IdGenerator)]],

    ####

    "register-customer" : [[("name", PersonName)],
                        [("mail", MailGenerator)]],
    
    "search-products" : [[("product_name", DescriptionGenerator)]],

    "make-order" : [[("customer", IdGenerator)],
                    [("product", IdGenerator)],
                    [("quantity", NumberGenerator)],
                    [("zip_code", DescriptionGenerator)],
                    [("address", DescriptionGenerator)]],

    "check-order" : [[("order_id", IdGenerator)]],

    "review-order" : [[("customer", IdGenerator)],
                    [("product", IdGenerator)],
                    [("stars", StarsGenerator)],
                    [("comment", DescriptionGenerator)]]
    
}

