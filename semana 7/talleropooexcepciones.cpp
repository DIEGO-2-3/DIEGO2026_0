#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

// --- 4. Excepción Personalizada ---
// Hereda de std::runtime_error según el requisito [cite: 27]
class CuentaNoEncontrada : public std::runtime_error {
public:
    explicit CuentaNoEncontrada(const std::string& mensaje) 
        : std::runtime_error(mensaje) {}
};

// --- 1. Clase CuentaBancaria ---
class CuentaBancaria {
private:
    int numeroCuenta;
    std::string titular;
    double saldo;   

public:
    CuentaBancaria(int numero, std::string nombre, double saldoInicial) 
        : numeroCuenta(numero), titular(nombre), saldo(saldoInicial) {
        if (saldoInicial < 0) {
            throw std::invalid_argument("El saldo inicial no puede ser negativo.");
        }
    }

    void depositar(double monto) {
        //  Uso de std::invalid_argument
        if (monto <= 0) {
            throw std::invalid_argument("El monto a depositar debe ser positivo.");
        }
        saldo += monto;
    }

    void retirar(double monto) {
        if (monto <= 0) {
            throw std::invalid_argument("El monto a retirar debe ser positivo.");
        }
        //  Uso de std::runtime_error por saldo insuficiente
        if (monto > saldo) {
            throw std::runtime_error("Saldo insuficiente para realizar el retiro.");
        }
        saldo -= monto;
    }

    double consultarSaldo() const { return saldo; }
    int getNumeroCuenta() const { return numeroCuenta; }
    std::string getTitular() const { return titular; }
};

// --- 2. Clase Banco ---
class Banco {
private:
    std::vector<CuentaBancaria> cuentas;

public:
    void agregarCuenta(int numero, std::string titular, double saldoInicial) {
        for (const auto& c : cuentas) {
            if (c.getNumeroCuenta() == numero) {
                throw std::invalid_argument("El número de cuenta ya existe.");
            }
        }
        cuentas.emplace_back(numero, titular, saldoInicial);
        std::cout << "Cuenta creada con éxito.\n";
    }

    // Retorna referencia para poder modificar el saldo (depositar/retirar)
    CuentaBancaria& buscarCuenta(int numero) {
        for (auto& c : cuentas) {
            if (c.getNumeroCuenta() == numero) return c;
        }
        // [cite: 28] Lanzar excepción personalizada
        throw CuentaNoEncontrada("Error: La cuenta numero " + std::to_string(numero) + " no existe.");
    }

    void mostrarCuentas() const {
        if (cuentas.empty()) {
            std::cout << "No hay cuentas registradas.\n";
            return;
        }
        std::cout << "\n--- LISTADO DE CUENTAS ---\n";
        for (const auto& c : cuentas) {
            std::cout << "ID: " << c.getNumeroCuenta() << " | Titular: " << c.getTitular() 
                      << " | Saldo: $" << c.consultarSaldo() << "\n";
        }
    }
};

// --- Interfaz de Usuario (Menú) ---
void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Banco miBanco;
    int opcion = 0;

    while (opcion != 6) {
        try {
            std::cout << "\n--- SISTEMA BANCARIO ---\n";
            std::cout << "1. Crear cuenta\n2. Depositar\n3. Retirar\n4. Consultar saldo\n5. Mostrar todas\n6. Salir\n";
            std::cout << "Seleccione una opcion: ";
            
            if (!(std::cin >> opcion)) {
                limpiarBuffer();
                // [cite: 25] Uso de std::out_of_range o similar para entradas inválidas
                throw std::out_of_range("Entrada invalida. Por favor, ingrese un numero.");
            }

            if (opcion < 1 || opcion > 6) {
                throw std::out_of_range("Opcion fuera de rango (1-6).");
            }

            if (opcion == 1) {
                int num; std::string nombre; double ini;
                std::cout << "Numero de cuenta: "; std::cin >> num;
                std::cout << "Titular: "; std::cin.ignore(); std::getline(std::cin, nombre);
                std::cout << "Saldo inicial: "; std::cin >> ini;
                miBanco.agregarCuenta(num, nombre, ini);
            } 
            else if (opcion >= 2 && opcion <= 4) {
                int num;
                std::cout << "Ingrese numero de cuenta: "; std::cin >> num;
                CuentaBancaria& cuenta = miBanco.buscarCuenta(num);

                if (opcion == 2) {
                    double monto; std::cout << "Monto a depositar: "; std::cin >> monto;
                    cuenta.depositar(monto);
                } else if (opcion == 3) {
                    double monto; std::cout << "Monto a retirar: "; std::cin >> monto;
                    cuenta.retirar(monto);
                } else {
                    std::cout << "Saldo actual: $" << cuenta.consultarSaldo() << "\n";
                }
            } 
            else if (opcion == 5) {
                miBanco.mostrarCuentas();
            }

        } 
        // [cite: 38, 39] Múltiples catch por referencia constante en orden correcto
        catch (const CuentaNoEncontrada& e) {
            std::cerr << "EXCEPCION PERSONALIZADA: " << e.what() << "\n";
        } catch (const std::invalid_argument& e) {
            std::cerr << "ERROR DE ARGUMENTO: " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "ERROR DE RANGO: " << e.what() << "\n";
        } catch (const std::runtime_error& e) {
            std::cerr << "ERROR DE EJECUCION: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "ERROR GENERAL: " << e.what() << "\n";
        }
    }

    return 0;
}