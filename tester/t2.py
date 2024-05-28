import socket
ports = [("courier", 42070), ("customer", 42069), ("seller", 42071)]
HOST = "127.0.0.1"  # L'indirizzo IP del server
PORT = 42071  # La porta del server

# Imposta un seed per la generazione casuale

# Definisci una richiesta di esempio

request_string = "add-product name l1231dqdlazio!! description prdotto##molto##molto##bello! price_tag 69 seller 1 warehouse 1"
#request_string = "add-product code banana name yoyo description ahahahaha##baba price 341.3243"

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        print(f"Inviando la richiesta: {request_string}")

        # Invia la richiesta al server
        s.send(request_string.encode())
        print("post invio")
        # Ricevi la risposta dal server
        response = s.recv(2048).decode()
        print(f"Risposta ricevuta: {response}")

