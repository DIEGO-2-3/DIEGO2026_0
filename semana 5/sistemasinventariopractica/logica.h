#ifndef LOGICA_H
#define LOGICA_H

#include "datos.h"

/* ================= SEGURIDAD Y PASSWORD ================= */
string leerPassword() {
    string pass; char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { cout << endl; break; }
        if (ch == 8) { 
            if (!pass.empty()) { pass.pop_back(); cout << "\b \b"; }
        } else { pass += ch; cout << "*"; }
    }
    return pass;
}

string codificar(string p) { for (char &c : p) c += 3; return p; }
string decodificar(string p) { for (char &c : p) c -= 3; return p; }

/* ================= ARCHIVOS ================= */
void guardarUsuarios() {
    ofstream f("usuarios.txt");
    for (auto &u : usuarios)
        f << u.username << " " << codificar(u.password) << " " << (u.rol == ADMIN ? "ADMIN" : "EMPLEADO") << endl;
    f.close();
}

void cargarUsuarios() {
    ifstream f("usuarios.txt");
    if (!f.is_open()) return;
    usuarios.clear(); Usuario u; string rolStr;
    while (f >> u.username >> u.password >> rolStr) {
        u.password = decodificar(u.password);
        u.rol = (rolStr == "ADMIN") ? ADMIN : EMPLEADO;
        usuarios.push_back(u);
    }
    f.close();
}

void guardarInventario() {
    ofstream f("productos.txt");
    for (auto &p : inventario)
        f << p.nombre << " " << p.stock << " " << p.precio << " " << (int)p.unidad << " " << p.stockMinimo << endl;
    f.close();
}

void cargarInventario() {
    ifstream f("productos.txt");
    if (!f.is_open()) return;
    inventario.clear(); Producto p; int unidadTemp;
    while (f >> p.nombre >> p.stock >> p.precio >> unidadTemp >> p.stockMinimo) {
        p.unidad = (TipoUnidad)unidadTemp;
        inventario.push_back(p);
    }
    f.close();
}

void guardarHistorial(Movimiento m) {
    ofstream f("historial.txt", ios::app);
    f << m.producto << " " << m.tipo << " " << m.cantidad << " " << m.total << " " << m.fecha.d << " " << m.fecha.m << " " << m.fecha.a << endl;
    f.close();
}

/* ================= FUNCIONES LÓGICAS ================= */
bool login() {
    int intentos = 3;
    while (intentos--) {
        titulo("INICIO DE SESION");
        string u, p;
        cout << "Usuario: "; cin >> u;
        cout << "Clave: "; p = leerPassword();
        for (auto &x : usuarios) {
            if (x.username == u && x.password == p) {
                usuarioActual = x;
                mensajeExito("Bienvenido " + u);
                return true;
            }
        }
        mensajeError("Credenciales incorrectas. Intentos: " + to_string(intentos));
    }
    return false;
}

int buscarProducto(string n) {
    for (int i = 0; i < inventario.size(); i++)
        if (inventario[i].nombre == n) return i;
    return -1;
}

void registrarProducto() {
    titulo("NUEVO PRODUCTO");
    Producto p;
    cout << "Nombre (sin espacios): "; cin >> p.nombre;
    if (buscarProducto(p.nombre) != -1) { mensajeError("El producto ya existe."); return; }
    
    int t = leerEntero("Unidad (1-Unidad, 2-Kg): ");
    p.unidad = (t == 2) ? KG : UNIDAD;
    
    p.stock = leerFlotante("Stock inicial: ");
    p.precio = leerFlotante("Precio Venta: S/. ");
    p.stockMinimo = 5;

    inventario.push_back(p);
    guardarInventario();
    mensajeExito("Producto registrado correctamente");
}

void registrarMovimiento(bool entrada) {
    titulo(entrada ? "ENTRADA DE STOCK" : "VENTA / SALIDA");
    string n; cout << "Nombre del Producto: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { mensajeError("Producto no encontrado."); return; }

    color(COLOR_AMARILLO);
    cout << "Stock actual: " << inventario[i].stock << " " << strUnidad(inventario[i].unidad) << endl;
    color(COLOR_DEFECTO);

    float cant = leerFlotante(entrada ? "Cantidad a ingresar: " : "Cantidad a retirar: ");

    if (!entrada && cant > inventario[i].stock) { mensajeError("Stock insuficiente."); return; }

    inventario[i].stock += entrada ? cant : -cant;
    Movimiento m = {n, entrada ? "ENTRADA" : "SALIDA", cant, cant * inventario[i].precio};
    
    cout << "--- Fecha ---" << endl;
    m.fecha.d = leerEntero("Dia (DD): ");
    m.fecha.m = leerEntero("Mes (MM): ");
    m.fecha.a = leerEntero("Anio (AAAA): ");

    historial.push_back(m);
    guardarInventario();
    guardarHistorial(m);

    if (!entrada) {
        cout << "\n"; color(COLOR_BLANCO_BRILLANTE);
        cout << "========= TICKET DE VENTA =========\n";
        cout << "Producto: " << n << endl;
        cout << "Cantidad: " << cant << " " << strUnidad(inventario[i].unidad) << endl;
        cout << "Total:    S/. " << fixed << setprecision(2) << m.total << endl;
        cout << "===================================\n"; system("pause");
    } else mensajeExito("Stock actualizado.");
}

void eliminarProducto() {
    titulo("ELIMINAR PRODUCTO");
    string n; cout << "Nombre: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { mensajeError("No existe"); return; }
    inventario.erase(inventario.begin() + i);
    guardarInventario();
    mensajeExito("Producto eliminado");
}

/* ================= REPORTES ================= */
void verInventario() {
    titulo("INVENTARIO GENERAL");
    color(COLOR_BLANCO_BRILLANTE);
    cout << left << setw(15) << "NOMBRE" << setw(10) << "STOCK" << setw(10) << "UNIDAD" << setw(12) << "PRECIO" << setw(15) << "ESTADO" << endl;
    cout << string(62, '-') << endl; color(COLOR_DEFECTO);

    for (auto &p : inventario) {
        cout << left << setw(15) << p.nombre << setw(10) << p.stock << setw(10) << strUnidad(p.unidad) << "S/." << setw(9) << fixed << setprecision(2) << p.precio;
        if (p.stock <= p.stockMinimo) { color(COLOR_ROJO); cout << " [STOCK BAJO]"; } 
        else { color(COLOR_VERDE); cout << " [OK]"; }
        color(COLOR_DEFECTO); cout << endl;
    }
    cout << "\n"; system("pause");
}

void verHistorial() {
    titulo("HISTORIAL DE MOVIMIENTOS");
    ifstream f("historial.txt");
    if (!f.is_open()) { mensajeError("No hay historial."); return; }
    string prod, tipo; float cant, tot; int d, m, a;
    cout << left << setw(12) << "FECHA" << setw(15) << "PRODUCTO" << setw(10) << "TIPO" << setw(10) << "CANT" << "TOTAL" << endl;
    cout << string(60, '-') << endl;
    while (f >> prod >> tipo >> cant >> tot >> d >> m >> a) {
        color(tipo == "ENTRADA" ? COLOR_VERDE : COLOR_ROJO);
        cout << d << "/" << m << "/" << a << "   " << left << setw(15) << prod << setw(10) << tipo << setw(10) << cant << "S/." << tot << endl;
    }
    f.close(); color(COLOR_DEFECTO); system("pause");
}

void reporteMensual() {
    titulo("REPORTE MENSUAL");
    int mes = leerEntero("Mes (numero): ");
    int anio = leerEntero("Anio: ");
    float totalVentas = 0;
    
    ifstream f("historial.txt");
    string prod, tipo; float cant, tot; int d, m, a;
    bool encontrado = false;
    cout << "\n--- Ventas " << mes << "/" << anio << " ---\n";
    while (f >> prod >> tipo >> cant >> tot >> d >> m >> a) {
        if (m == mes && a == anio && tipo == "SALIDA") {
            cout << prod << " -> S/." << tot << endl;
            totalVentas += tot; encontrado = true;
        }
    }
    f.close();
    cout << string(30, '-') << endl;
    if (encontrado) { color(COLOR_AMARILLO); cout << "TOTAL VENDIDO: S/. " << fixed << setprecision(2) << totalVentas << endl; } 
    else cout << "Sin ventas en esa fecha.\n";
    color(COLOR_DEFECTO); system("pause");
}

/* ================= MENUS ================= */
void menuEntradaAdmin() {
    int op;
    do {
        titulo("PANEL ADMINISTRADOR");
        cout << "1. Agregar Stock (Compra)\n2. Eliminar Producto\n3. Modificar Precio/Stock\n4. Configurar Stock Minimo\n5. Volver\n"; 
        op = leerEntero(">> ");
        string n; int i;
        switch (op) {
            case 1: registrarMovimiento(true); break;
            case 2: eliminarProducto(); break;
            case 3: 
                cout << "Producto: "; cin >> n; i = buscarProducto(n);
                if(i!=-1) { 
                    inventario[i].precio = leerFlotante("Nuevo Precio: ");
                    inventario[i].stock = leerFlotante("Nuevo Stock: ");
                    guardarInventario(); 
                    mensajeExito("Actualizado"); 
                }
                else mensajeError("No encontrado"); break;
            case 4:
                cout << "Producto: "; cin >> n; i = buscarProducto(n);
                if(i!=-1) { 
                    inventario[i].stockMinimo = leerEntero("Nuevo Stock Minimo: ");
                    guardarInventario(); 
                    mensajeExito("Configurado"); 
                }
                else mensajeError("No encontrado"); break;
        }
    } while (op != 5);
}

void menu() {
    int op;
    do {
        titulo("SISTEMA DE ALMACEN v2.0 INVENTARIO");
        cout << "Usuario: " << usuarioActual.username << endl;
        if(usuarioActual.rol == ADMIN) { color(COLOR_ROJO); cout << "[ADMINISTRADOR]"; } else { color(COLOR_VERDE); cout << "[EMPLEADO]"; }
        color(COLOR_DEFECTO);
        cout << "\n\n1. Registrar Nuevo Producto\n2. Gestion de Stock (Entradas/Admin)\n3. Realizar Venta (Salida)\n4. Ver Inventario\n5. Ver Historial Completo\n6. Reporte de Ventas Mensual\n7. Cerrar Sesion\n"; 
        op = leerEntero("Opcion: ");
        
        switch (op) {
            case 1: registrarProducto(); break;
            case 2: if (usuarioActual.rol == ADMIN) menuEntradaAdmin(); else registrarMovimiento(true); break;
            case 3: registrarMovimiento(false); break;
            case 4: verInventario(); break;
            case 5: verHistorial(); break;
            case 6: reporteMensual(); break;
            case 7: cout << "Cerrando..." << endl; break;
        }
    } while (op != 7);
}

#endif