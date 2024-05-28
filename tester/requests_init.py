from param_value.alpha_numeric import AlphaNumeric
from param_value.email import Email
from param_value.phone_number import PhoneNumber
from param_value.card import Card
from param_value.string_s import StringS
from param_value.string_m import StringM
from param_value.string_l import StringL
from param_value.real_gez import RealGEZ
from param_value.int_gz import IntGZ
from param_value.identifier import Identifier
from param_value.timestamp import Timestamp
from param_value.stars import Stars


requests = {
    "register-seller" : [[("name", StringS)],
                        [("piva", StringS)],
                        [("description", StringS)],
                        [("mail", StringS)]],

    "add-product" : [[("name", AlphaNumeric)],
                    [("description", StringS)],
                    [("price_tag", StringL)],
                    [("seller", RealGEZ)],
                    [("warehouse", RealGEZ)]],

    "update-product" : [[("id", IntGZ)],
                        [("description", StringS)]],

    ####

    "register-courier" : [[("name", StringS)],
                        [("mail", Email)],
                        [("warehouse", Email)]],
    
    "take-delivery" : [[("orderId", StringS)],
                        [("courier", Identifier)],
                        [("date", AlphaNumeric)],
                        [("status", AlphaNumeric)]],
    
    "update-delivery-status" : [[("id", AlphaNumeric)],
                                [("status", Timestamp)]],

    "view-pending-orders" : [[("courier_warehouse", Identifier)]],

    ####

    "register-customer" : [[("name", StringS)],
                        [("mail", Email)]],
    
    "search-products" : [[("product_name", StringS)]],

    "make-order" : [[("customer", RealGEZ)],
                    [("product", Email)],
                    [("quantity", Card)],
                    [("date", Card)],
                    [("zip_code", AlphaNumeric)],
                    [("address", AlphaNumeric)]],

    "check-order" : [[("order_id", AlphaNumeric)]],

    "review-order" : [[("customer", AlphaNumeric)],
                    [("product", Email)],
                    [("stars", Stars)],
                    [("comment", Email)]]
    
}


apis = { "seller" : ["register-seller", "add-product", "update-product"],
        "courier" : ["register-courier", "take-delivery", "update-delivery-status", "view-pending-orders"],
        "customer" : ["register-customer", "search-products", "make-order", "check-order", "review-order"]}

ports = [("seller", 42071), ("courier", 42070), ("customer", 42069)]