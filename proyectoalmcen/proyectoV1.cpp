#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>   // Necesario para dar formato a los números (decimales)
#include <windows.h> // Para Windows

using namespace std;

// --- ENUMS Y ESTRUCTURAS ---

enum TipoUnidad { UNIDAD, KG };

struct Fecha {
    int dia, mes, anio;
};

struct Producto {
    string nombre;
    float stockActual;
    float precioUnitario; // Para el voucher
    TipoUnidad tipo;      // KG o UNIDAD
};

struct Movimiento {
    string productoNombre;
    string tipoMovimiento; // "ENTRADA" (Compra) o "SALIDA" (Venta/Consumo)
    float cantidad;
    float totalDinero;
    Fecha fecha;
    TipoUnidad unidad;
};

struct Usuario {
    string username;
    string password;
};

// --- BASE DE DATOS (En memoria) ---
vector<Usuario> usuariosRegistrados;
vector<Producto> inventarioGlobal;     // Inventario actual
vector<Movimiento> historialGlobal;    // Historial de todas las transacciones

// --- FUNCIONES DE UTILIDAD ---

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Función para obtener texto del tipo de unidad
string strUnidad(TipoUnidad t) {
    return (t == KG) ? "kg" : "unid";
}

float validarEntrada(string mensaje) {
    float valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= 0) {
            return valor;
        } else {
            cout << ">> Error: Entrada inválida. Ingrese un número positivo.\n";
            cin.clear();
            limpiarBuffer();
        }
    }
}

// Función simple para pedir fecha (para simular historial)
Fecha pedirFecha() {
    Fecha f;
    cout << "Ingrese fecha (DD MM AAAA): ";
    while (!(cin >> f.dia >> f.mes >> f.anio) || f.mes < 1 || f.mes > 12 || f.dia < 1 || f.dia > 31) {
        cout << ">> Fecha inválida. Intente de nuevo (Ej: 15 03 2026): ";
        cin.clear();
        limpiarBuffer();
    }
    return f;
}

// --- VOUCHER ---
void imprimirVoucher(Movimiento m) {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "          COMPROBANTE DE OPERACION      " << endl;
    cout << "========================================" << endl;
    cout << " Fecha: " << m.fecha.dia << "/" << m.fecha.mes << "/" << m.fecha.anio << endl;
    cout << " Tipo:  " << m.tipoMovimiento << endl;
    cout << "----------------------------------------" << endl;
    cout << " PRODUCTO: " << m.productoNombre << endl;
    cout << " CANTIDAD: " << m.cantidad << " " << strUnidad(m.unidad) << endl;
    cout << " TOTAL:    S/. " << fixed << setprecision(2) << m.totalDinero << endl;
    cout << "========================================" << endl;
    cout << "      Gracias por usar nuestro sistema  " << endl;
    cout << "========================================" << endl;
    cout << "\nPresione Enter para continuar...";
    limpiarBuffer();
    cin.get();
}

// --- GESTIÓN DE PRODUCTOS ---

// Buscar índice del producto, retorna -1 si no existe
int buscarProducto(string nombre) {
    for (size_t i = 0; i < inventarioGlobal.size(); i++) {
        if (inventarioGlobal[i].nombre == nombre) return i;
    }
    return -1;
}

void agregarProducto() {
    cout << "\n=== ALTA DE NUEVO PRODUCTO ===" << endl;
    Producto p;
    cout << "Nombre del producto: ";
    cin >> p.nombre; // Simplificado sin espacios para este ejemplo rápido

    if (buscarProducto(p.nombre) != -1) {
        cout << ">> El producto ya existe. Use la opción de 'Entrada de Stock'.\n";
        return;
    }

    int tipoInput;
    cout << "Tipo de unidad (1: Unidades, 2: Kilogramos): ";
    cin >> tipoInput;
    p.tipo = (tipoInput == 2) ? KG : UNIDAD;

    p.stockActual = validarEntrada("Stock Inicial: ");
    p.precioUnitario = validarEntrada("Precio Unitario (S/.): ");

    inventarioGlobal.push_back(p);
    
    // Registrar el movimiento inicial
    Movimiento mov;
    mov.productoNombre = p.nombre;
    mov.tipoMovimiento = "ENTRADA (INICIO)";
    mov.cantidad = p.stockActual;
    mov.unidad = p.tipo;
    mov.totalDinero = p.stockActual * p.precioUnitario;
    mov.fecha = pedirFecha();

    historialGlobal.push_back(mov);
    cout << ">> Producto registrado con éxito.\n";
}

void registrarMovimiento(bool esEntrada) {
    if (inventarioGlobal.empty()) {
        cout << ">> No hay productos registrados.\n";
        return;
    }

    string nombre;
    cout << "\n=== " << (esEntrada ? "ENTRADA DE STOCK" : "VENTA / SALIDA") << " ===" << endl;
    cout << "Nombre del producto: ";
    cin >> nombre;

    int idx = buscarProducto(nombre);
    if (idx == -1) {
        cout << ">> Producto no encontrado.\n";
        return;
    }

    Producto &prod = inventarioGlobal[idx];
    cout << "Stock actual: " << prod.stockActual << " " << strUnidad(prod.tipo) << endl;

    float cantidad = validarEntrada("Cantidad a " + string(esEntrada ? "ingresar: " : "retirar: "));

    if (!esEntrada && cantidad > prod.stockActual) {
        cout << ">> Error: Stock insuficiente.\n";
        return;
    }

    // Actualizar Stock
    if (esEntrada) prod.stockActual += cantidad;
    else prod.stockActual -= cantidad;

    // Guardar en Historial
    Movimiento mov;
    mov.productoNombre = prod.nombre;
    mov.tipoMovimiento = esEntrada ? "ENTRADA" : "SALIDA";
    mov.cantidad = cantidad;
    mov.unidad = prod.tipo;
    mov.totalDinero = cantidad * prod.precioUnitario;
    mov.fecha = pedirFecha();

    historialGlobal.push_back(mov);

    cout << ">> Stock actualizado: " << prod.stockActual << " " << strUnidad(prod.tipo) << endl;

    if (!esEntrada) {
        imprimirVoucher(mov); // Imprimir voucher solo si es salida/venta
    }
}

// --- REPORTES ---

void verInventarioActual() {
    cout << "\n======= INVENTARIO ACTUAL =======" << endl;
    cout << left << setw(15) << "Producto" << setw(10) << "Stock" << setw(10) << "Unidad" << "Precio/U" << endl;
    cout << "-----------------------------------------------" << endl;
    for (const auto& p : inventarioGlobal) {
        cout << left << setw(15) << p.nombre 
             << setw(10) << p.stockActual 
             << setw(10) << strUnidad(p.tipo) 
             << "S/." << p.precioUnitario << endl;
        
        if(p.stockActual < 5) cout << "   [ALERTA: STOCK BAJO]" << endl;
    }
    cout << "-----------------------------------------------" << endl;
}

void verHistorial() {
    cout << "\n======= HISTORIAL COMPLETO =======" << endl;
    for (const auto& m : historialGlobal) {
        cout << "[" << m.fecha.dia << "/" << m.fecha.mes << "/" << m.fecha.anio << "] "
             << m.tipoMovimiento << ": " << m.cantidad << " " << strUnidad(m.unidad)
             << " de " << m.productoNombre << " (Total: S/." << m.totalDinero << ")\n";
    }
}

void reporteMensual() {
    int mes, anio;
    cout << "\n=== REPORTE MENSUAL ===" << endl;
    cout << "Ingrese Mes (1-12): "; cin >> mes;
    cout << "Ingrese Año: "; cin >> anio;

    cout << "\nResultados del " << mes << "/" << anio << ":" << endl;
    cout << "-----------------------------------" << endl;
    
    float totalIngresos = 0;
    bool encontrado = false;

    for (const auto& m : historialGlobal) {
        if (m.fecha.mes == mes && m.fecha.anio == anio) {
            cout << m.fecha.dia << "/" << m.fecha.mes << " - " 
                 << m.productoNombre << " (" << m.tipoMovimiento << ") " 
                 << m.cantidad << strUnidad(m.unidad) << endl;
            
            if (m.tipoMovimiento == "SALIDA") {
                totalIngresos += m.totalDinero;
            }
            encontrado = true;
        }
    }

    if (!encontrado) cout << ">> No hubo movimientos en este mes.\n";
    
    cout << "-----------------------------------" << endl;
    cout << "TOTAL VENTAS (SALIDAS) DEL MES: S/. " << totalIngresos << endl;
}

// --- LOGIN Y MENU ---

void registrarUsuario() {
    Usuario nuevoUser;
    cout << "\n=== REGISTRO DE USUARIO ===" << endl;
    cout << "Usuario: "; cin >> nuevoUser.username;
    
    for (const auto& u : usuariosRegistrados) {
        if (u.username == nuevoUser.username) {
            cout << ">> Error: Usuario ya existe.\n"; return;
        }
    }
    cout << "Contraseña: "; cin >> nuevoUser.password;
    usuariosRegistrados.push_back(nuevoUser);
    cout << ">> Usuario registrado.\n";
}

bool iniciarSesion() {
    if (usuariosRegistrados.empty()) {
        cout << ">> Registre un usuario primero.\n"; return false;
    }
    string u, p;
    cout << "\n=== LOGIN ===" << endl;
    cout << "Usuario: "; cin >> u;
    cout << "Pass: "; cin >> p;

    for (const auto& user : usuariosRegistrados) {
        if (user.username == u && user.password == p) return true;
    }
    cout << ">> Datos incorrectos.\n"; return false;
}

void menuPrincipal() {
    int op;
    do {
        cout << "\n################################" << endl;
        cout << "      CONTROL DE ALMACEN v1.0   " << endl;
        cout << "################################" << endl;
        cout << "1. Registrar Nuevo Producto" << endl;
        cout << "2. Entrada de Stock (Compra)" << endl;
        cout << "3. Salida de Stock (Venta + Voucher)" << endl;
        cout << "4. Ver Inventario Actual" << endl;
        cout << "5. Ver Historial Completo" << endl;
        cout << "6. Reporte Mensual" << endl;
        cout << "7. Cerrar Sesión" << endl;
        cout << "Opción: ";
        
        if (!(cin >> op)) {
            cin.clear(); limpiarBuffer(); op = 0;
        }

        switch(op) {
            case 1: agregarProducto(); break;
            case 2: registrarMovimiento(true); break;
            case 3: registrarMovimiento(false); break;
            case 4: verInventarioActual(); break;
            case 5: verHistorial(); break;
            case 6: reporteMensual(); break;
            case 7: cout << "Cerrando sesión...\n"; break;
            default: cout << "Opción inválida.\n";
        }
    } while(op != 7);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int opcion;
    bool salir = false;

    // Usuario por defecto para pruebas rápidas (opcional)
    usuariosRegistrados.push_back({"admin", "1234"}); 

    while (!salir) {
        cout << "\n1. Registrarse | 2. Login | 3. Salir: ";
        if(cin >> opcion) {
            if (opcion == 1) registrarUsuario();
            else if (opcion == 2) {
                if(iniciarSesion()) menuPrincipal();
            }
            else if (opcion == 3) salir = true;
            else cout << "Inválido.\n";
        } else {
            cin.clear(); limpiarBuffer();
        }
    }
    return 0;
}