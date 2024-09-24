#include <Crypto.h>
#include <ChaCha.h>

// Clave de 256 bits (32 bytes) y nonce de 64 bits (8 bytes)
byte key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
byte nonce[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

// Mensaje original (texto plano)
byte plaintext[] = "Vine a comala porque me dijeron que aca...";
byte ciphertext[sizeof(plaintext)];
byte decryptedtext[sizeof(plaintext)];

// Inicializa el objeto de ChaCha
ChaCha chacha;

void setup() {
  Serial.begin(115200);

  // Establece la clave y el nonce para ChaCha20
  chacha.setKey(key, sizeof(key));
  chacha.setIV(nonce, sizeof(nonce));

  // Cifrar el mensaje
  chacha.encrypt(ciphertext, plaintext, sizeof(plaintext));

  // Imprimir mensaje cifrado
  Serial.print("Mensaje cifrado: ");
  for (size_t i = 0; i < sizeof(ciphertext); i++) {
    Serial.print(ciphertext[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Desencriptar el mensaje
  chacha.setIV(nonce, sizeof(nonce));  // Reinicializa el nonce
  chacha.decrypt(decryptedtext, ciphertext, sizeof(ciphertext));

  // Imprimir mensaje descifrado
  Serial.print("Mensaje descifrado: ");
  for (size_t i = 0; i < sizeof(decryptedtext); i++) {
    Serial.print((char)decryptedtext[i]);
  }
  Serial.println();
}

void loop() {
  // No hace falta repetir nada
}
