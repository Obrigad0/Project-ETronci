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
 "add-product" : [[("code", AlphaNumeric)],
                  [("name", StringS)],
                  [("description", StringL)],
                  [("price", RealGEZ)]],

 "add-restock" : [[("quantity", IntGZ)],
                  [("supplier", StringS)],
                  [("product", AlphaNumeric)]],

 "add-supplier" : [[("business_name", StringS)]],

 "add-customer" : [[("name", StringS)],
                   [("surname", StringS)],
                   [("email", Email)],
                   [("phone-number", PhoneNumber)]],

 "add-address" : [[("email", Email)],
                  [("zip_code", AlphaNumeric)],
                  [("street_number", AlphaNumeric)],
                  [("street", StringM)],
                  [("city", Identifier)]],

 "add-card" : [[("number", Card)],
               [("email", Email)]],

 "add-purchase" : [[("fare", RealGEZ)],
                   [("customer", Email)],
                   [("card", Card)],
                   [("zip_code", AlphaNumeric)],
                   [("street", StringM)],
                   [("street_number", AlphaNumeric)],
                   [("product", AlphaNumeric)],
                   [("quantity", IntGZ)]],

 "add-rating" : [[("product", AlphaNumeric)],
                 [("customer", Email)],
                 [("stars", Stars)]],

 "add-refund" : [[("assigned_delivery", AlphaNumeric)],
                 [("product", AlphaNumeric)],
                 [("quantity", IntGZ)]],

 "delete-purchase" : [[("purchase", Identifier)]],

 "search-products" : [[("product_name", StringS)]],

 "view-order" : [[("purchase_id", Identifier)]],

 "view-refund" : [[("refund_id", Identifier)]],

 "update-product" : [[("code", AlphaNumeric)],
                     [("description", StringL)]],

 "take-purchase-delivery" : [[("courier", StringS)],
                             [("purchase", Identifier)],
                             [("deliverycode", AlphaNumeric)]],
 
 "update-assigned-delivery" : [[("deliverycode", AlphaNumeric)],
                               [("start", Timestamp), ("lost", Timestamp), ("end", Timestamp)]],
 
 "update-refund-request" : [[("refund", Identifier)],
                            [("courier", StringS), ("refund_start", Timestamp), ("refund_end", Timestamp)]],

 "view-statistic" : [[("supplier", StringS)]],
 
 "view-available-purchase" : []
}


apis = {"courier" : ["take-purchase-delivery", "update-assigned-delivery", "update-refund-request", "view-available-purchase"],
       "customer" : ["add-customer", "add-address", "search-products", "add-card", "add-purchase", "add-rating", "add-refund-request", "delete-purchase", "view-order", "view-refund"],
       "supplier" : ["add-product", "add-restock", "add-supplier", "update-product", "view-statistic"]}

ports = [("courier", 42070), ("customer", 42069), ("supplier", 42071)]