#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

enum class Rol { ADMIN, EMPLEADO };
enum class TipoUnidad { UNIDAD, KG };
enum class TipoMovimiento { ENTRADA, SALIDA };

class GestorUI {
public:
    static const int COLOR_DEFECTO = 7;
    static const int COLOR_AZUL = 11;
    static const int COLOR_VERDE = 10;
    static const int COLOR_ROJO = 12;
    static const int COLOR_AMARILLO = 14;
    static const int COLOR_BLANCO = 15;

    static void color(int c);
    static void titulo(const string& texto);
    static void mensajeExito(const string& texto);
    static void mensajeError(const string& texto);
    static string strUnidad(TipoUnidad u);
    static string strMovimiento(TipoMovimiento m);
};

class Seguridad {
public:
    static string leerPassword();
    static string codificar(string p);
    static string decodificar(string p);
};

#endif