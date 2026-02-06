#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Parte A: Modelado del struct 
struct Producto {
    int codigo;
    string nombre;
    float precio;
    int stock;
};

// Prototipos de funciones
void registrarProductos(Producto inventario[], int &n);
void mostrarProductos(Producto inventario[], int n);
void buscarPorCodigo(Producto inventario[], int n);
void buscarPorNombre(Producto inventario[], int n);
void mostrarMayorStock(Producto inventario[], int n);
void mostrarMasCaro(Producto inventario[], int n);
void calcularTotalInventario(Producto inventario[], int n);
void ordenarPorPrecio(Producto inventario[], int n); // Reto [cite: 58]
void ordenarPorNombre(Producto inventario[], int n); // Reto [cite: 60]

int main() {
    const int MAX = 100; // Capacidad máxima del arreglo [cite: 26]
    Producto inventario[MAX];
    int n = 0; // Cantidad actual de productos registrados
    int opcion;

    do {
        // Parte C: Menú obligatorio [cite: 34-42]
        cout << "\n--- MINI SISTEMA DE TIENDA ---" << endl;
        cout << "1. Registrar productos" << endl;
        cout << "2. Mostrar productos" << endl;
        cout << "3. Buscar producto por codigo" << endl;
        cout << "4. Buscar producto por nombre" << endl;
        cout << "5. Mostrar producto con mayor stock" << endl;
        cout << "6. Mostrar producto mas caro" << endl;
        cout << "7. Calcular valor total del inventario" << endl;
        cout << "8. Salir" << endl;
        cout << "9. [RETO] Ordenar por precio (Mayor a Menor)" << endl;
        cout << "10. [RETO] Ordenar por nombre (A-Z)" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: registrarProductos(inventario, n); break;
            case 2: mostrarProductos(inventario, n); break;
            case 3: buscarPorCodigo(inventario, n); break;
            case 4: buscarPorNombre(inventario, n); break;
            case 5: mostrarMayorStock(inventario, n); break;
            case 6: mostrarMasCaro(inventario, n); break;
            case 7: calcularTotalInventario(inventario, n); break;
            case 8: cout << "Saliendo del sistema..." << endl; break;
            case 9: ordenarPorPrecio(inventario, n); break;
            case 10: ordenarPorNombre(inventario, n); break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 8);

    return 0;
}

// Parte B: Registro con validaciones [cite: 28-32]
void registrarProductos(Producto inventario[], int &n) {
    int cantidad;
    cout << "Cuantos productos desea registrar? (Minimo 5): ";
    cin >> cantidad;

    for (int i = 0; i < cantidad; i++) {
        cout << "\nProducto " << n + 1 << ":" << endl;
        cout << "Codigo: ";
        cin >> inventario[n].codigo;
        cin.ignore();
        cout << "Nombre: ";
        getline(cin, inventario[n].nombre);

        // Validacion de precio > 0 [cite: 31]
        do {
            cout << "Precio: ";
            cin >> inventario[n].precio;
            if (inventario[n].precio <= 0) cout << "Error: El precio debe ser mayor a 0." << endl;
        } while (inventario[n].precio <= 0);

        // Validacion de stock >= 0 [cite: 32]
        do {
            cout << "Stock: ";
            cin >> inventario[n].stock;
            if (inventario[n].stock < 0) cout << "Error: El stock no puede ser negativo." << endl;
        } while (inventario[n].stock < 0);

        n++;
    }
}

// Parte D: Lógica de visualización en tabla [cite: 45]
void mostrarProductos(Producto inventario[], int n) {
    if (n == 0) {
        cout << "No hay productos registrados." << endl;
        return;
    }
    cout << "\n" << left << setw(10) << "CODIGO" << setw(20) << "NOMBRE" << setw(10) << "PRECIO" << setw(10) << "STOCK" << endl;
    cout << "---------------------------------------------------" << endl;
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << inventario[i].codigo 
             << setw(20) << inventario[i].nombre 
             << setw(10) << inventario[i].precio 
             << setw(10) << inventario[i].stock << endl;
    }
}

// Busqueda por codigo [cite: 47]
void buscarPorCodigo(Producto inventario[], int n) {
    int cod;
    bool encontrado = false;
    cout << "Ingrese codigo a buscar: ";
    cin >> cod;
    for (int i = 0; i < n; i++) {
        if (inventario[i].codigo == cod) {
            cout << "Producto encontrado: " << inventario[i].nombre << " | Stock: " << inventario[i].stock << endl;
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "No encontrado" << endl;
}

// Busqueda por nombre [cite: 49]
void buscarPorNombre(Producto inventario[], int n) {
    string nom;
    bool encontrado = false;
    cout << "Ingrese nombre a buscar: ";
    cin.ignore();
    getline(cin, nom);
    for (int i = 0; i < n; i++) {
        if (inventario[i].nombre == nom) {
            cout << "Producto encontrado! Codigo: " << inventario[i].codigo << " | Precio: " << inventario[i].precio << endl;
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "No encontrado" << endl;
}

// Mayor stock [cite: 51]
void mostrarMayorStock(Producto inventario[], int n) {
    if (n == 0) return;
    int maxIdx = 0;
    for (int i = 1; i < n; i++) {
        if (inventario[i].stock > inventario[maxIdx].stock) maxIdx = i;
    }
    cout << "Producto con mayor stock: " << inventario[maxIdx].nombre << " (" << inventario[maxIdx].stock << " unidades)" << endl;
}

// Producto mas caro [cite: 53]
void mostrarMasCaro(Producto inventario[], int n) {
    if (n == 0) return;
    int caroIdx = 0;
    for (int i = 1; i < n; i++) {
        if (inventario[i].precio > inventario[caroIdx].precio) caroIdx = i;
    }
    cout << "Producto mas caro: " << inventario[caroIdx].nombre << " (Precio: " << inventario[caroIdx].precio << ")" << endl;
}

// Valor total: suma(precio * stock) [cite: 55]
void calcularTotalInventario(Producto inventario[], int n) {
    float total = 0;
    for (int i = 0; i < n; i++) {
        total += (inventario[i].precio * inventario[i].stock);
    }
    cout << "El valor total del inventario es: $" << total << endl;
}

// Reto: Ordenar por precio (Mayor a Menor) [cite: 58]
void ordenarPorPrecio(Producto inventario[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (inventario[j].precio < inventario[j+1].precio) {
                swap(inventario[j], inventario[j+1]);
            }
        }
    }
    cout << "Inventario ordenado por precio (Descendente)." << endl;
    mostrarProductos(inventario, n);
}

// Reto: Ordenar por nombre (A-Z) [cite: 60]
void ordenarPorNombre(Producto inventario[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (inventario[j].nombre > inventario[j+1].nombre) {
                swap(inventario[j], inventario[j+1]);
            }
        }
    }
    cout << "Inventario ordenado alfabeticamente." << endl;
    mostrarProductos(inventario, n);
}