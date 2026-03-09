#include "Usuarios.h"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    system("title SISTEMA DE ALMACEN POO"); 
    
    GestorSesion sesion;
    Usuario* usuarioActual = sesion.login();

    if (usuarioActual != nullptr) {
        SistemaAlmacen sistema;
        usuarioActual->mostrarMenu(sistema);
    }

    return 0;
}