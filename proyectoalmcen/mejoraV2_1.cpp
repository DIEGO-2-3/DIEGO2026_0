#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip> // Para tablas bonitas
#include <limits>
#include <conio.h>
#include <windows.h> // Para colores y sistema

using namespace std;

/* ================= COLORES ================= */
// Definimos colores para hacerlo más fácil de usar
#define COLOR_DEFECTO 7
#define COLOR_AZUL 11
#define COLOR_VERDE 10
#define COLOR_ROJO 12
#define COLOR_AMARILLO 14
#define COLOR_BLANCO_BRILLANTE 15

void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void titulo(string texto) {
    system("cls");
    color(COLOR_AZUL);
    cout << "========================================" << endl;
    cout << "   " << texto << endl;
    cout << "========================================" << endl;
    color(COLOR_DEFECTO);
}

void mensajeExito(string texto) {
    color(COLOR_VERDE);
    cout << "[!] " << texto << endl;
    color(COLOR_DEFECTO);
    Sleep(1000); // Pausa breve
}

void mensajeError(string texto) {
    color(COLOR_ROJO);
    cout << "[ERROR] " << texto << endl;
    color(COLOR_DEFECTO);
    system("pause");
}

/* ================= ENUMS ================= */
enum Rol { ADMIN, EMPLEADO };
enum TipoUnidad { UNIDAD, KG };

/* ================= STRUCTS ================= */
struct Usuario {
    string username;
    string password;
    Rol rol;
};

struct Fecha {
    int d, m, a;
};

struct Producto {
    string nombre;
    float stock;
    float precio;
    TipoUnidad unidad;
    int stockMinimo;
};

struct Movimiento {
    string producto;
    string tipo;
    float cantidad;
    float total;
    Fecha fecha;
};

/* ================= VARIABLES GLOBALES ================= */
vector<Usuario> usuarios;
vector<Producto> inventario;
vector<Movimiento> historial;
Usuario usuarioActual;

/* ================= UTILIDADES ================= */
string strUnidad(TipoUnidad u) {
    return (u == KG) ? "kg" : "unid";
}

/* ================= PASSWORD ================= */
string leerPassword() {
    string pass;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { cout << endl; break; } // Enter
        if (ch == 8) { // Backspace
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass += ch;
            cout << "*";
        }
    }
    return pass;
}

/* ================= CODIFICAR ================= */
string codificar(string p) {
    for (char &c : p) c += 3;
    return p;
}

string decodificar(string p) {
    for (char &c : p) c -= 3;
    return p;
}

/* ================= ARCHIVOS (CORREGIDO) ================= */
void guardarUsuarios() {
    ofstream f("usuarios.txt");
    for (auto &u : usuarios) {
        f << u.username << " "
          << codificar(u.password) << " "
          << (u.rol == ADMIN ? "ADMIN" : "EMPLEADO") << endl;
    }
    f.close();
}

void cargarUsuarios() {
    ifstream f("usuarios.txt");
    if (!f.is_open()) return;
    usuarios.clear();
    Usuario u;
    string rolStr;
    while (f >> u.username >> u.password >> rolStr) {
        u.password = decodificar(u.password);
        u.rol = (rolStr == "ADMIN") ? ADMIN : EMPLEADO;
        usuarios.push_back(u);
    }
    f.close();
}

void guardarInventario() {
    ofstream f("productos.txt");
    for (auto &p : inventario) {
        // Guardamos la unidad como entero (0 o 1)
        f << p.nombre << " " << p.stock << " " << p.precio << " "
          << (int)p.unidad << " " << p.stockMinimo << endl;
    }
    f.close();
}

// ESTA FUNCIÓN FALTABA Y ES CRITICA
void cargarInventario() {
    ifstream f("productos.txt");
    if (!f.is_open()) return;
    inventario.clear();
    Producto p;
    int unidadTemp;
    while (f >> p.nombre >> p.stock >> p.precio >> unidadTemp >> p.stockMinimo) {
        p.unidad = (TipoUnidad)unidadTemp;
        inventario.push_back(p);
    }
    f.close();
}

void guardarHistorial(Movimiento m) {
    ofstream f("historial.txt", ios::app);
    f << m.producto << " " << m.tipo << " "
      << m.cantidad << " " << m.total << " "
      << m.fecha.d << " " << m.fecha.m << " " << m.fecha.a << endl;
    f.close();
}

/* ================= LOGIN ================= */
bool login() {
    int intentos = 3;
    while (intentos--) {
        titulo("INICIO DE SESION");
        string u, p;
        cout << "Usuario: ";
        cin >> u;
        cout << "Clave: ";
        p = leerPassword();

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

/* ================= PRODUCTOS ================= */
int buscarProducto(string n) {
    for (int i = 0; i < inventario.size(); i++)
        if (inventario[i].nombre == n) return i;
    return -1;
}

void registrarProducto() {
    titulo("NUEVO PRODUCTO");
    Producto p;
    cout << "Nombre (sin espacios): ";
    cin >> p.nombre;

    if (buscarProducto(p.nombre) != -1) {
        mensajeError("El producto ya existe.");
        return;
    }

    int t;
    cout << "Unidad (1-Unidad, 2-Kg): ";
    cin >> t;
    p.unidad = (t == 2) ? KG : UNIDAD;

    cout << "Stock inicial: "; cin >> p.stock;
    cout << "Precio Venta: S/. "; cin >> p.precio;
    p.stockMinimo = 5; // Valor por defecto

    inventario.push_back(p);
    guardarInventario();
    mensajeExito("Producto registrado correctamente");
}

void registrarMovimiento(bool entrada) {
    titulo(entrada ? "ENTRADA DE STOCK" : "VENTA / SALIDA");
    string n;
    cout << "Nombre del Producto: ";
    cin >> n;

    int i = buscarProducto(n);
    if (i == -1) {
        mensajeError("Producto no encontrado.");
        return;
    }

    // Mostrar info actual
    color(COLOR_AMARILLO);
    cout << "Stock actual: " << inventario[i].stock << " " << strUnidad(inventario[i].unidad) << endl;
    color(COLOR_DEFECTO);

    float cant;
    cout << "Cantidad a " << (entrada ? "ingresar: " : "retirar: ");
    cin >> cant;

    if (!entrada && cant > inventario[i].stock) {
        mensajeError("Stock insuficiente para realizar la venta.");
        return;
    }

    // Actualizar stock
    inventario[i].stock += entrada ? cant : -cant;

    Movimiento m;
    m.producto = n;
    m.tipo = entrada ? "ENTRADA" : "SALIDA";
    m.cantidad = cant;
    m.total = cant * inventario[i].precio;

    // Fecha automática simple o manual
    cout << "Fecha (dia mes anio): ";
    cin >> m.fecha.d >> m.fecha.m >> m.fecha.a;

    historial.push_back(m);
    guardarInventario();
    guardarHistorial(m);

    if (!entrada) {
        cout << "\n";
        color(COLOR_BLANCO_BRILLANTE);
        cout << "========= TICKET DE VENTA =========\n";
        cout << "Producto: " << n << endl;
        cout << "Cantidad: " << cant << " " << strUnidad(inventario[i].unidad) << endl;
        cout << "Total:    S/. " << fixed << setprecision(2) << m.total << endl;
        cout << "===================================\n";
        system("pause");
    } else {
        mensajeExito("Stock actualizado.");
    }
}

/* ================= GESTIÓN ADMIN ================= */
void eliminarProducto() {
    titulo("ELIMINAR PRODUCTO");
    string n;
    cout << "Nombre: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { mensajeError("No existe"); return; }
    
    inventario.erase(inventario.begin() + i);
    guardarInventario();
    mensajeExito("Producto eliminado permanentemente");
}

void menuEntradaAdmin() {
    int op;
    do {
        titulo("PANEL ADMINISTRADOR");
        cout << "1. Agregar Stock (Compra)" << endl;
        cout << "2. Eliminar Producto" << endl;
        cout << "3. Modificar Precio/Stock Manual" << endl;
        cout << "4. Configurar Stock Minimo" << endl;
        cout << "5. Volver" << endl;
        cout << ">> ";
        cin >> op;

        string n; int i;

        switch (op) {
            case 1: registrarMovimiento(true); break;
            case 2: eliminarProducto(); break;
            case 3: 
                cout << "Producto: "; cin >> n;
                i = buscarProducto(n);
                if(i!=-1) {
                    cout << "Nuevo Precio: "; cin >> inventario[i].precio;
                    cout << "Nuevo Stock: "; cin >> inventario[i].stock;
                    guardarInventario();
                    mensajeExito("Datos actualizados");
                } else mensajeError("No encontrado");
                break;
            case 4:
                cout << "Producto: "; cin >> n;
                i = buscarProducto(n);
                if(i!=-1) {
                    cout << "Nuevo Stock Minimo: "; cin >> inventario[i].stockMinimo;
                    guardarInventario();
                    mensajeExito("Alerta configurada");
                } else mensajeError("No encontrado");
                break;
        }
    } while (op != 5);
}

/* ================= REPORTES ================= */
void verInventario() {
    titulo("INVENTARIO GENERAL");
    
    // Encabezado de tabla
    color(COLOR_BLANCO_BRILLANTE);
    cout << left << setw(15) << "NOMBRE" 
         << setw(10) << "STOCK" 
         << setw(10) << "UNIDAD" 
         << setw(12) << "PRECIO" 
         << setw(15) << "ESTADO" << endl;
    cout << string(62, '-') << endl;
    color(COLOR_DEFECTO);

    for (auto &p : inventario) {
        cout << left << setw(15) << p.nombre 
             << setw(10) << p.stock 
             << setw(10) << strUnidad(p.unidad) 
             << "S/." << setw(9) << fixed << setprecision(2) << p.precio;
        
        if (p.stock <= p.stockMinimo) {
            color(COLOR_ROJO);
            cout << " [STOCK BAJO]";
            color(COLOR_DEFECTO);
        } else {
            color(COLOR_VERDE);
            cout << " [OK]";
            color(COLOR_DEFECTO);
        }
        cout << endl;
    }
    cout << "\n";
    system("pause");
}

void verHistorial() {
    titulo("HISTORIAL DE MOVIMIENTOS");
    ifstream f("historial.txt");
    if (!f.is_open()) { mensajeError("No hay historial aun."); return; }

    string prod, tipo;
    float cant, tot;
    int d, m, a;

    cout << left << setw(12) << "FECHA" << setw(15) << "PRODUCTO" << setw(10) << "TIPO" << setw(10) << "CANT" << "TOTAL" << endl;
    cout << string(60, '-') << endl;

    while (f >> prod >> tipo >> cant >> tot >> d >> m >> a) {
        color(tipo == "ENTRADA" ? COLOR_VERDE : COLOR_ROJO);
        cout << d << "/" << m << "/" << a << "   "
             << left << setw(15) << prod 
             << setw(10) << tipo 
             << setw(10) << cant 
             << "S/." << tot << endl;
    }
    f.close();
    color(COLOR_DEFECTO);
    system("pause");
}

void reporteMensual() {
    titulo("REPORTE MENSUAL");
    int mes, anio;
    float totalVentas = 0;
    
    cout << "Ingrese Mes (1-12): "; cin >> mes;
    cout << "Ingrese Anio: "; cin >> anio;

    ifstream f("historial.txt");
    string prod, tipo;
    float cant, tot;
    int d, m, a;
    bool encontrado = false;

    cout << "\n--- Ventas del " << mes << "/" << anio << " ---\n";

    while (f >> prod >> tipo >> cant >> tot >> d >> m >> a) {
        if (m == mes && a == anio && tipo == "SALIDA") {
            cout << prod << " -> S/." << tot << endl;
            totalVentas += tot;
            encontrado = true;
        }
    }
    f.close();

    cout << string(30, '-') << endl;
    if (encontrado) {
        color(COLOR_AMARILLO);
        cout << "TOTAL VENDIDO: S/. " << fixed << setprecision(2) << totalVentas << endl;
    } else {
        cout << "No se encontraron ventas en esa fecha." << endl;
    }
    color(COLOR_DEFECTO);
    system("pause");
}

/* ================= MENU PRINCIPAL ================= */
void menu() {
    int op;
    do {
        titulo("SISTEMA DE ALMACEN v2.0");
        cout << "Usuario: " << usuarioActual.username << endl;
        if(usuarioActual.rol == ADMIN) { color(COLOR_ROJO); cout << "[ADMINISTRADOR]"; }
        else { color(COLOR_VERDE); cout << "[EMPLEADO]"; }
        color(COLOR_DEFECTO);
        
        cout << "\n\n1. Registrar Nuevo Producto" << endl;
        cout << "2. Gestion de Stock (Entradas/Admin)" << endl;
        cout << "3. Realizar Venta (Salida)" << endl;
        cout << "4. Ver Inventario" << endl;
        cout << "5. Ver Historial Completo" << endl;
        cout << "6. Reporte de Ventas Mensual" << endl;
        cout << "7. Cerrar Sesion" << endl;
        cout << "\nOpcion: ";
        cin >> op;

        switch (op) {
            case 1: registrarProducto(); break;
            case 2:
                if (usuarioActual.rol == ADMIN) menuEntradaAdmin();
                else registrarMovimiento(true);
                break;
            case 3: registrarMovimiento(false); break;
            case 4: verInventario(); break;
            case 5: verHistorial(); break;
            case 6: reporteMensual(); break;
            case 7: cout << "Cerrando sistema..." << endl; break;
            default: mensajeError("Opcion invalida");
        }
    } while (op != 7);
}

/* ================= MAIN ================= */
int main() {
    SetConsoleOutputCP(CP_UTF8); // Para tildes y ñ
    system("title SISTEMA DE ALMACEN"); // Título de la ventana
    
    // Carga inicial
    cargarUsuarios();
    cargarInventario(); // ESTO FALTABA EN TU CÓDIGO ORIGINAL

    // Crear admin por defecto si no existe archivo
    if (usuarios.empty()) {
        usuarios.push_back({"admin", "admin", ADMIN});
        usuarios.push_back({"vendedor", "123", EMPLEADO});
        guardarUsuarios();
    }

    if (login()) {
        menu();
    }

    return 0;
}       