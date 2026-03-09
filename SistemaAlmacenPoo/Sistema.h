#ifndef SISTEMA_H
#define SISTEMA_H

#include "Entidades.h"
#include <vector>

class GestorArchivos {
public:
    static void guardarInventario(const vector<Producto>& inventario);
    static void cargarInventario(vector<Producto>& inventario);
    static void guardarHistorial(const Movimiento& m);
};

class SistemaAlmacen {
private:
    vector<Producto> inventario;
    bool ejecutando;
    int buscarProducto(const string& n) const;

public:
    SistemaAlmacen();
    bool estaEjecutando() const { return ejecutando; }
    void detenerSistema() { ejecutando = false; }
    
    void registrarProducto();
    void registrarMovimiento(TipoMovimiento tipoMov);
    void eliminarProducto();
    void modificarProducto(bool modificarPrecioYStock);
    void verInventario() const;
    void verHistorial() const;
    void reporteMensual() const;
};

#endif