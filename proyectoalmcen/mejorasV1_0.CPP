    #include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <windows.h>

using namespace std;

// ================== UTILIDADES VISUALES ==================
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ================== ENUMS Y ESTRUCTURAS ==================
enum TipoUnidad { UNIDAD, KG };
enum Rol { ADMIN, EMPLEADO };

struct Fecha {
    int d, m, a;
};

struct Producto {
    string nombre;
    float stock;
    float precio;
    TipoUnidad unidad;
};

struct Movimiento {
    string producto;
    string tipo;
    float cantidad;
    float total;
    Fecha fecha;
};

struct Usuario {
    string user;
    string pass;
    Rol rol;
};

// ================== BASE DE DATOS ==================
vector<Usuario> usuarios;
vector<Producto> inventario;
vector<Movimiento> historial;

// ================== FUNCIONES ==================
string strUnidad(TipoUnidad u) {
    return (u == KG) ? "kg" : "unid";
}

Fecha pedirFecha() {
    Fecha f;
    cout << "Fecha (DD MM AAAA): ";
    cin >> f.d >> f.m >> f.a;
    return f;
}

int buscarProducto(string nombre) {
    for (size_t i = 0; i < inventario.size(); i++)
        if (inventario[i].nombre == nombre) return i;
    return -1;
}

// ================== INVENTARIO ==================
void registrarProducto() {
    Producto p;
    cout << "Nombre: "; cin >> p.nombre;
    if (buscarProducto(p.nombre) != -1) {
        color(12); cout << "Producto ya existe\n"; color(7); return;
    }
    int t;
    cout << "Unidad (1=Unid, 2=Kg): "; cin >> t;
    p.unidad = (t == 2) ? KG : UNIDAD;
    cout << "Stock inicial: "; cin >> p.stock;
    cout << "Precio unitario: "; cin >> p.precio;
    inventario.push_back(p);
    color(10); cout << "Producto registrado\n"; color(7);
}

void entradaSalida(bool entrada) {
    string nombre;
    cout << "Producto: "; cin >> nombre;
    int i = buscarProducto(nombre);
    if (i == -1) {
        color(12); cout << "No encontrado\n"; color(7); return;
    }
    float cant;
    cout << "Cantidad: "; cin >> cant;
    if (!entrada && cant > inventario[i].stock) {
        color(12); cout << "Stock insuficiente\n"; color(7); return;
    }
    inventario[i].stock += entrada ? cant : -cant;

    Movimiento m;
    m.producto = nombre;
    m.tipo = entrada ? "ENTRADA" : "SALIDA";
    m.cantidad = cant;
    m.total = cant * inventario[i].precio;
    m.fecha = pedirFecha();
    historial.push_back(m);

    if (!entrada) {
        color(11);
        cout << "\n--- VOUCHER ---\n";
        cout << "Producto: " << m.producto << endl;
        cout << "Cantidad: " << m.cantidad << endl;
        cout << "Total: S/. " << fixed << setprecision(2) << m.total << endl;
        color(7);
    }
}

// ================== REPORTES ==================
void mostrarInventario() {
    cout << "\nPRODUCTO     STOCK   PRECIO\n";
    for (auto &p : inventario) {
        cout << left << setw(12) << p.nombre
             << setw(8) << p.stock
             << "S/. " << p.precio << endl;
        if (p.stock < 5) {
            color(14); cout << "  ALERTA STOCK BAJO\n"; color(7);
        }
    }
}

void ordenarInventario() {
    int op;
    cout << "1.Nombre 2.Stock 3.Precio: ";
    cin >> op;
    if (op == 1)
        sort(inventario.begin(), inventario.end(),
             [](Producto a, Producto b){ return a.nombre < b.nombre; });
    if (op == 2)
        sort(inventario.begin(), inventario.end(),
             [](Producto a, Producto b){ return a.stock < b.stock; });
    if (op == 3)
        sort(inventario.begin(), inventario.end(),
             [](Producto a, Producto b){ return a.precio < b.precio; });
    color(10); cout << "Inventario ordenado\n"; color(7);
}

void kardexProducto() {
    string nombre;
    cout << "Producto: "; cin >> nombre;
    for (auto &m : historial)
        if (m.producto == nombre)
            cout << m.fecha.d << "/" << m.fecha.m << " "
                 << m.tipo << " " << m.cantidad << endl;
}

// ================== LOGIN ==================
Usuario* login() {
    string u, p;
    cout << "Usuario: "; cin >> u;
    cout << "Clave: "; cin >> p;
    for (auto &x : usuarios)
        if (x.user == u && x.pass == p) return &x;
    return nullptr;
}

// ================== MENU ==================
void menu(Usuario *u) {
    int op;
    do {
        cout << "\n1.Inventario\n2.Entrada\n3.Salida\n4.Ordenar\n5.Kardex\n";
        if (u->rol == ADMIN)
            cout << "6.Registrar Producto\n";
        cout << "0.Salir\nOpcion: ";
        cin >> op;

        if (op == 1) mostrarInventario();
        if (op == 2) entradaSalida(true);
        if (op == 3) entradaSalida(false);
        if (op == 4) ordenarInventario();
        if (op == 5) kardexProducto();
        if (op == 6 && u->rol == ADMIN) registrarProducto();
    } while (op != 0);
}

// ================== MAIN ==================
int main() {
    SetConsoleOutputCP(CP_UTF8);
    usuarios.push_back({"admin","1234",ADMIN});
    usuarios.push_back({"empleado","123",EMPLEADO});

    Usuario *u = login();
    if (u) menu(u);
    else cout << "Login incorrecto\n";
    return 0;
}