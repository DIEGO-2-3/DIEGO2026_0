#include <iostream>
#include <string>

using namespace std;

class Celular {
private: 
    string marca;
    string modelo;
    int bateria;

public:
    // 1) Setters con parámetros con el mismo nombre que el atributo 
    // 2) Uso de this-> para diferenciar 
    void setMarca(string marca) {
        this->marca = marca; 
    }

    void setModelo(string modelo) {
        this->modelo = modelo;
    }

    void setBateria(int bateria) {
        // Seguimos aplicando la validación de la actividad anterior [cite: 30]
        if (bateria >= 0 && bateria <= 100) {
            this->bateria = bateria;
        } else {
            cout << "Error: Valor invalido para bateria." << endl;
            this->bateria = 0;
        }
    }

    // Getters
    string getMarca() const { return this->marca; }
    string getModelo() const { return this->modelo; }
    int getBateria() const { return this->bateria; }

    void mostrarInfo() {
        cout << "Celular: " << marca << " " << modelo << " | Bateria: " << bateria << "%" << endl;
    }
};

int main() {
    // 3) Prueba de que el programa sigue funcionando [cite: 37]
    Celular miCel;
    miCel.setMarca("Google");
    miCel.setModelo("Pixel 8");
    miCel.setBateria(95);

    cout << "--- Prueba Actividad 3 (this->) ---" << endl;
    miCel.mostrarInfo();

    return 0;
}