#include <sodium.h>
#include <iostream>

int main() {
    // Inicializar la biblioteca libsodium
    if (sodium_init() < 0) {
        std::cerr << "Error al inicializar libsodium." << std::endl;
        return 1; // Salida con error
    }

    // Generar un número pseudoaleatorio de 32 bits
    uint32_t random_number = randombytes_random();

    // Mostrar el número aleatorio generado
    std::cout << "Numero aleatorio generado: " << random_number << std::endl;

    return 0; // Salida exitosa
}
