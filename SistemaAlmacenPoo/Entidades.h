#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "Utilidades.h"

class Fecha {
private:
    int d, m, a;
public:
    Fecha(int dia = 1, int mes = 1, int anio = 2000) : d(dia), m(mes), a(anio) {}
    int getDia() const { return d; }
    int getMes() const { return m; }
    int getAnio() const { return a; }
};

class Producto {
private:
    string nombre;
    float stock;
    float precio;
    TipoUnidad unidad;
    int stockMinimo;

public:
    Producto() : nombre(""), stock(0), precio(0), unidad(TipoUnidad::UNIDAD), stockMinimo(5) {}
    Producto(string n, float s, float p, TipoUnidad u, int sm) 
        : nombre(n), stock(s), precio(p), unidad(u), stockMinimo(sm) {}

    string getNombre() const { return nombre; }
    float getStock() const { return stock; }
    float getPrecio() const { return precio; }
    TipoUnidad getUnidad() const { return unidad; }
    int getStockMinimo() const { return stockMinimo; }

    void setStock(float s) { stock = s; }
    void setPrecio(float p) { precio = p; }
    void setStockMinimo(int sm) { stockMinimo = sm; }
    
    void actualizarStock(float cantidad, TipoMovimiento tipo) {
        if (tipo == TipoMovimiento::ENTRADA) stock += cantidad;
        else stock -= cantidad;
    }
    bool tieneStockBajo() const { return stock <= stockMinimo; }
};

class Movimiento {
private:
    string producto;
    TipoMovimiento tipo;
    float cantidad;
    float total;
    Fecha fecha;

public:
    Movimiento(string p, TipoMovimiento t, float c, float tot, Fecha f)
        : producto(p), tipo(t), cantidad(c), total(tot), fecha(f) {}

    string getProducto() const { return producto; }
    TipoMovimiento getTipo() const { return tipo; }
    float getCantidad() const { return cantidad; }
    float getTotal() const { return total; }
    Fecha getFecha() const { return fecha; }
};

#endif