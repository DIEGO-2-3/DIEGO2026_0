#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>

using namespace std;

// ==========================================
// 1. CLASES DE ENTIDAD (Datos)
// ==========================================

class Usuario {
public:
    string username;
    string password;
    int rol; // 0: ADMIN, 1: EMPLEADO
    
    Usuario(string u = "", string p = "", int r = 1) : username(u), password(p), rol(r) {}
};

class Producto {
public:
    string nombre;
    float stock;
    float precio;
    int unidad; // 0: UNIDAD, 1: KG
    int stockMinimo;

    Producto(string n = "", float s = 0, float p = 0, int u = 0, int sm = 5)
        : nombre(n), stock(s), precio(p), unidad(u), stockMinimo(sm) {}
        
    string obtenerUnidadStr() {
        return (unidad == 1) ? "kg" : "unid";
    }
};

class Movimiento {
public:
    string producto;
    string tipo; // "ENTRADA" o "SALIDA"
    float cantidad;
    float total;
    int dia, mes, anio;

    Movimiento(string prod, string t, float cant, float tot, int d, int m, int a)
        : producto(prod), tipo(t), cantidad(cant), total(tot), dia(d), mes(m), anio(a) {}
};

// ==========================================
// 2. CLASE CONTROLADOR (Lógica del Sistema)
// ==========================================

class SistemaAlmacen {
private:
    // ENCAPSULAMIENTO: Los vectores ahora son privados. 
    // Nadie fuera de esta clase puede alterarlos directamente.
    vector<Usuario> usuarios;
    vector<Producto> inventario;
    vector<Movimiento> historial;
    Usuario usuarioActual;

    // Métodos utilitarios ocultos al usuario final
    void color(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); }
    
    void titulo(string texto) {
        system("cls");
        color(11); // Azul
        cout << "========================================\n";
        cout << "   " << texto << "\n";
        cout << "========================================\n";
        color(7);  // Defecto
    }

    int buscarProducto(string nombre) {
        for (size_t i = 0; i < inventario.size(); i++) {
            if (inventario[i].nombre == nombre) return i;
        }
        return -1;
    }

    // Aquí irían tus métodos de archivos (cargarUsuarios, guardarInventario, etc.)
    void cargarDatos() {
        // Lógica de ifstream...
    }
    void guardarDatos() {
        // Lógica de ofstream...
    }

public:
    // CONSTRUCTOR: Se ejecuta automáticamente al crear el sistema
    SistemaAlmacen() {
        cargarDatos();
        // Si no hay usuarios, creamos un admin por defecto
        if(usuarios.empty()) {
            usuarios.push_back(Usuario("admin", "123", 0));
        }
    }

    bool iniciarSesion() {
        int intentos = 3;
        while (intentos--) {
            titulo("INICIO DE SESION");
            string u, p;
            cout << "Usuario: "; cin >> u;
            cout << "Clave: "; cin >> p; // Simplificado para el ejemplo
            
            for (auto &x : usuarios) {
                if (x.username == u && x.password == p) {
                    usuarioActual = x;
                    cout << "\nBienvenido " << u << "!\n";
                    Sleep(1000);
                    return true;
                }
            }
            cout << "Credenciales incorrectas. Intentos: " << intentos << "\n";
            system("pause");
        }
        return false;
    }

    void registrarProducto() {
        titulo("NUEVO PRODUCTO");
        string nom; float stk, prec; int und;
        
        cout << "Nombre (sin espacios): "; cin >> nom;
        if (buscarProducto(nom) != -1) { 
            cout << "El producto ya existe.\n"; system("pause"); return; 
        }
        
        cout << "Unidad (0-Unidad, 1-Kg): "; cin >> und;
        cout << "Stock inicial: "; cin >> stk;
        cout << "Precio Venta: S/. "; cin >> prec;

        inventario.push_back(Producto(nom, stk, prec, und, 5));
        guardarDatos();
        cout << "Producto registrado correctamente.\n";
        system("pause");
    }

    void verInventario() {
        titulo("INVENTARIO GENERAL");
        cout << left << setw(15) << "NOMBRE" << setw(10) << "STOCK" 
             << setw(10) << "UNIDAD" << setw(12) << "PRECIO" << "\n";
        cout << string(50, '-') << endl;

        for (auto &p : inventario) {
            cout << left << setw(15) << p.nombre << setw(10) << p.stock 
                 << setw(10) << p.obtenerUnidadStr() << "S/." << p.precio << "\n";
        }
        cout << "\n"; system("pause");
    }

    void menuPrincipal() {
        if (!iniciarSesion()) return;

        int op;
        do {
            titulo("SISTEMA DE ALMACEN POO");
            cout << "1. Registrar Nuevo Producto\n";
            cout << "2. Ver Inventario\n";
            cout << "3. Salir\n";
            cout << "Opcion: "; cin >> op;

            switch (op) {
                case 1: registrarProducto(); break;
                case 2: verInventario(); break;
                case 3: cout << "Cerrando sistema...\n"; break;
            }
        } while (op != 3);
    }
};

// ==========================================
// 3. PUNTO DE ENTRADA (main)
// ==========================================
int main() {
    // Instanciamos nuestro objeto principal y lo arrancamos
    SistemaAlmacen miSistema;
    miSistema.menuPrincipal();
    
    return 0;
}