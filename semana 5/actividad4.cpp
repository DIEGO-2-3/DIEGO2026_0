#include <iostream>
#include <string>

using namespace std;

class Celular {
private:
    string marca;
    string modelo;
    int bateria;

public:
    // 1) Constructor por defecto [cite: 41]
    Celular() {
        this->marca = "Desconocida";
        this->modelo = "Genérico";
        this->bateria = 100;
    }

    // 2) Constructor con 2 parámetros [cite: 42]
    Celular(string marca, string modelo) {
        this->marca = marca;
        this->modelo = modelo;
        this->bateria = 100; // Inicia en 100 por defecto
    }

    // 3) Constructor con 3 parámetros [cite: 43]
    Celular(string marca, string modelo, int bateria) {
        this->marca = marca;
        this->modelo = modelo;
        setBateria(bateria); // Usamos el setter para aprovechar la validación
    }

    // Setter con validación (del ejercicio anterior)
    void setBateria(int bateria) {
        if (bateria >= 0 && bateria <= 100) {
            this->bateria = bateria;
        } else {
            cout << "Valor de bateria invalido. Se asignara 0." << endl;
            this->bateria = 0;
        }
    }

    void mostrarInfo() {
        cout << "Marca: " << marca << " | Modelo: " << modelo << " | Bateria: " << bateria << "%" << endl;
    }
};

int main() {
    // 4) Crear 3 objetos usando cada constructor [cite: 44]
    cout << "--- Creando objetos con distintos constructores ---" << endl;

    Celular cel1; // Constructor por defecto
    Celular cel2("Nokia", "1100"); // Constructor con 2 parámetros
    Celular cel3("Motorola", "Edge 40", 85); // Constructor con 3 parámetros

    // Mostrar información
    cel1.mostrarInfo();
    cel2.mostrarInfo();
    cel3.mostrarInfo();

    return 0;
}