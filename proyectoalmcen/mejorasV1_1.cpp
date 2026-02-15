#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <windows.h>

using namespace std;

// ================= VISUAL =================
void color(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// ================= ENUMS =================
enum TipoUnidad { UNIDAD, KG };
enum Rol { ADMIN, EMPLEADO };

// ================= STRUCTS =================
struct Fecha{
    int d,m,a;
};

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

// ================= ARCHIVOS =================
void cargarUsuarios(){
    ifstream f("usuarios.txt");
    Usuario u;
    int r;
    while(f >> u.user >> u.pass >> r){
        u.rol = (Rol)r;
        usuarios.push_back(u);
    }
    f.close();
}

void guardarUsuarios(){
    ofstream f("usuarios.txt");
    for(auto &u:usuarios)
        f << u.user << " " << u.pass << " " << u.rol << endl;
    f.close();
}

void cargarProductos(){
    ifstream f("productos.txt");
    Producto p;
    int u;
    while(f >> p.nombre >> p.stock >> p.precio >> u){
        p.unidad = (TipoUnidad)u;
        inventario.push_back(p);
    }
    f.close();
}

void guardarProductos(){
    ofstream f("productos.txt");
    for(auto &p:inventario)
        f << p.nombre << " " << p.stock << " "
          << p.precio << " " << p.unidad << endl;
    f.close();
}

void cargarHistorial(){
    ifstream f("historial.txt");
    Movimiento m;
    while(f >> m.producto >> m.tipo >> m.cantidad >> m.total
           >> m.fecha.d >> m.fecha.m >> m.fecha.a){
        historial.push_back(m);
    }
    f.close();
}

void guardarHistorial(){
    ofstream f("historial.txt");
    for(auto &m:historial)
        f << m.producto << " " << m.tipo << " "
          << m.cantidad << " " << m.total << " "
          << m.fecha.d << " " << m.fecha.m << " "
          << m.fecha.a << endl;
    f.close();
}

// ================= UTIL =================
Fecha pedirFecha(){
    Fecha f;
    cout<<"Fecha (DD MM AAAA): ";
    cin>>f.d>>f.m>>f.a;
    return f;
}

int buscarProducto(string n){
    for(int i=0;i<inventario.size();i++)
        if(inventario[i].nombre==n) return i;
    return -1;
}

// ================= FUNCIONES =================
void registrarProducto(){
    Producto p;
    cout<<"Nombre: "; cin>>p.nombre;
    if(buscarProducto(p.nombre)!=-1){
        color(12); cout<<"Ya existe\n"; color(7); return;
    }
    int u;
    cout<<"Unidad (0=UNID 1=KG): "; cin>>u;
    p.unidad=(TipoUnidad)u;
    cout<<"Stock: "; cin>>p.stock;
    cout<<"Precio: "; cin>>p.precio;
    inventario.push_back(p);
    guardarProductos();
    color(10); cout<<"Producto guardado\n"; color(7);
}

void entradaSalida(bool entrada){
    string n; cout<<"Producto: "; cin>>n;
    int i=buscarProducto(n);
    if(i==-1){
        color(12); cout<<"No existe\n"; color(7); return;
    }
    float c; cout<<"Cantidad: "; cin>>c;
    if(!entrada && c>inventario[i].stock){
        color(12); cout<<"Stock insuficiente\n"; color(7); return;
    }
    inventario[i].stock += entrada ? c : -c;

    Movimiento m;
    m.producto=n;
    m.tipo=entrada?"ENTRADA":"SALIDA";
    m.cantidad=c;
    m.total=c*inventario[i].precio;
    m.fecha=pedirFecha();
    historial.push_back(m);

    guardarProductos();
    guardarHistorial();

    if(!entrada){
        color(11);
        cout<<"VOUCHER\nTotal S/. "<<m.total<<endl;
        color(7);
    }
}

Usuario* login(){
    string u,p;
    cout<<"Usuario: "; cin>>u;
    cout<<"Clave: "; cin>>p;
    for(auto &x:usuarios)
        if(x.user==u && x.pass==p) return &x;
    return nullptr;
}

// ================= MENU =================
void menu(Usuario* u){
    int op;
    do{
        cout<<"\n1 Inventario\n2 Entrada\n3 Salida\n";
        cout<<"4 Kardex\n";
        if(u->rol==ADMIN) cout<<"5 Nuevo Producto\n";
        cout<<"0 Salir\nOpcion: ";
        cin>>op;

        if(op==1)
            for(auto &p:inventario)
                cout<<p.nombre<<" "<<p.stock<<" S/."<<p.precio<<endl;
        if(op==2) entradaSalida(true);
        if(op==3) entradaSalida(false);
        if(op==4){
            string n; cout<<"Producto: "; cin>>n;
            for(auto &m:historial)
                if(m.producto==n)
                    cout<<m.tipo<<" "<<m.cantidad<<" "<<m.fecha.d<<"/"<<m.fecha.m<<endl;
        }
        if(op==5 && u->rol==ADMIN) registrarProducto();
    }while(op!=0);
}

// ================= MAIN =================
int main(){
    SetConsoleOutputCP(CP_UTF8);

    cargarUsuarios();
    cargarProductos();
    cargarHistorial();

    if(usuarios.empty()){
        usuarios.push_back({"admin","1234",ADMIN});
        guardarUsuarios();
    }

    Usuario* u=login();
    if(u) menu(u);
    else cout<<"Login incorrecto\n";

    return 0;
}