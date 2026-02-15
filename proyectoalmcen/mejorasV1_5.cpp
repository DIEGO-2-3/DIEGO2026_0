#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <windows.h>

using namespace std;

/* =======================
   CONFIGURACIÓN CONSOLA
   ======================= */
void color(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void limpiar(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/* =======================
   ESTRUCTURAS Y ENUMS
   ======================= */
enum Rol { ADMIN, EMPLEADO };

struct Fecha{
    int d, m, a;
};

struct Producto{
    string nombre;
    float stock;
    float precio;
};

struct Usuario{
    string user;
    string pass;
    Rol rol;
};

struct Movimiento{
    int voucher;
    string producto;
    string tipo;
    float cantidad;
    float total;
    string usuario;
    Fecha fecha;
};

/* =======================
   VARIABLES GLOBALES
   ======================= */
vector<Producto> inventario;
vector<Usuario> usuarios;
vector<Movimiento> historial;
int contadorVoucher = 1;

/* =======================
   UTILIDADES
   ======================= */
float leerFloatSeguro(string msg){
    float x;
    while(true){
        cout << msg;
        if(cin >> x && x >= 0) return x;
        cin.clear(); limpiar();
        color(12); cout << "Valor invalido\n"; color(7);
    }
}

Fecha pedirFecha(){
    Fecha f;
    cout << "Fecha (DD MM AAAA): ";
    cin >> f.d >> f.m >> f.a;
    return f;
}

string leerPassword(){
    string p;
    cin >> p;
    return p;
}

/* =======================
   BUSQUEDAS
   ======================= */
int buscarProducto(string n){
    for(int i=0;i<inventario.size();i++)
        if(inventario[i].nombre == n) return i;
    return -1;
}

/* =======================
   REGISTRO DE PRODUCTO
   ======================= */
void registrarProducto(){
    Producto p;
    cout << "Nombre del producto: ";
    cin >> p.nombre;

    if(buscarProducto(p.nombre) != -1){
        color(12); cout << "Producto ya existe\n"; color(7);
        return;
    }

    p.stock = leerFloatSeguro("Stock inicial: ");
    p.precio = leerFloatSeguro("Precio unitario: ");

    inventario.push_back(p);
    color(10); cout << "Producto registrado\n"; color(7);
}

/* =======================
   REGISTRO DE EMPLEADO
   ======================= */
void registrarEmpleado(){
    Usuario u;
    cout << "Nuevo usuario: ";
    cin >> u.user;

    for(auto &x : usuarios){
        if(x.user == u.user){
            color(12); cout << "Usuario ya existe\n"; color(7);
            return;
        }
    }

    cout << "Contraseña: ";
    u.pass = leerPassword();
    u.rol = EMPLEADO;

    usuarios.push_back(u);
    color(10); cout << "Empleado registrado\n"; color(7);
}

/* =======================
   VOUCHER
   ======================= */
void imprimirVoucher(Movimiento &m){
    color(11);
    cout << "\n====================================\n";
    cout << "        COMPROBANTE DE VENTA\n";
    cout << "====================================\n";
    cout << "Voucher N°: " << setw(6) << setfill('0') << m.voucher << endl;
    cout << "Fecha: " << m.fecha.d << "/" << m.fecha.m << "/" << m.fecha.a << endl;
    cout << "Vendedor: " << m.usuario << endl;
    cout << "------------------------------------\n";
    cout << "Producto: " << m.producto << endl;
    cout << "Cantidad: " << m.cantidad << endl;
    cout << "TOTAL: S/. " << fixed << setprecision(2) << m.total << endl;
    cout << "====================================\n";
    cout << "   Gracias por su compra\n";
    cout << "====================================\n";
    color(7);
}

/* =======================
   ENTRADA / SALIDA
   ======================= */
void entradaSalida(bool entrada, Usuario* u){
    string n;
    cout << "Producto: ";
    cin >> n;

    int i = buscarProducto(n);
    if(i == -1){
        color(12); cout << "Producto no encontrado\n"; color(7);
        return;
    }

    float c = leerFloatSeguro("Cantidad: ");
    if(!entrada && c > inventario[i].stock){
        color(12); cout << "Stock insuficiente\n"; color(7);
        return;
    }

    inventario[i].stock += entrada ? c : -c;

    Movimiento m;
    m.producto = n;
    m.tipo = entrada ? "ENTRADA" : "SALIDA";
    m.cantidad = c;
    m.total = c * inventario[i].precio;
    m.usuario = u->user;
    m.fecha = pedirFecha();

    if(!entrada){
        m.voucher = contadorVoucher++;
        imprimirVoucher(m);
    }else{
        m.voucher = 0;
    }

    historial.push_back(m);
}

/* =======================
   INVENTARIO
   ======================= */
void verInventario(){
    cout << "\n----- INVENTARIO -----\n";
    for(auto &p : inventario){
        cout << p.nombre << " | Stock: " << p.stock
             << " | Precio: S/." << p.precio << endl;
    }
}

/* =======================
   HISTORIAL
   ======================= */
void verHistorial(){
    cout << "\n----- HISTORIAL -----\n";
    for(auto &m : historial){
        cout << m.tipo << " | " << m.producto
             << " | Cant: " << m.cantidad
             << " | Total: S/." << m.total
             << " | Usuario: " << m.usuario << endl;
    }
}

/* =======================
   REPORTE MENSUAL
   ======================= */
void reporteMensual(){
    int mes, anio;
    cout << "Mes: "; cin >> mes;
    cout << "Año: "; cin >> anio;

    float total = 0;
    cout << "\n--- REPORTE " << mes << "/" << anio << " ---\n";
    for(auto &m : historial){
        if(m.fecha.m == mes && m.fecha.a == anio && m.tipo == "SALIDA"){
            cout << m.producto << " S/." << m.total << endl;
            total += m.total;
        }
    }
    cout << "TOTAL VENTAS: S/." << total << endl;
}

/* =======================
   MENU
   ======================= */
void menu(Usuario* u){
    int op;
    do{
        cout << "\n1 Inventario\n2 Entrada\n3 Salida\n4 Historial\n";
        if(u->rol == ADMIN)
            cout << "5 Registrar Producto\n6 Registrar Empleado\n7 Reporte Mensual\n";
        cout << "0 Salir\nOpcion: ";
        cin >> op;

        if(op == 1) verInventario();
        if(op == 2) entradaSalida(true, u);
        if(op == 3) entradaSalida(false, u);
        if(op == 4) verHistorial();
        if(op == 5 && u->rol == ADMIN) registrarProducto();
        if(op == 6 && u->rol == ADMIN) registrarEmpleado();
        if(op == 7 && u->rol == ADMIN) reporteMensual();

    }while(op != 0);
}

/* =======================
   LOGIN
   ======================= */
Usuario* login(){
    string u,p;
    cout << "Usuario: "; cin >> u;
    cout << "Clave: "; cin >> p;

    for(auto &x : usuarios){
        if(x.user == u && x.pass == p)
            return &x;
    }
    return nullptr;
}

/* =======================
   MAIN
   ======================= */
int main(){
    SetConsoleOutputCP(CP_UTF8);

    // ADMIN POR DEFECTO
    usuarios.push_back({"admin","1234",ADMIN});

    Usuario* u = nullptr;
    do{
        cout << "\n=== LOGIN ===\n";
        u = login();
        if(!u) color(12), cout << "Credenciales incorrectas\n", color(7);
    }while(!u);

    menu(u);
    return 0;
}