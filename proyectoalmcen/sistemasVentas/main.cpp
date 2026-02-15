#include "logica.h"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    system("title SISTEMA DE ALMACEN"); 
    
    // Carga de archivos
    cargarUsuarios();
    cargarInventario();

    // Usuario por defecto
    if (usuarios.empty()) {
        usuarios.push_back({"admin", "admin", ADMIN});
        usuarios.push_back({"vendedor", "123", EMPLEADO});
        guardarUsuarios();
    }

    // Inicio
    if (login()) {
        menu();
    }

    return 0;
}