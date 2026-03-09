#include <iostream>
#include <string>

using namespace std;

// 1) Definición de la clase Celular [cite: 21]
class Celular {
public:
    string marca;
    string modelo;
    int bateria;

    // 3) Método para imprimir la información [cite: 23]
    void mostrarInfo() {
        cout << "Marca: " << marca << " | Modelo: " << modelo << " | Bateria: " << bateria << "%" << endl;
    }
};

int main() {
    // 2) Creación de 2 objetos con valores distintos [cite: 22]
    Celular cel1;
    cel1.marca = "Samsung";
    cel1.modelo = "Galaxy S23";
    cel1.bateria = 85;

    Celular cel2;
    cel2.marca = "Apple";
    cel2.modelo = "iPhone 15";
    cel2.bateria = 92;

    // 4) Llamada al método para los 2 objetos [cite: 24]
    cout << "Informacion de cel1:" << endl;
    cel1.mostrarInfo();

    cout << "\nInformacion de cel2:" << endl;
    cel2.mostrarInfo();

    return 0;
}