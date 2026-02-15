#ifndef DATOS_H
#define DATOS_H

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
    Sleep(1000); 
}

void mensajeError(string texto) {
    color(COLOR_ROJO);
    cout << "[ERROR] " << texto << endl;
    color(COLOR_DEFECTO);
    system("pause");
}

/* ================= ENUMS Y STRUCTS ================= */
enum Rol { ADMIN, EMPLEADO };
enum TipoUnidad { UNIDAD, KG };

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

#endif