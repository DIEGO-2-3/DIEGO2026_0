#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// ================= INTERFAZ DE MONITOREO =================
class Monitoreable {
public:
    virtual void monitorear() = 0;
    virtual ~Monitoreable() {}
};

// ================= CLASE ABSTRACTA =================
class Vehiculo {
protected:
    int id;
    double velocidad;

public:
    Vehiculo(int id, double velocidad) : id(id), velocidad(velocidad) {}

    virtual void moverse() = 0;

    virtual void mostrarInfo() {
        cout << "ID: " << id << " | Velocidad: " << velocidad << " km/h" << endl;
    }

    virtual ~Vehiculo() {
        cout << "Destructor Vehiculo llamado" << endl;
    }

    friend class Reporte;
};

// ================= CLASE AUTO =================
class Auto : public Vehiculo, public Monitoreable {
private:
    int puertas;

public:
    Auto(int id, double velocidad, int puertas)
        : Vehiculo(id, velocidad), puertas(puertas) {}

    void moverse() override {
        cout << "Auto desplazandose por carretera..." << endl;
    }

    void monitorear() override {
        cout << "Monitoreando sistema de conduccion autonoma del Auto." << endl;
    }

    void mostrarInfo() override {
        Vehiculo::mostrarInfo();
        cout << "Puertas: " << puertas << endl;
    }
};

// ================= CLASE DRONE =================
class Drone : public Vehiculo, public Monitoreable {
private:
    double altitudMaxima;

public:
    Drone(int id, double velocidad, double altitudMaxima)
        : Vehiculo(id, velocidad), altitudMaxima(altitudMaxima) {}

    void moverse() override {
        cout << "Drone volando en espacio aereo..." << endl;
    }

    void monitorear() override {
        cout << "Monitoreando estabilidad y sensores del Drone." << endl;
    }

    void mostrarInfo() override {
        Vehiculo::mostrarInfo();
        cout << "Altitud maxima: " << altitudMaxima << " m" << endl;
    }
};

// ================= CLASE CAMION =================
class Camion : public Vehiculo, public Monitoreable {
private:
    double capacidadCarga;

public:
    Camion(int id, double velocidad, double capacidadCarga)
        : Vehiculo(id, velocidad), capacidadCarga(capacidadCarga) {}

    void moverse() override {
        cout << "Camion transportando carga pesada..." << endl;
    }

    void monitorear() override {
        cout << "Monitoreando sistema de carga y suspension del Camion." << endl;
    }

    void mostrarInfo() override {
        Vehiculo::mostrarInfo();
        cout << "Capacidad de carga: " << capacidadCarga << " toneladas" << endl;
    }
};

// ================= CLASE AMIGA =================
class Reporte {
public:
    static void generarReporte(const Vehiculo& v) {
        cout << "\n--- REPORTE DEL VEHICULO ---" << endl;
        cout << "ID interno: " << v.id << endl;
        cout << "Velocidad actual: " << v.velocidad << endl;
    }
};

// ================= POLIMORFISMO ESTATICO =================
template <typename T>
T calcularEficiencia(T consumo, T distancia) {
    return distancia / consumo;
}

// ================= MAIN =================
int main() {

    vector<unique_ptr<Vehiculo>> flota;

    flota.push_back(make_unique<Auto>(1, 80.0, 4));
    flota.push_back(make_unique<Drone>(2, 120.0, 500));
    flota.push_back(make_unique<Camion>(3, 60.0, 20));

    cout << "==== SISTEMA DE GESTION DE FLOTA ====\n" << endl;

    for (const auto& vehiculo : flota) {
        vehiculo->moverse();
        vehiculo->mostrarInfo();

        // Uso de clase amiga
        Reporte::generarReporte(*vehiculo);

        cout << "------------------------------------\n";
    }

    // Polimorfismo estatico
    cout << "\nEficiencia Auto: "
         << calcularEficiencia<double>(8.0, 400.0)
         << " km por litro" << endl;

    return 0;
}