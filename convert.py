#!/usr/bin/python3

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa

def convert_to_pem(n, e, d, p, q, dp, dq, qp):
    private_key = rsa.RSAPrivateNumbers(
        p=int(p),
        q=int(q),
        d=int(d),
        dmp1=int(dp),
        dmq1=int(dq),
        iqmp=int(qp),
        public_numbers=rsa.RSAPublicNumbers(e=int(e), n=int(n))
    ).private_key(backend=default_backend())

    pem_key = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.TraditionalOpenSSL,
        encryption_algorithm=serialization.NoEncryption()
    )

    return pem_key.decode('utf-8')

def get_input(prompt):
    return input(prompt).strip()

def main():
    print("Veuillez entrer les valeurs des variables RSA :")
    n = get_input("Variable N ? :")
    e = get_input("Variable E ? :")
    d = get_input("Variable D ? :")
    p = get_input("Variable P ? :")
    q = get_input("Variable Q ? :")
    dp = get_input("Variable DP ? :")
    dq = get_input("Variable DQ ? :")
    qp = get_input("Variable QP ? :")

    pem_key = convert_to_pem(n, e, d, p, q, dp, dq, qp)

    print("\nClé PEM générée avec succès:")
    print(pem_key)

if __name__ == "__main__":
    main()
