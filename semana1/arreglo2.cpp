#include <iostream> // Para usar std::cout

int main() {
    // Tu código actual:
    int tablero[3][3] = {
        {1, 2, 3}, // Fila 0
        {4, 5, 6}, // Fila 1
        {7, 8, 9}  // Fila 2
    };

    // Ejemplo de cómo acceder a un dato:
    std::cout << "El numero en el centro es: " << tablero[1][1] << std::endl;

    return 0; // Indica que el programa terminó con éxito
}