#include <iostream>
#include <string>

using namespace std;

class Celular {
private: 
    // 1) Atributos privados: ya no son accesibles directamente [cite: 27]
    string marca;
    string modelo;
    int bateria;

public:
    // 3) Setters con validación [cite: 29]
    void setMarca(string m) {
        marca = m;
    }

    void setModelo(string mod) {
        modelo = mod;
    }

    void setBateria(int b) {
        // Validación: batería solo entre 0 y 100 [cite: 30]
        if (b >= 0 && b <= 100) {
            bateria = b;
        } else {
            cout << "Error: Valor de bateria (" << b << ") invalido. Debe estar entre 0 y 100." << endl;
            bateria = 0; // Asignamos un valor seguro por defecto
        }
    }

    // 2) Getters const (prometen no modificar el objeto) [cite: 28]
    string getMarca() const { return marca; }
    string getModelo() const { return modelo; }
    int getBateria() const { return bateria; }

    void mostrarInfo() {
        cout << "Celular: " << marca << " " << modelo << " | Bateria: " << bateria << "%" << endl;
    }
};

int main() {
    Celular miCel;

    // 4) Prueba con valores válidos
    miCel.setMarca("Xiaomi");
    miCel.setModelo("Redmi Note 12");
    miCel.setBateria(80);
    cout << "--- Prueba con valores validos ---" << endl;
    miCel.mostrarInfo();

    // 4) Prueba con valores inválidos [cite: 31]
    cout << "\n--- Prueba con valores invalidos ---" << endl;
    miCel.setBateria(150);  // Debería mostrar error [cite: 31]
    miCel.setBateria(-20);  // Debería mostrar error [cite: 31]
    
    cout << "Estado final tras intentos fallidos:" << endl;
    miCel.mostrarInfo();

    return 0;
}