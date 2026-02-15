/*
==================== BITÁCORA IA ====================
Herramienta IA usada: Google Gemini
Objetivo del uso: Generar estructura base de memoria dinámica y lógica de redimensionamiento manual.
Preguntas que hice a la IA (resumen):
1) ¿Cómo implementar un arreglo dinámico en C++ que crezca automáticamente sin usar std::vector?
2) ¿Cuál es la mejor forma de buscar un struct en un arreglo y devolver un puntero para modificarlo?
3) Dame un ejemplo de cómo guardar structs con strings que tienen espacios en un archivo de texto.

Qué sugerencias acepté y por qué:
- Acepté usar un puntero auxiliar para redimensionar (copiar datos viejos -> borrar viejo -> asignar nuevo) porque es la forma estándar de manejar memoria manual.
- Acepté retornar "Libro*" en la función de búsqueda para poder modificar el estado "disponible" directamente desde la referencia del puntero.
- Acepté usar getline para la persistencia para evitar problemas con espacios en los nombres de libros.

Qué sugerencias rechacé y por qué:
- Rechacé usar la librería <vector> aunque la IA lo sugirió primero, porque las restricciones del examen obligan a usar new[] y delete[].

Test manual diseñado por mí:
Entrada (pasos/comandos):
- 1. Agregar Libro: ID 10, Título "C++ Básico", Autor "Stroustrup", Año 2020.
- 2. Agregar Usuario: ID 50, Nombre "Juan Perez", Carrera "Sistemas".
- 3. Prestar: Usuario 50 pide Libro 10.
- 4. Listar Libros (verificar que ID 10 diga "No disponible").
- 5. Devolver: Libro 10.
Salida esperada:
- El préstamo se registra correctamente. Al listar, el libro pasa de Disponible a NO Disponible.
- Al devolver, el libro vuelve a estar Disponible y el préstamo desaparece de la lista de activos.

Nota de autoría:
Declaro que entiendo el código entregado y puedo explicarlo.
====================================================
*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> // Para transform (case insensitive)

using namespace std;

// ================= ESTUCTURAS [cite: 22, 25, 36] =================

struct Libro {
    int id;
    string titulo;
    string autor;
    int anio;
    bool disponible;
};

struct Usuario {
    int id;
    string nombre;
    string carrera;
};

struct Prestamo {
    int idUsuario;
    int idLibro;
    // Se podrían agregar fechas, pero el requerimiento mínimo es registrar el vínculo.
};

// ================= VARIABLES GLOBALES (ESTADO DEL SISTEMA) =================
// Se usan punteros para manejar arreglos dinámicos [cite: 43]

Libro* libros = nullptr;
int cantLibros = 0;
int capLibros = 2; // Capacidad inicial pequeña para probar redimensión

Usuario* usuarios = nullptr;
int cantUsuarios = 0;
int capUsuarios = 2;

Prestamo* prestamos = nullptr;
int cantPrestamos = 0;
int capPrestamos = 2;

// ================= PROTOTIPOS DE FUNCIONES [cite: 37] =================

// Gestión de Memoria
void inicializarSistema();
void redimensionarLibros();
void redimensionarUsuarios();
void redimensionarPrestamos();
void liberarMemoria(); // 

// Lógica de Negocio
void agregarLibro();
void listarLibros();
void agregarUsuario();
void listarUsuarios();
void prestarLibro(); // [cite: 28]
void devolverLibro(); // [cite: 29]
void mostrarPrestamosActivos(); // [cite: 32]

// Búsqueda y Utilidades
Libro* buscarLibroPorId(int id); // Retorna puntero [cite: 41]
Usuario* buscarUsuarioPorId(int id);
void buscarLibrosPorTexto(); // [cite: 31]
string aMinusculas(string str);

// Persistencia (Extra)
void guardarDatos();
void cargarDatos();

// ================= MAIN =================

int main() {
    inicializarSistema();
    cargarDatos(); // Intentar cargar datos previos

    int opcion = 0;
    do {
        cout << "\n=== SISTEMA DE BIBLIOTECA ===" << endl;
        cout << "1. Agregar Libro" << endl;
        cout << "2. Listar Libros" << endl;
        cout << "3. Agregar Usuario" << endl;
        cout << "4. Listar Usuarios" << endl;
        cout << "5. Prestar Libro" << endl;
        cout << "6. Devolver Libro" << endl;
        cout << "7. Ver Prestamos Activos" << endl;
        cout << "8. Buscar Libro (Titulo/Autor)" << endl;
        cout << "9. Guardar y Salir" << endl;
        cout << "Opcion: ";
        
        if (!(cin >> opcion)) { // Validación básica de entrada
            cout << "Entrada invalida." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // Limpiar buffer del enter

        switch(opcion) {
            case 1: agregarLibro(); break;
            case 2: listarLibros(); break;
            case 3: agregarUsuario(); break;
            case 4: listarUsuarios(); break;
            case 5: prestarLibro(); break;
            case 6: devolverLibro(); break;
            case 7: mostrarPrestamosActivos(); break;
            case 8: buscarLibrosPorTexto(); break;
            case 9: 
                guardarDatos();
                cout << "Saliendo..." << endl; 
                break;
            default: cout << "Opcion desconocida." << endl;
        }

    } while (opcion != 9);

    liberarMemoria();
    return 0;
}

// ================= IMPLEMENTACIÓN DE FUNCIONES =================

void inicializarSistema() {
    // Reserva inicial de memoria dinámica [cite: 42]
    libros = new Libro[capLibros];
    usuarios = new Usuario[capUsuarios];
    prestamos = new Prestamo[capPrestamos];
}

// Función auxiliar para redimensionar (Duplicar capacidad) [cite: 44]
void redimensionarLibros() {
    cout << "[DEBUG] Redimensionando arreglo de libros (Capacidad: " << capLibros << " -> " << capLibros * 2 << ")..." << endl;
    int nuevaCap = capLibros * 2;
    Libro* nuevoArr = new Libro[nuevaCap];

    // Copiar datos
    for (int i = 0; i < cantLibros; i++) {
        nuevoArr[i] = libros[i];
    }

    delete[] libros; // Liberar memoria vieja
    libros = nuevoArr; // Apuntar al nuevo arreglo
    capLibros = nuevaCap;
}

void redimensionarUsuarios() {
    int nuevaCap = capUsuarios * 2;
    Usuario* nuevoArr = new Usuario[nuevaCap];
    for (int i = 0; i < cantUsuarios; i++) nuevoArr[i] = usuarios[i];
    delete[] usuarios;
    usuarios = nuevoArr;
    capUsuarios = nuevaCap;
}

void redimensionarPrestamos() {
    int nuevaCap = capPrestamos * 2;
    Prestamo* nuevoArr = new Prestamo[nuevaCap];
    for (int i = 0; i < cantPrestamos; i++) nuevoArr[i] = prestamos[i];
    delete[] prestamos;
    prestamos = nuevoArr;
    capPrestamos = nuevaCap;
}

void agregarLibro() {
    if (cantLibros >= capLibros) {
        redimensionarLibros();
    }

    Libro nuevo;
    cout << "ID del libro: "; cin >> nuevo.id;
    cin.ignore(); // Limpiar buffer
    
    // Validación ID duplicado [cite: 46]
    if (buscarLibroPorId(nuevo.id) != nullptr) {
        cout << "Error: Ya existe un libro con ese ID.\n";
        return;
    }

    cout << "Titulo: "; getline(cin, nuevo.titulo);
    cout << "Autor: "; getline(cin, nuevo.autor);
    cout << "Anio: "; cin >> nuevo.anio;
    nuevo.disponible = true;

    libros[cantLibros] = nuevo;
    cantLibros++;
    cout << "Libro agregado con exito.\n";
}

void listarLibros() {
    if (cantLibros == 0) {
        cout << "No hay libros registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE LIBROS ---\n";
    for (int i = 0; i < cantLibros; i++) {
        cout << "ID: " << libros[i].id 
             << " | " << libros[i].titulo 
             << " | " << libros[i].autor 
             << " | Estado: " << (libros[i].disponible ? "Disponible" : "PRESTADO") 
             << endl;
    }
}

void agregarUsuario() {
    if (cantUsuarios >= capUsuarios) {
        redimensionarUsuarios();
    }

    Usuario nuevo;
    cout << "ID Usuario: "; cin >> nuevo.id;
    cin.ignore();

    if (buscarUsuarioPorId(nuevo.id) != nullptr) {
        cout << "Error: ID de usuario repetido.\n";
        return;
    }

    cout << "Nombre: "; getline(cin, nuevo.nombre);
    cout << "Carrera: "; getline(cin, nuevo.carrera);

    usuarios[cantUsuarios] = nuevo;
    cantUsuarios++;
    cout << "Usuario agregado.\n";
}

void listarUsuarios() {
    if (cantUsuarios == 0) {
        cout << "No hay usuarios registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE USUARIOS ---\n";
    for (int i = 0; i < cantUsuarios; i++) {
        cout << "ID: " << usuarios[i].id 
             << " | " << usuarios[i].nombre 
             << " | " << usuarios[i].carrera << endl;
    }
}

// Uso de punteros para buscar (Retorna dirección de memoria o nullptr) 
Libro* buscarLibroPorId(int id) {
    for (int i = 0; i < cantLibros; i++) {
        if (libros[i].id == id) {
            return &libros[i]; // Retorna la dirección del libro en el arreglo
        }
    }
    return nullptr;
}

Usuario* buscarUsuarioPorId(int id) {
    for (int i = 0; i < cantUsuarios; i++) {
        if (usuarios[i].id == id) {
            return &usuarios[i];
        }
    }
    return nullptr;
}

void prestarLibro() {
    int idU, idL;
    cout << "Ingrese ID del Usuario: "; cin >> idU;
    cout << "Ingrese ID del Libro: "; cin >> idL;

    Usuario* u = buscarUsuarioPorId(idU);
    Libro* l = buscarLibroPorId(idL);

    if (u == nullptr) {
        cout << "Error: Usuario no encontrado.\n";
        return;
    }
    if (l == nullptr) {
        cout << "Error: Libro no encontrado.\n";
        return;
    }
    if (!l->disponible) {
        cout << "Error: El libro ya esta prestado.\n";
        return;
    }

    // Registrar prestamo
    if (cantPrestamos >= capPrestamos) {
        redimensionarPrestamos();
    }
    
    prestamos[cantPrestamos].idUsuario = idU;
    prestamos[cantPrestamos].idLibro = idL;
    cantPrestamos++;

    // Modificar estado del libro (Referencia a través del puntero) [cite: 39]
    l->disponible = false; 

    cout << "Prestamo realizado con exito: " << l->titulo << " a " << u->nombre << endl;
}

void devolverLibro() {
    int idL;
    cout << "Ingrese ID del Libro a devolver: "; cin >> idL;

    // Buscar si existe el libro
    Libro* l = buscarLibroPorId(idL);
    if (l == nullptr) {
        cout << "Libro no existe.\n";
        return;
    }
    if (l->disponible) {
        cout << "Este libro no estaba prestado.\n";
        return;
    }

    // Buscar el préstamo activo y eliminarlo
    int indicePrestamo = -1;
    for (int i = 0; i < cantPrestamos; i++) {
        if (prestamos[i].idLibro == idL) {
            indicePrestamo = i;
            break;
        }
    }

    if (indicePrestamo != -1) {
        // Eliminar préstamo (Movemos el último al lugar del borrado para no dejar huecos)
        prestamos[indicePrestamo] = prestamos[cantPrestamos - 1];
        cantPrestamos--;
        
        // Actualizar libro
        l->disponible = true;
        cout << "Libro devuelto correctamente.\n";
    } else {
        cout << "Error de inconsistencia: El libro figura como no disponible pero no hay registro de prestamo.\n";
        l->disponible = true; // Corrección forzada
    }
}

void mostrarPrestamosActivos() {
    cout << "\n--- PRESTAMOS ACTIVOS ---\n";
    if (cantPrestamos == 0) cout << "(Ninguno)\n";

    for (int i = 0; i < cantPrestamos; i++) {
        Usuario* u = buscarUsuarioPorId(prestamos[i].idUsuario);
        Libro* l = buscarLibroPorId(prestamos[i].idLibro);
        
        // Validación de seguridad por si se borraron usuarios/libros (opcional)
        string nombreU = (u != nullptr) ? u->nombre : "Usuario Eliminado";
        string tituloL = (l != nullptr) ? l->titulo : "Libro Eliminado";

        cout << "Libro ID: " << prestamos[i].idLibro << " (" << tituloL << ") "
             << "--> Prestado a: " << nombreU << endl;
    }
}

// Utilidad para búsqueda insensible a mayúsculas
string aMinusculas(string str) {
    string temp = str;
    // transform aplica tolower a todo el string
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    return temp;
}

void buscarLibrosPorTexto() {
    string busqueda;
    cout << "Ingrese texto a buscar (titulo o autor): ";
    getline(cin, busqueda);
    string busquedaMin = aMinusculas(busqueda);

    cout << "Resultados de busqueda:\n";
    bool encontrado = false;
    for (int i = 0; i < cantLibros; i++) {
        string tituloMin = aMinusculas(libros[i].titulo);
        string autorMin = aMinusculas(libros[i].autor);

        // npos significa "no position" (no encontrado) [cite: 31]
        if (tituloMin.find(busquedaMin) != string::npos || 
            autorMin.find(busquedaMin) != string::npos) {
            cout << "- [" << libros[i].id << "] " << libros[i].titulo << " (" << libros[i].autor << ")\n";
            encontrado = true;
        }
    }
    if (!encontrado) cout << "No se encontraron coincidencias.\n";
}

void liberarMemoria() {
    delete[] libros;
    delete[] usuarios;
    delete[] prestamos;
    cout << "Memoria liberada correctamente.\n";
}

// ================= PERSISTENCIA (EXTRA)  =================
// Formato simple: Guardamos contadores primero, luego los datos linea por linea.

void guardarDatos() {
    ofstream archivo("datos_biblioteca.txt");
    if (!archivo.is_open()) {
        cout << "Error al guardar archivo.\n";
        return;
    }

    // 1. Guardar Libros
    archivo << cantLibros << endl;
    for(int i=0; i<cantLibros; i++) {
        archivo << libros[i].id << endl;
        archivo << libros[i].titulo << endl;
        archivo << libros[i].autor << endl;
        archivo << libros[i].anio << endl;
        archivo << libros[i].disponible << endl;
    }

    // 2. Guardar Usuarios
    archivo << cantUsuarios << endl;
    for(int i=0; i<cantUsuarios; i++) {
        archivo << usuarios[i].id << endl;
        archivo << usuarios[i].nombre << endl;
        archivo << usuarios[i].carrera << endl;
    }

    // 3. Guardar Prestamos
    archivo << cantPrestamos << endl;
    for(int i=0; i<cantPrestamos; i++) {
        archivo << prestamos[i].idUsuario << endl;
        archivo << prestamos[i].idLibro << endl;
    }

    archivo.close();
    cout << "Datos guardados en 'datos_biblioteca.txt'.\n";
}

void cargarDatos() {
    ifstream archivo("datos_biblioteca.txt");
    if (!archivo.is_open()) return; // No existe archivo previo, iniciamos vacio

    // Limpiar memoria actual si hubiera (en este caso inicia vacio, pero por seguridad)
    // Se asume que cargamos al inicio del programa.

    int total;
    
    // Cargar Libros
    archivo >> total;
    // Aseguramos capacidad
    while(capLibros < total) redimensionarLibros(); 
    cantLibros = total;
    
    for(int i=0; i<total; i++) {
        archivo >> libros[i].id;
        archivo.ignore(); // saltar newline
        getline(archivo, libros[i].titulo);
        getline(archivo, libros[i].autor);
        archivo >> libros[i].anio;
        archivo >> libros[i].disponible;
    }

    // Cargar Usuarios
    archivo >> total;
    while(capUsuarios < total) redimensionarUsuarios();
    cantUsuarios = total;

    for(int i=0; i<total; i++) {
        archivo >> usuarios[i].id;
        archivo.ignore();
        getline(archivo, usuarios[i].nombre);
        getline(archivo, usuarios[i].carrera);
    }

    // Cargar Prestamos
    archivo >> total;
    while(capPrestamos < total) redimensionarPrestamos();
    cantPrestamos = total;

    for(int i=0; i<total; i++) {
        archivo >> prestamos[i].idUsuario;
        archivo >> prestamos[i].idLibro;
    }

    archivo.close();
    cout << "Datos cargados correctamente (" << cantLibros << " libros, " << cantUsuarios << " usuarios).\n";
}