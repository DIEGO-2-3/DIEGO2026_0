#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <conio.h>
#include <windows.h>

using namespace std;

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
    int stockMinimo = 5;
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
void limpiar() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string strUnidad(TipoUnidad u) {
    return (u == KG) ? "kg" : "unid";
}

/* ================= PASSWORD ================= */
string leerPassword() {
    string pass;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { cout << endl; break; }
        if (ch == 8 && !pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        } else if (ch != 8) {
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

/* ================= ARCHIVOS ================= */
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
    string rol;
    while (f >> u.username >> u.password >> rol) {
        u.password = decodificar(u.password);
        u.rol = (rol == "ADMIN") ? ADMIN : EMPLEADO;
        usuarios.push_back(u);
    }
    f.close();
}

void guardarInventario() {
    ofstream f("productos.txt");
    for (auto &p : inventario) {
        f << p.nombre << " " << p.stock << " " << p.precio << " "
          << p.unidad << " " << p.stockMinimo << endl;
    }
    f.close();
}

void guardarHistorial() {
    ofstream f("historial.txt", ios::app);
    for (auto &m : historial) {
        f << m.producto << " " << m.tipo << " "
          << m.cantidad << " " << m.total << " "
          << m.fecha.d << "/" << m.fecha.m << "/" << m.fecha.a << endl;
    }
    f.close();
    historial.clear();
}

/* ================= LOGIN ================= */
bool login() {
    int intentos = 3;
    while (intentos--) {
        string u, p;
        cout << "\n===== LOGIN =====\nUsuario: ";
        cin >> u;
        cout << "Clave: ";
        p = leerPassword();

        for (auto &x : usuarios) {
            if (x.username == u && x.password == p) {
                usuarioActual = x;
                return true;
            }
        }
        cout << "Credenciales incorrectas\n";
        cout << "Intentos restantes: " << intentos << endl;
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
    Producto p;
    cout << "Nombre: ";
    cin >> p.nombre;

    if (buscarProducto(p.nombre) != -1) {
        cout << "Producto ya existe\n";
        return;
    }

    int t;
    cout << "Unidad (1-Unidad, 2-Kg): ";
    cin >> t;
    p.unidad = (t == 2) ? KG : UNIDAD;

    cout << "Stock inicial: ";
    cin >> p.stock;
    cout << "Precio: ";
    cin >> p.precio;

    inventario.push_back(p);
    guardarInventario();
    cout << "Producto registrado\n";
}

void registrarMovimiento(bool entrada) {
    string n;
    cout << "Producto: ";
    cin >> n;

    int i = buscarProducto(n);
    if (i == -1) {
        cout << "No existe\n";
        return;
    }

    float cant;
    cout << "Cantidad: ";
    cin >> cant;

    if (!entrada && cant > inventario[i].stock) {
        cout << "Stock insuficiente\n";
        return;
    }

    inventario[i].stock += entrada ? cant : -cant;

    Movimiento m;
    m.producto = n;
    m.tipo = entrada ? "ENTRADA" : "SALIDA";
    m.cantidad = cant;
    m.total = cant * inventario[i].precio;

    cout << "Fecha (d m a): ";
    cin >> m.fecha.d >> m.fecha.m >> m.fecha.a;

    historial.push_back(m);
    guardarInventario();
    guardarHistorial();

    if (!entrada) {
        cout << "=== VOUCHER ===\n";
        cout << n << " " << cant << " " << strUnidad(inventario[i].unidad)
             << " Total S/. " << fixed << setprecision(2) << m.total << endl;
    }
}

/* ================= SUBMENU ENTRADA ================= */
void eliminarProducto() {
    string n;
    cout << "Producto a eliminar: ";
    cin >> n;
    int i = buscarProducto(n);
    if (i == -1) return;
    inventario.erase(inventario.begin() + i);
    guardarInventario();
    cout << "Producto eliminado\n";
}

void actualizarProducto() {
    string n;
    cout << "Producto: ";
    cin >> n;
    int i = buscarProducto(n);
    if (i == -1) return;
    cout << "Nuevo precio: ";
    cin >> inventario[i].precio;
    cout << "Nuevo stock: ";
    cin >> inventario[i].stock;
    guardarInventario();
}

void limitarProducto() {
    string n;
    cout << "Producto: ";
    cin >> n;
    int i = buscarProducto(n);
    if (i == -1) return;
    cout << "Stock minimo: ";
    cin >> inventario[i].stockMinimo;
    guardarInventario();
}

void menuEntradaAdmin() {
    int op;
    do {
        cout << "\n=== ENTRADA STOCK (ADMIN) ===\n";
        cout << "1. Agregar Stock\n";
        cout << "2. Eliminar Producto\n";
        cout << "3. Actualizar Producto\n";
        cout << "4. Limitar Producto\n";
        cout << "5. Volver\nOpcion: ";
        cin >> op;

        switch (op) {
            case 1: registrarMovimiento(true); break;
            case 2: eliminarProducto(); break;
            case 3: actualizarProducto(); break;
            case 4: limitarProducto(); break;
        }
    } while (op != 5);
}

/* ================= REPORTES ================= */
void verInventario() {
    for (auto &p : inventario) {
        cout << p.nombre << " | " << p.stock << " "
             << strUnidad(p.unidad)
             << " | S/." << p.precio;
        if (p.stock < p.stockMinimo)
            cout << " [STOCK BAJO]";
        cout << endl;
    }
}

void reporteMensual() {
    int m, a;
    float total = 0;
    cout << "Mes y Año: ";
    cin >> m >> a;

    ifstream f("historial.txt");
    string prod, tipo, fecha;
    float cant, tot;

    while (f >> prod >> tipo >> cant >> tot >> fecha) {
        if (stoi(fecha.substr(3,2)) == m && stoi(fecha.substr(6)) == a) {
            if (tipo == "SALIDA") total += tot;
        }
    }
    f.close();
    cout << "Total ventas: S/. " << total << endl;
}

/* ================= MENU PRINCIPAL ================= */
void menu() {
    int op;
    do {
        cout << "\nCONTROL DE ALMACEN\n";
        cout << "1. Registrar Producto\n";
        cout << "2. Entrada Stock\n";
        cout << "3. Salida Stock\n";
        cout << "4. Ver Inventario\n";
        cout << "5. Ver Historial\n";
        cout << "6. Reporte Mensual\n";
        cout << "7. Cerrar Sesion\nOpcion: ";
        cin >> op;

        switch (op) {
            case 1: registrarProducto(); break;
            case 2:
                if (usuarioActual.rol == ADMIN)
                    menuEntradaAdmin();
                else
                    registrarMovimiento(true);
                break;
            case 3: registrarMovimiento(false); break;
            case 4: verInventario(); break;
            case 6: reporteMensual(); break;
        }
    } while (op != 7);
}

/* ================= MAIN ================= */
int main() {
    SetConsoleOutputCP(CP_UTF8);
    cargarUsuarios();

    if (usuarios.empty()) {
        usuarios.push_back({"admin", "1234", ADMIN});
        usuarios.push_back({"empleado", "1234", EMPLEADO});
        guardarUsuarios();
    }

    if (login())
        menu();

    return 0;
}