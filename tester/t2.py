import socket
ports = [("courier", 42070), ("customer", 42069), ("seller", 42071)]
HOST = "127.0.0.1"  # L'indirizzo IP del server
PORT = 42069  # La porta del server

# Imposta un seed per la generazione casuale

# Definisci una richiesta di esempio

request_string = "check-order order_id 2"
#request_string = "take-delivery orderId 2 courier 1 status shipped "

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

