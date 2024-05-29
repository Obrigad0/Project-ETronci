import socket
import random
import string
from test_functions import requests

methods = { "seller" : ["register-seller", "add-product", "update-product"],
            "courier" : ["register-courier", "take-delivery", "update-delivery-status", "view-pending-orders"],
            "customer" : ["register-customer", "search-products", "make-order", "check-order", "review-order"]}

ports = {"courier" : 42070, "customer" : 42069, "seller" : 42071}

HOST = "127.0.0.1"  # L'indirizzo IP del server
PORT = 42069  # La porta del server

RANDOM_SEED = 71 # Imposta un seed per la generazione casuale

# Richiesta esempio : nome-funzione key1 value1 ... keyN valueN
request_string = "check-order order_id 2"
#request_string = "take-delivery orderId 2 courier 1 status shipped"

def generate_random_argument(parameter_class):
    return parameter_class().receive_random_value()

def generate_random_request(method):
    method_name = random.choice(methods[method])
    
    PORT = ports[method]

    request_args = []
    for arg_set in requests[method_name]:
        arg_values = {}
        for arg_name, arg_class in arg_set:
            arg_values[arg_name] = generate_random_argument(arg_class)
        request_args.append(arg_values)

    request_string = f"{method_name}"
    for arg_values in request_args:
        for arg_name, arg_value in arg_values.items():
            if type(arg_value) == string:
                arg_value.replace(" ", "##")              # è giusto così lo spacing ?
            request_string += f" {arg_name} {arg_value}"
    
    return request_string.strip()


if __name__ == "__main__":
    totale = 10
    richieste = 50
    succesful = 0
    failed = 0
    
    for _ in range(totale):
        
        method = random.choice(list(methods.keys()))
        PORT = ports[method]
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
  
            for _ in range(richieste):
                request_string = generate_random_request(method)
                print(f"Inviando la richiesta: {request_string}")
    
                s.send(request_string.encode()) # Invia la richiesta al server
                print("post invio")
                
                response = s.recv(2048).decode() # Ricevi la risposta dal server
                print(f"Risposta ricevuta: {response}")
    
                if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR"):
                    failed += 1
                else:
                    succesful += 1
                            
    print(f"\n succesful requests: {succesful}/{totale} \n failed requests: {failed}/{totale} \n\n")
    
