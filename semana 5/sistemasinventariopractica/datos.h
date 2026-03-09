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

// NUEVO: Funciones para validar entrada de datos
int leerEntero(string mensaje) {
    int num;
    cout << mensaje;
    while (!(cin >> num)) {
        cin.clear(); // Limpia el estado de error de cin
        // Ignora los caracteres incorrectos hasta el salto de linea (nota: usamos parentesis extras para evitar conflictos con windows.h)
        cin.ignore((numeric_limits<streamsize>::max)(), '\n'); 
        color(COLOR_ROJO);
        cout << "[ERROR] Dato invalido. " << mensaje;
        color(COLOR_DEFECTO);
    }
    return num;
}

float leerFlotante(string mensaje) {
    float num;
    cout << mensaje;
    while (!(cin >> num)) {
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        color(COLOR_ROJO);
        cout << "[ERROR] Dato invalido. " << mensaje;
        color(COLOR_DEFECTO);
    }
    return num;
}

#endif