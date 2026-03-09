#include "Sistema.h"
#include <fstream>
#include <iomanip>

// === IMPLEMENTACIÓN DE GESTOR DE ARCHIVOS ===
void GestorArchivos::guardarInventario(const vector<Producto>& inventario) {
    ofstream f("productos.txt");
    for (const auto &p : inventario)
        f << p.getNombre() << " " << p.getStock() << " " << p.getPrecio() << " " 
          << (int)p.getUnidad() << " " << p.getStockMinimo() << endl;
    f.close();
}

void GestorArchivos::cargarInventario(vector<Producto>& inventario) {
    ifstream f("productos.txt");
    if (!f.is_open()) return;
    inventario.clear(); 
    string nombre; float stock, precio; int unidadTemp, stockMinimo;
    while (f >> nombre >> stock >> precio >> unidadTemp >> stockMinimo) {
        inventario.emplace_back(nombre, stock, precio, (TipoUnidad)unidadTemp, stockMinimo);
    }
    f.close();
}

void GestorArchivos::guardarHistorial(const Movimiento& m) {
    ofstream f("historial.txt", ios::app);
    f << m.getProducto() << " " << GestorUI::strMovimiento(m.getTipo()) << " " 
      << m.getCantidad() << " " << m.getTotal() << " " 
      << m.getFecha().getDia() << " " << m.getFecha().getMes() << " " << m.getFecha().getAnio() << endl;
    f.close();
}

// === IMPLEMENTACIÓN DE SISTEMA ALMACÉN ===
SistemaAlmacen::SistemaAlmacen() : ejecutando(true) {
    GestorArchivos::cargarInventario(inventario);
}

int SistemaAlmacen::buscarProducto(const string& n) const {
    for (size_t i = 0; i < inventario.size(); i++)
        if (inventario[i].getNombre() == n) return i;
    return -1;
}

void SistemaAlmacen::registrarProducto() {
    GestorUI::titulo("NUEVO PRODUCTO");
    string nombre;
    cout << "Nombre (sin espacios): "; cin >> nombre;
    if (buscarProducto(nombre) != -1) { GestorUI::mensajeError("El producto ya existe."); return; }
    
    int t; cout << "Unidad (1-Unidad, 2-Kg): "; cin >> t;
    TipoUnidad unidad = (t == 2) ? TipoUnidad::KG : TipoUnidad::UNIDAD;
    
    float stock, precio;
    cout << "Stock inicial: "; cin >> stock;
    cout << "Precio Venta: S/. "; cin >> precio;

    inventario.emplace_back(nombre, stock, precio, unidad, 5);
    GestorArchivos::guardarInventario(inventario);
    GestorUI::mensajeExito("Producto registrado correctamente");
}

void SistemaAlmacen::registrarMovimiento(TipoMovimiento tipoMov) {
    bool esEntrada = (tipoMov == TipoMovimiento::ENTRADA);
    GestorUI::titulo(esEntrada ? "ENTRADA DE STOCK" : "VENTA / SALIDA");
    
    string n; cout << "Nombre del Producto: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { GestorUI::mensajeError("Producto no encontrado."); return; }

    GestorUI::color(GestorUI::COLOR_AMARILLO);
    cout << "Stock actual: " << inventario[i].getStock() << " " << GestorUI::strUnidad(inventario[i].getUnidad()) << endl;
    GestorUI::color(GestorUI::COLOR_DEFECTO);

    float cant; cout << "Cantidad a " << (esEntrada ? "ingresar: " : "retirar: "); cin >> cant;

    if (!esEntrada && cant > inventario[i].getStock()) { GestorUI::mensajeError("Stock insuficiente."); return; }

    inventario[i].actualizarStock(cant, tipoMov);
    
    int d, m, a;
    cout << "Fecha (dia mes anio): "; cin >> d >> m >> a;
    Movimiento mov(n, tipoMov, cant, cant * inventario[i].getPrecio(), Fecha(d, m, a));

    GestorArchivos::guardarInventario(inventario);
    GestorArchivos::guardarHistorial(mov);

    if (!esEntrada) {
        cout << "\n"; GestorUI::color(GestorUI::COLOR_BLANCO);
        cout << "========= TICKET DE VENTA =========\n";
        cout << "Producto: " << n << endl;
        cout << "Cantidad: " << cant << " " << GestorUI::strUnidad(inventario[i].getUnidad()) << endl;
        cout << "Total:    S/. " << fixed << setprecision(2) << mov.getTotal() << endl;
        cout << "===================================\n"; system("pause");
    } else {
        GestorUI::mensajeExito("Stock actualizado.");
    }
}

void SistemaAlmacen::eliminarProducto() {
    GestorUI::titulo("ELIMINAR PRODUCTO");
    string n; cout << "Nombre: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { GestorUI::mensajeError("No existe"); return; }
    inventario.erase(inventario.begin() + i);
    GestorArchivos::guardarInventario(inventario);
    GestorUI::mensajeExito("Producto eliminado");
}

void SistemaAlmacen::modificarProducto(bool modificarPrecioYStock) {
    string n; cout << "Producto: "; cin >> n;
    int i = buscarProducto(n);
    if (i == -1) { GestorUI::mensajeError("No encontrado"); return; }

    if (modificarPrecioYStock) {
        float p, s;
        cout << "Nuevo Precio: "; cin >> p;
        cout << "Nuevo Stock: "; cin >> s;
        inventario[i].setPrecio(p);
        inventario[i].setStock(s);
    } else {
        int sm;
        cout << "Nuevo Stock Minimo: "; cin >> sm;
        inventario[i].setStockMinimo(sm);
    }
    GestorArchivos::guardarInventario(inventario);
    GestorUI::mensajeExito("Actualizado");
}

void SistemaAlmacen::verInventario() const {
    GestorUI::titulo("INVENTARIO GENERAL");
    GestorUI::color(GestorUI::COLOR_BLANCO);
    cout << left << setw(15) << "NOMBRE" << setw(10) << "STOCK" << setw(10) << "UNIDAD" << setw(12) << "PRECIO" << setw(15) << "ESTADO" << endl;
    cout << string(62, '-') << endl; GestorUI::color(GestorUI::COLOR_DEFECTO);

    for (const auto &p : inventario) {
        cout << left << setw(15) << p.getNombre() << setw(10) << p.getStock() << setw(10) << GestorUI::strUnidad(p.getUnidad()) 
             << "S/." << setw(9) << fixed << setprecision(2) << p.getPrecio();
        
        if (p.tieneStockBajo()) { GestorUI::color(GestorUI::COLOR_ROJO); cout << " [STOCK BAJO]"; } 
        else { GestorUI::color(GestorUI::COLOR_VERDE); cout << " [OK]"; }
        GestorUI::color(GestorUI::COLOR_DEFECTO); cout << endl;
    }
    cout << "\n"; system("pause");
}

void SistemaAlmacen::verHistorial() const {
    GestorUI::titulo("HISTORIAL DE MOVIMIENTOS");
    ifstream f("historial.txt");
    if (!f.is_open()) { GestorUI::mensajeError("No hay historial."); return; }
    string prod, tipo; float cant, tot; int d, m, a;
    cout << left << setw(12) << "FECHA" << setw(15) << "PRODUCTO" << setw(10) << "TIPO" << setw(10) << "CANT" << "TOTAL" << endl;
    cout << string(60, '-') << endl;
    while (f >> prod >> tipo >> cant >> tot >> d >> m >> a) {
        GestorUI::color(tipo == "ENTRADA" ? GestorUI::COLOR_VERDE : GestorUI::COLOR_ROJO);
        cout << d << "/" << m << "/" << a << "   " << left << setw(15) << prod << setw(10) << tipo << setw(10) << cant << "S/." << tot << endl;
    }
    f.close(); GestorUI::color(GestorUI::COLOR_DEFECTO); system("pause");
}

void SistemaAlmacen::reporteMensual() const {
    GestorUI::titulo("REPORTE MENSUAL");
    int mes, anio; float totalVentas = 0;
    cout << "Mes: "; cin >> mes; cout << "Anio: "; cin >> anio;
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
    if (encontrado) { GestorUI::color(GestorUI::COLOR_AMARILLO); cout << "TOTAL VENDIDO: S/. " << fixed << setprecision(2) << totalVentas << endl; } 
    else cout << "Sin ventas en esa fecha.\n";
    GestorUI::color(GestorUI::COLOR_DEFECTO); system("pause");
}