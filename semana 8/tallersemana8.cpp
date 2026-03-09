#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <regex>

using namespace std;

class EntidadNoEncontrada : public runtime_error {
public:
    EntidadNoEncontrada(const string& mensaje) : runtime_error(mensaje) {}
};

class EntradaHistoria {
public:
    string fecha;
    string nota;
    EntradaHistoria(string f, string n) : fecha(f), nota(n) {}
};

class HistoriaClinica {
private:
    vector<EntradaHistoria> entradas;
public:
    void agregarEntrada(string fecha, string nota) {
        entradas.push_back(EntradaHistoria(fecha, nota));
    }
};

class Persona {
protected:
    string nombre;
public:
    Persona(string n) : nombre(n) {}
    virtual ~Persona() = default;
    virtual void mostrar() const = 0;
    string getNombre() const { return nombre; }
};

class Paciente : public Persona {
private:
    string dni;
    int edad;
    string telefono;
    HistoriaClinica historia; 
public:
    Paciente(string n, string d, int e, string t) : Persona(n) {
        if (!regex_match(d, regex("^[0-9]{8}$"))) throw invalid_argument("El DNI debe tener 8 numeros exactos.");
        if (e < 0 || e > 120) throw invalid_argument("La edad debe ser entre 0 y 120.");
        if (!regex_match(t, regex("^[0-9]{9}$"))) throw invalid_argument("El telefono debe tener 9 numeros.");
        dni = d; edad = e; telefono = t;
    }
    string getDni() const { return dni; }
    void mostrar() const override { cout << "Paciente: " << nombre << " | DNI: " << dni << endl; }
};

class Doctor : public Persona {
private:
    string codigo;
    string especialidad;
    double tarifa;
public:
    Doctor(string n, string c, string esp, double tar) : Persona(n) {
        if (tar <= 0) throw invalid_argument("La tarifa debe ser mayor a 0.");
        codigo = c; especialidad = esp; tarifa = tar;
    }
    string getCodigo() const { return codigo; }
    string getEspecialidad() const { return especialidad; }
    void mostrar() const override { cout << "Doctor: " << nombre << " | Esp: " << especialidad << endl; }
};

class Cita {
private:
    string id;
    Paciente* paciente; 
    Doctor* doctor;     
    string fecha;
    string hora;
    string motivo;
public:
    Cita(string i, Paciente* p, Doctor* d, string f, string h, string m) : id(i), paciente(p), doctor(d), fecha(f), hora(h), motivo(m) {
        if (f.empty() || h.empty()) throw invalid_argument("Fecha y hora no pueden estar vacias.");
    }
    string getId() const { return id; }
    Doctor* getDoctor() const { return doctor; }
    Paciente* getPaciente() const { return paciente; }
    string getFecha() const { return fecha; }
    string getHora() const { return hora; }
    string getMotivo() const { return motivo; }
    void mostrarCita() const {
        cout << "Cita [" << id << "] - Fecha: " << fecha << " " << hora << " | Paciente: " << paciente->getNombre() << " | Doctor: " << doctor->getNombre() << " | Motivo: " << motivo << endl;
    }
};

class Clinica {
private:
    vector<unique_ptr<Paciente>> pacientes; 
    vector<unique_ptr<Doctor>> doctores;
    vector<unique_ptr<Cita>> citas;
public:
    void registrarPaciente(string nombre, string dni, int edad, string tel) {
        pacientes.push_back(make_unique<Paciente>(nombre, dni, edad, tel));
        cout << "Paciente registrado.\n";
    }

    void registrarDoctor(string nombre, string codigo, string esp, double tarifa) {
        doctores.push_back(make_unique<Doctor>(nombre, codigo, esp, tarifa));
        cout << "Doctor registrado.\n";
    }

    Paciente* buscarPaciente(string dni) {
        for (auto& p : pacientes) {
            if (p->getDni() == dni) return p.get();
        }
        throw EntidadNoEncontrada("Paciente no encontrado.");
    }

    Doctor* buscarDoctor(string codigo) {
        for (auto& d : doctores) {
            if (d->getCodigo() == codigo) return d.get();
        }
        throw EntidadNoEncontrada("Doctor no encontrado.");
    }

    void agendarCita(string id, string dniPaciente, string codDoctor, string fecha, string hora, string motivo) {
        Paciente* p = buscarPaciente(dniPaciente);
        Doctor* d = buscarDoctor(codDoctor);
        
        for (auto& c : citas) {
            if (c->getDoctor()->getCodigo() == codDoctor && c->getFecha() == fecha && c->getHora() == hora) {
                throw runtime_error("El doctor ya tiene una cita en ese horario.");
            }
        }
        citas.push_back(make_unique<Cita>(id, p, d, fecha, hora, motivo));
        cout << "Cita agendada.\n";
    }

    void cancelarCita(string id) {
        for (auto it = citas.begin(); it != citas.end(); ++it) {
            if ((*it)->getId() == id) {
                citas.erase(it);
                cout << "Cita cancelada.\n";
                return;
            }
        }
        throw runtime_error("La cita no existe.");
    }

    void listarCitasPorPaciente(string dni) {
        bool hayCitas = false;
        cout << "\n--- Citas del Paciente con DNI " << dni << " ---\n";
        for (const auto& c : citas) {
            if (c->getPaciente()->getDni() == dni) {
                c->mostrarCita();
                hayCitas = true;
            }
        }
        if (!hayCitas) cout << "No hay citas registradas para este paciente.\n";
    }

    void listarCitasPorDoctor(string codigo) {
        bool hayCitas = false;
        cout << "\n--- Citas del Doctor con codigo " << codigo << " ---\n";
        for (const auto& c : citas) {
            if (c->getDoctor()->getCodigo() == codigo) {
                c->mostrarCita();
                hayCitas = true;
            }
        }
        if (!hayCitas) cout << "No hay citas registradas para este doctor.\n";
    }
    
    const vector<unique_ptr<Doctor>>& getDoctores() const { return doctores; }
    const vector<unique_ptr<Cita>>& getCitas() const { return citas; }
};

class Reporte {
public:
    static void generarPorEspecialidad(const Clinica& clinica) {
        cout << "\n--- Reporte de Citas por Especialidad ---\n";
        for (const auto& doctor : clinica.getDoctores()) {
            int cont = 0;
            for (const auto& cita : clinica.getCitas()) {
                if (cita->getDoctor()->getCodigo() == doctor->getCodigo()) cont++;
            }
            cout << doctor->getEspecialidad() << ": " << cont << " citas.\n";
        }
    }
};

int main() {
    Clinica miClinica;
    int opcion = 0;

    while (opcion != 8) {
        cout << "\n--- MENU CLINICA SALUD+ ---\n";
        cout << "1) Registrar paciente\n2) Registrar doctor\n3) Agendar cita\n4) Cancelar cita\n5) Listar citas de un paciente\n6) Listar citas de un doctor\n7) Reporte por especialidad\n8) Salir\nElige una opcion: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0;
        }

        try {
            if (opcion == 1) {
                string nombre, dni, tel;
                int edad;
                cout << "Nombre del paciente: "; cin >> ws; getline(cin, nombre);
                cout << "DNI: "; cin >> dni;
                cout << "Edad: "; cin >> edad;
                cout << "Telefono: "; cin >> tel;
                miClinica.registrarPaciente(nombre, dni, edad, tel);
            } 
            else if (opcion == 2) {
                string nombre, cod, esp;
                double tarifa;
                cout << "Nombre del doctor: "; cin >> ws; getline(cin, nombre);
                cout << "Codigo: "; cin >> cod;
                cout << "Especialidad: "; cin >> ws; getline(cin, esp);
                cout << "Tarifa: "; cin >> tarifa;
                miClinica.registrarDoctor(nombre, cod, esp, tarifa);
            } 
            else if (opcion == 3) {
                string id, dni, cod, fecha, hora, motivo;
                cout << "ID de la cita: "; cin >> id;
                cout << "DNI del paciente: "; cin >> dni;
                cout << "Codigo del doctor: "; cin >> cod;
                cout << "Fecha (YYYY-MM-DD): "; cin >> fecha;
                cout << "Hora (HH:MM): "; cin >> hora;
                cout << "Motivo: "; cin >> ws; getline(cin, motivo);
                miClinica.agendarCita(id, dni, cod, fecha, hora, motivo);
            } 
            else if (opcion == 4) {
                string id;
                cout << "ID de la cita a cancelar: "; cin >> id;
                miClinica.cancelarCita(id);
            }
            else if (opcion == 5) {
                string dni;
                cout << "DNI del paciente: "; cin >> dni;
                miClinica.listarCitasPorPaciente(dni);
            }
            else if (opcion == 6) {
                string cod;
                cout << "Codigo del doctor: "; cin >> cod;
                miClinica.listarCitasPorDoctor(cod);
            }
            else if (opcion == 7) {
                Reporte::generarPorEspecialidad(miClinica);
            } 
            else if (opcion < 1 || opcion > 8) {
                throw out_of_range("Opcion invalida. Intenta de nuevo.");
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}