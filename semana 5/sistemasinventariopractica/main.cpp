#include "logica.h"

int main() {
    // Configuración de consola
    SetConsoleOutputCP(CP_UTF8); 
    system("title SISTEMA DE ALMACEN"); 
    
    // 1. Carga de archivos (Base de datos)
    cargarUsuarios();
    cargarInventario();

    // 2. Configuración inicial: Usuario por defecto si la lista está vacía
    if (usuarios.empty()) {
        usuarios.push_back({"admin", "admin", ADMIN});
        usuarios.push_back({"vendedor", "123", EMPLEADO});
        guardarUsuarios();
    }

    // 3. Inicio del flujo del programa
    if (login()) {
        menu();
    }

    return 0;
}