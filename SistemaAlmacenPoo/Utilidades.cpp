#include "Utilidades.h"
#include <conio.h>

void GestorUI::color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void GestorUI::titulo(const string& texto) {
    system("cls");
    color(COLOR_AZUL);
    cout << "========================================" << endl;
    cout << "   " << texto << endl;
    cout << "========================================" << endl;
    color(COLOR_DEFECTO);
}

void GestorUI::mensajeExito(const string& texto) {
    color(COLOR_VERDE);
    cout << "[!] " << texto << endl;
    color(COLOR_DEFECTO);
    Sleep(1000); 
}

void GestorUI::mensajeError(const string& texto) {
    color(COLOR_ROJO);
    cout << "[ERROR] " << texto << endl;
    color(COLOR_DEFECTO);
    system("pause");
}

string GestorUI::strUnidad(TipoUnidad u) {
    return (u == TipoUnidad::KG) ? "kg" : "unid";
}

string GestorUI::strMovimiento(TipoMovimiento m) {
    return (m == TipoMovimiento::ENTRADA) ? "ENTRADA" : "SALIDA";
}

string Seguridad::leerPassword() {
    string pass; char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { cout << endl; break; }
        if (ch == 8) { 
            if (!pass.empty()) { pass.pop_back(); cout << "\b \b"; }
        } else { pass += ch; cout << "*"; }
    }
    return pass;
}

string Seguridad::codificar(string p) { for (char &c : p) c += 3; return p; }
string Seguridad::decodificar(string p) { for (char &c : p) c -= 3; return p; }