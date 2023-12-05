#!/usr/bin/python3

import serial
import time
import re
import sys

# Remplacez "/dev/ttyUSB0" par le bon port série de votre ESP32
serial_port = "/dev/ttyUSB0"
baud_rate = 115200

# Fonction pour envoyer des données à l'ESP32
def send_data(data):
    ser.write(data.encode())

# Fonction pour recevoir des données de l'ESP32
def receive_data():
    return ser.read_all().decode().strip()

# Fonction pour convertir la clé découpée en PEM
def convert_to_pem(n, e, d, p, q, dp, dq, qp):
    pem_format = f"""-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA{n}
{n}
{n}
{n}
{n}
-----END RSA PRIVATE KEY-----
"""
    return pem_format

try:
    # Ouvrir la connexion série
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    time.sleep(2)  # Attente pour permettre à l'ESP32 de s'initialiser

    # Boucle interactive
    while True:
        user_input = input("Entrez une commande (q pour quitter): ")
        if user_input.lower() == 'q':
            break

        # Envoyer la commande à l'ESP32
        send_data(user_input)

        # Attendre et afficher la réponse de l'ESP32
        sys.stdout.write(receive_data() + '\n')
        sys.stdout.flush()

except serial.SerialException as e:
    print(f"Erreur d'ouverture du port série: {e}")
except KeyboardInterrupt:
    print("\nInterruption par l'utilisateur.")
finally:
    # Fermer la connexion série à la fin
    if ser.is_open:
        ser.close()
