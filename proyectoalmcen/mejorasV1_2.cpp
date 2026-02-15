#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <conio.h>

using namespace std;

// ================= VISUAL =================
void color(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// ================= ENUMS =================
enum TipoUnidad { UNIDAD, KG };
enum Rol { ADMIN, EMPLEADO };

// ================= STRUCTS =================
struct Fecha{ int d,m,a; };

struct Producto{
    string nombre;
    float stock;
    float precio;
    TipoUnidad unidad;
};

struct Movimiento{
    string producto;
    string tipo;
    float cantidad;
    float total;
    Fecha fecha;
};

struct Usuario{
    string user;
    string pass;
    Rol rol;
};

// ================= BASE =================
vector<Usuario> usuarios;
vector<Producto> inventario;
vector<Movimiento> historial;

// ================= VALIDACIONES =================
float leerFloatSeguro(string msg){
    float v;
    while(true){
        cout << msg;
        if(cin >> v && v >= 0) return v;
        color(12); cout << "Error: dato invalido\n"; color(7);
        cin.clear();
        cin.ignore(1000,'\n');
    }
}

// ================= PASSWORD =================
string leerPassword(){
    string pass;
    char c;
    while((c = _getch()) != 13){
        if(c == 8 && !pass.empty()){
            pass.pop_back();
            cout << "\b \b";
        } else if(c != 8){
            pass.push_back(c);
            cout << "*";
        }
    }
    cout << endl;
    return pass;
}

// ================= ARCHIVOS =================
void cargarUsuarios(){
    ifstream f("usuarios.txt");
    Usuario u; int r;
    while(f >> u.user >> u.pass >> r){
        u.rol = (Rol)r;
        usuarios.push_back(u);
    }
}

void guardarUsuarios(){
    ofstream f("usuarios.txt");
    for(auto &u:usuarios)
        f << u.user << " " << u.pass << " " << u.rol << endl;
}

// ================= UTIL =================
int buscarProducto(string n){
    for(int i=0;i<inventario.size();i++)
        if(inventario[i].nombre == n) return i;
    return -1;
}

Fecha pedirFecha(){
    Fecha f;
    cout << "Fecha (DD MM AAAA): ";
    cin >> f.d >> f.m >> f.a;
    return f;
}

// ================= FUNCIONES =================
void registrarProducto(){
    Producto p;
    cout << "Nombre: "; cin >> p.nombre;
    if(buscarProducto(p.nombre) != -1){
        color(12); cout << "Producto ya existe\n"; color(7); return;
    }
    int u;
    cout << "Unidad (0=UNID 1=KG): "; cin >> u;
    p.unidad = (TipoUnidad)u;
    p.stock = leerFloatSeguro("Stock inicial: ");
    p.precio = leerFloatSeguro("Precio: ");
    inventario.push_back(p);
    color(10); cout << "Producto registrado\n"; color(7);
}

void entradaSalida(bool entrada){
    string n;
    cout << "Producto: "; cin >> n;
    int i = buscarProducto(n);
    if(i == -1){
        color(12); cout << "No existe\n"; color(7); return;
    }
    float c = leerFloatSeguro("Cantidad: ");
    if(!entrada && c > inventario[i].stock){
        color(12); cout << "Stock insuficiente\n"; color(7); return;
    }
    inventario[i].stock += entrada ? c : -c;

    Movimiento m;
    m.producto = n;
    m.tipo = entrada ? "ENTRADA" : "SALIDA";
    m.cantidad = c;
    m.total = c * inventario[i].precio;
    m.fecha = pedirFecha();
    historial.push_back(m);

    if(!entrada){
        color(11);
        cout << "VOUCHER\nTOTAL: S/. " << fixed << setprecision(2) << m.total << endl;
        color(7);
    }
}

// ================= LOGIN =================
Usuario* login(){
    string u;
    cout << "Usuario: "; cin >> u;
    cout << "Clave: ";
    string p = leerPassword();

    for(auto &x:usuarios)
        if(x.user == u && x.pass == p) return &x;
    return nullptr;
}

// ================= MENU =================
void menu(Usuario* u){
    int op;
    do{
        cout << "\n1 Inventario\n2 Entrada\n3 Salida\n4 Kardex\n";
        if(u->rol == ADMIN) cout << "5 Nuevo Producto\n";
        cout << "0 Salir\nOpcion: ";
        cin >> op;

        if(op == 1)
            for(auto &p:inventario)
                cout << p.nombre << " " << p.stock << " S/." << p.precio << endl;
        if(op == 2) entradaSalida(true);
        if(op == 3) entradaSalida(false);
        if(op == 4){
            string n; cout << "Producto: "; cin >> n;
            for(auto &m:historial)
                if(m.producto == n)
                    cout << m.tipo << " " << m.cantidad << endl;
        }
        if(op == 5 && u->rol == ADMIN) registrarProducto();
    }while(op != 0);
}

// ================= MAIN =================
int main(){
    SetConsoleOutputCP(CP_UTF8);

    cargarUsuarios();
    if(usuarios.empty()){
        usuarios.push_back({"admin","1234",ADMIN});
        guardarUsuarios();
    }

    Usuario* u = login();
    if(u) menu(u);
    else color(12), cout << "Login incorrecto\n";

    return 0;
}