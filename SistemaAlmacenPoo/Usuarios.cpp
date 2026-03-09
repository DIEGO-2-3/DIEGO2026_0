#include "Usuarios.h"
#include <fstream>

Administrador::Administrador(string u, string p) : Usuario(u, p, Rol::ADMIN) {}

void Administrador::mostrarMenu(SistemaAlmacen& sistema) {
    int op;
    do {
        GestorUI::titulo("SISTEMA DE ALMACEN v3.0 (POO)");
        cout << "Usuario: " << username << " ";
        GestorUI::color(GestorUI::COLOR_ROJO); cout << "[ADMINISTRADOR]\n"; GestorUI::color(GestorUI::COLOR_DEFECTO);
        
        cout << "\n1. Registrar Nuevo Producto\n2. Agregar Stock (Entrada)\n3. Realizar Venta (Salida)\n"
             << "4. Ver Inventario\n5. Ver Historial Completo\n6. Reporte de Ventas Mensual\n"
             << "7. Eliminar Producto\n8. Modificar Precio/Stock\n9. Configurar Stock Minimo\n10. Cerrar Sesion\nOpcion: "; 
        cin >> op;

        switch (op) {
            case 1: sistema.registrarProducto(); break;
            case 2: sistema.registrarMovimiento(TipoMovimiento::ENTRADA); break;
            case 3: sistema.registrarMovimiento(TipoMovimiento::SALIDA); break;
            case 4: sistema.verInventario(); break;
            case 5: sistema.verHistorial(); break;
            case 6: sistema.reporteMensual(); break;
            case 7: sistema.eliminarProducto(); break;
            case 8: sistema.modificarProducto(true); break;
            case 9: sistema.modificarProducto(false); break;
            case 10: sistema.detenerSistema(); break;
        }
    } while (op != 10);
}

Empleado::Empleado(string u, string p) : Usuario(u, p, Rol::EMPLEADO) {}

void Empleado::mostrarMenu(SistemaAlmacen& sistema) {
    int op;
    do {
        GestorUI::titulo("SISTEMA DE ALMACEN v3.0 (POO)");
        cout << "Usuario: " << username << " ";
        GestorUI::color(GestorUI::COLOR_VERDE); cout << "[EMPLEADO]\n"; GestorUI::color(GestorUI::COLOR_DEFECTO);
        
        cout << "\n1. Registrar Nuevo Producto\n2. Realizar Venta (Salida)\n"
             << "3. Ver Inventario\n4. Ver Historial Completo\n5. Reporte de Ventas Mensual\n"
             << "6. Cerrar Sesion\nOpcion: "; 
        cin >> op;

        switch (op) {
            case 1: sistema.registrarProducto(); break;
            case 2: sistema.registrarMovimiento(TipoMovimiento::SALIDA); break;
            case 3: sistema.verInventario(); break;
            case 4: sistema.verHistorial(); break;
            case 5: sistema.reporteMensual(); break;
            case 6: sistema.detenerSistema(); break;
        }
    } while (op != 6);
}

GestorSesion::GestorSesion() {
    cargarUsuarios();
    if (usuarios.empty()) guardarUsuariosDefault();
}

void GestorSesion::cargarUsuarios() {
    ifstream f("usuarios.txt");
    if (!f.is_open()) return;
    string u, p, rolStr;
    while (f >> u >> p >> rolStr) {
        string pass = Seguridad::decodificar(p);
        if (rolStr == "ADMIN") usuarios.push_back(make_unique<Administrador>(u, pass));
        else usuarios.push_back(make_unique<Empleado>(u, pass));
    }
    f.close();
}

void GestorSesion::guardarUsuariosDefault() {
    ofstream f("usuarios.txt");
    f << "admin " << Seguridad::codificar("admin") << " ADMIN\n";
    f << "vendedor " << Seguridad::codificar("123") << " EMPLEADO\n";
    f.close();
    cargarUsuarios(); 
}

Usuario* GestorSesion::login() {
    int intentos = 3;
    while (intentos--) {
        GestorUI::titulo("INICIO DE SESION");
        string u, p;
        cout << "Usuario: "; cin >> u;
        cout << "Clave: "; p = Seguridad::leerPassword();

        for (const auto& user : usuarios) {
            if (user->getUsername() == u && user->getPassword() == p) {
                GestorUI::mensajeExito("Bienvenido " + u);
                return user.get(); 
            }
        }
        GestorUI::mensajeError("Credenciales incorrectas. Intentos: " + to_string(intentos));
    }
    return nullptr;
}