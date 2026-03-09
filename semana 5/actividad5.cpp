#include <iostream>
#include <string>

using namespace std;

// --- Parte A: Clase Base ---
class Persona {
private:
    string nombre;
    int edad;

public:
    // Constructor de la clase base
    Persona(string nombre, int edad) {
        this->nombre = nombre;
        setEdad(edad); // Usamos el setter para validar
    }

    // Setters y Getters con validación [cite: 50]
    void setNombre(string nombre) { this->nombre = nombre; }
    string getNombre() const { return nombre; }

    void setEdad(int edad) {
        if (edad >= 0) {
            this->edad = edad;
        } else {
            cout << "Error: La edad no puede ser negativa. Asignando 0." << endl;
            this->edad = 0;
        }
    }
    int getEdad() const { return edad; }

    // Método mostrar() [cite: 51]
    void mostrar() {
        cout << "Nombre: " << nombre << " | Edad: " << edad;
    }
};

// --- Parte B: Clases Derivadas ---

// 4) Clase Estudiante que hereda de Persona [cite: 53]
class Estudiante : public Persona {
private:
    string carrera;

public:
    // El constructor de la hija debe llamar al constructor de la madre
    Estudiante(string nombre, int edad, string carrera) : Persona(nombre, edad) {
        this->carrera = carrera;
    }

    void estudiar() {
        cout << " -> El estudiante esta estudiando " << carrera << "." << endl;
    }
};

// 5) Clase Profesor que herede de Persona [cite: 56]
class Profesor : public Persona {
private:
    string materia;

public:
    Profesor(string nombre, int edad, string materia) : Persona(nombre, edad) {
        this->materia = materia;
    }

    void ensenar() {
        cout << " -> El profesor esta ensenando " << materia << "." << endl;
    }
};

// --- Parte C: Pruebas ---
int main() {
    // 6) Crear un Estudiante y un Profesor [cite: 60]
    Estudiante est1("Diego", 20, "Ingenieria de Sistemas");
    Profesor prof1("Ing. Martinez", 45, "Programacion C++");

    // Mostrar datos y llamar métodos [cite: 60]
    cout << "--- Datos del Estudiante ---" << endl;
    est1.mostrar(); 
    est1.estudiar();

    cout << "\n--- Datos del Profesor ---" << endl;
    prof1.mostrar();
    prof1.ensenar();

    return 0;
}