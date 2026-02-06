#include <iostream>
#include <string>
#include <vector>
#include <limits>    // Necesario para limpiar el buffer correctamente
#include <windows.h> // Necesario para arreglar los acentos en Windows

using namespace std;

// --- ESTRUCTURAS DE DATOS ---

struct Producto {
    string nombre;
    float stockFinal;
    bool alerta;
};

struct Usuario {
    string username;
    string password;
};

// --- BASE DE DATOS (En memoria) ---
vector<Usuario> usuariosRegistrados;

// --- FUNCIONES DE UTILIDAD ---

// Limpia la "basura" que queda en el teclado para evitar saltos de línea automáticos
void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Función robusta para pedir números y evitar que el programa explote si escriben letras
float validarEntrada(string mensaje) {
    float valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= 0) {
            return valor;
        } else {
            cout << ">> Error: Entrada inválida. Por favor ingrese un número positivo." << endl;
            cin.clear(); // Restaura el estado de cin
            limpiarBuffer(); // Elimina la entrada incorrecta
        }
    }
}

// --- MÓDULO DE SEGURIDAD (LOGIN/REGISTRO) ---

void registrarUsuario() {
    Usuario nuevoUser;
    cout << "\n=== REGISTRO DE USUARIO ===" << endl;
    cout << "Ingrese nuevo usuario: ";
    cin >> nuevoUser.username;

    // Verificar si el usuario ya existe para evitar duplicados
    for (const auto& u : usuariosRegistrados) {
        if (u.username == nuevoUser.username) {
            cout << ">> Error: El usuario '" << nuevoUser.username << "' ya existe.\n";
            return;
        }
    }

    cout << "Ingrese contraseña: ";
    cin >> nuevoUser.password;

    usuariosRegistrados.push_back(nuevoUser);
    cout << ">> ¡Registro exitoso! Ahora puede iniciar sesión.\n";
}

bool iniciarSesion() {
    if (usuariosRegistrados.empty()) {
        cout << ">> No hay usuarios registrados. Por favor registre uno primero.\n";
        return false;
    }

    string user, pass;
    cout << "\n=== INICIO DE SESIÓN ===" << endl;
    cout << "Usuario: ";
    cin >> user;
    cout << "Contraseña: ";
    cin >> pass;

    for (const auto& u : usuariosRegistrados) {
        if (u.username == user && u.password == pass) {
            cout << "\n>> ¡Bienvenido, " << user << "!" << endl;
            return true;
        }
    }

    cout << ">> Error: Usuario o contraseña incorrectos.\n";
    return false;
}

// --- MÓDULO DE INVENTARIO ---

void gestionarInventario() {
    int n;
    cout << "\n=== SISTEMA DE INVENTARIO MULTIPRODUCTO ===" << endl;
    
    // Validación para asegurar que 'n' es un número entero positivo
    while (true) {
        cout << "¿Cuántos productos desea registrar? ";
        if (cin >> n && n > 0) break;
        cout << ">> Error: Ingrese un número entero mayor a 0.\n";
        cin.clear();
        limpiarBuffer();
    }

    vector<Producto> lista(n);

    for(int i = 0; i < n; i++) {
        cout << "\n--- Producto " << i+1 << " ---" << endl;
        cout << "Nombre: ";
        limpiarBuffer(); // Importante: limpiar antes de leer texto con espacios
        getline(cin, lista[i].nombre);

        float inicial = validarEntrada("Stock inicial (kg): ");
        float consumo = validarEntrada("Consumo de hoy (kg): ");

        // Lógica de negocio: El consumo no puede ser mayor al stock existente
        if (consumo > inicial) {
            cout << ">> Advertencia: El consumo excede el stock. Se ajustará a 0." << endl;
            lista[i].stockFinal = 0;
        } else {
            lista[i].stockFinal = inicial - consumo;
        }
        
        lista[i].alerta = (lista[i].stockFinal < 5);
    }

    // Reporte final formateado
    cout << "\n======= REPORTE FINAL DE INVENTARIO =======" << endl;
    cout << "Estado\t\tStock\tProducto" << endl;
    cout << "-------------------------------------------" << endl;
    for(int i = 0; i < n; i++) {
        if(lista[i].alerta) cout << "[ALERTA]  \t";
        else cout << "[OK]      \t";
        
        cout << lista[i].stockFinal << " kg\t" << lista[i].nombre << endl;
    }
    cout << "-------------------------------------------" << endl;
    
    cout << "\nPresione Enter para volver al menú...";
    limpiarBuffer();
    cin.get();
}

// --- MENÚ PRINCIPAL ---

int main() {
    // ESTA LÍNEA ARREGLA LOS CARACTERES RAROS EN WINDOWS
    SetConsoleOutputCP(CP_UTF8); 

    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n################################" << endl;
        cout << "    GESTIÓN DE ALMACÉN v2.0    " << endl;
        cout << "################################" << endl;
        cout << "1. Registrar Nuevo Usuario" << endl;
        cout << "2. Iniciar Sesión" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opción: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            limpiarBuffer();
            opcion = 0; 
        }

        switch (opcion) {
            case 1:
                registrarUsuario();
                break;
            case 2:
                if (iniciarSesion()) {
                    gestionarInventario();
                }
                break;
            case 3:
                cout << "Saliendo del sistema..." << endl;
                salir = true;
                break;
            default:
                cout << ">> Opción no válida." << endl;
        }
    }

    return 0;
}   