#include <Arduino.h>
#include <mbedtls/config.h>
#include <mbedtls/platform.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/rsa.h>
#include <mbedtls/sha256.h>
#include <mbedtls/sha512.h>

mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_rsa_context rsa;

void printPEM(const char *label, mbedtls_mpi *mpi);
void printRSAKey(mbedtls_rsa_context *key);

void setup() {
  Serial.begin(115200);
  Serial.println("Bienvenue dans l'interpréteur de commandes ESP32!");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    executeCommand(command);
  }
}

void executeCommand(String command) {
  command.trim();  // Supprimer les espaces de début et de fin

  if (command.equals("generate_rsa_key")) {
    generateRSAKey();
  } else if (command.startsWith("sha256 ")) {
    String input = command.substring(7);
    hashSHA256(input);
  } else if (command.startsWith("sha512 ")) {
    String input = command.substring(7);
    hashSHA512(input);
  } else {
    Serial.println("Commande non reconnue. Les commandes disponibles sont :");
    Serial.println("- generate_rsa_key");
    Serial.println("- sha256 [chaine]");
    Serial.println("- sha512 [chaine]");
  }
}

void generateRSAKey() {
  mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
  
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  const char *pers = "rsa_gen";
  mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                        (const unsigned char *) pers, strlen(pers));

  mbedtls_rsa_gen_key(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, 2048, 65537);

  Serial.println("Paire de clés RSA 4096 bits générée avec succès!");

  printRSAKey(&rsa);

  mbedtls_rsa_free(&rsa);
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);
}

void printRSAKey(mbedtls_rsa_context *key) {
  Serial.println("----- Clé RSA N -----");
  printPEM("N = ", &key->N);
  Serial.println("----- Clé RSA E -----");
  printPEM("E = ", &key->E);
  Serial.println("----- Clé RSA P -----");
  printPEM("P = ", &key->P);
  Serial.println("----- Clé RSA Q -----");
  printPEM("Q = ", &key->Q);
  Serial.println("----- Clé RSA D -----");
  printPEM("D = ", &key->D);
  Serial.println("----- Clé RSA DP -----");
  printPEM("DP = ", &key->DP);
  Serial.println("----- Clé RSA DQ -----");
  printPEM("DQ = ", &key->DQ);
  Serial.println("----- Clé RSA QP -----");
  printPEM("QP = ", &key->QP);
}

void hashSHA256(String input) {
  mbedtls_sha256_context sha256;
  mbedtls_sha256_init(&sha256);

  mbedtls_sha256_starts(&sha256, 0);
  mbedtls_sha256_update(&sha256, (const unsigned char *)input.c_str(), input.length());

  unsigned char hash[32];
  mbedtls_sha256_finish(&sha256, hash);

  printHash(hash, 32);

  mbedtls_sha256_free(&sha256);
}

void hashSHA512(String input) {
  mbedtls_sha512_context sha512;
  mbedtls_sha512_init(&sha512);

  mbedtls_sha512_starts(&sha512, 0);
  mbedtls_sha512_update(&sha512, (const unsigned char *)input.c_str(), input.length());

  unsigned char hash[64];
  mbedtls_sha512_finish(&sha512, hash);

  printHash(hash, 64);

  mbedtls_sha512_free(&sha512);
}

void printHash(unsigned char *hash, int length) {
  Serial.print("Hash : ");
  for (int i = 0; i < length; i++) {
    Serial.print(String(hash[i], HEX));
  }
  Serial.println();
}

void printPEM(const char *label, mbedtls_mpi *mpi) {
  char buf[4096];  // Augmenter la taille du tampon
  size_t len = 0;

  int ret = mbedtls_mpi_write_string(mpi, 16, buf, sizeof(buf), &len);
  if (ret != 0) {
    Serial.println("Erreur lors de l'écriture de l'entier MPI dans la chaîne PEM");
    return;
  }

  Serial.print(label);
  Serial.println(buf);
}
