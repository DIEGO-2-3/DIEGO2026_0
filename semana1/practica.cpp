#include <iostream>

using namespace std;

int main() {
    
    int opcion = 0;
    bool diaCerrado = false;

    
    int units, unitPrice, code;
    
    long long ingresoNeto = 0;
    int contadorVentasValidas = 0;
    long long sumaIngresoVentasValidas = 0; 
    int contadorDevoluciones = 0;
    int contadorInvalidas = 0;
    
    long long mayorIngreso = 0;
    long long menorIngreso = 0;
    bool primerIngresoValido = true; 

    
    long long historialCodigos = 0;
    int cantidadTransacciones = 0; 

    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1) Registrar transaccion" << endl;
        cout << "2) Reporte de ventas del dia" << endl;
        cout << "3) Reporte por transaccion (A / B / C / D)" << endl;
        cout << "4) Cerrar dia" << endl;
        cout << "5) Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: { 
                if (diaCerrado) {
                    cout << "NO SE PUEDEN REGISTRAR MAS TRANSACCIONES. DIA CERRADO." << endl;
                } else {
                    cout << "Ingrese (unidades precio codigo): ";
                    cin >> units >> unitPrice >> code;

                    
                    bool esInvalida = false;
                    
                    if (units == 0 || unitPrice <= 0) {
                        esInvalida = true;
                        contadorInvalidas++;
                    } else {
                        long long ingreso = (long long)units * unitPrice; // 
                        
                        ingresoNeto += ingreso;

                        if (units > 0) { 
                            contadorVentasValidas++;
                            sumaIngresoVentasValidas += ingreso;

                            if (primerIngresoValido) {
                                mayorIngreso = ingreso;
                                menorIngreso = ingreso;
                                primerIngresoValido = false;
                            } else {
                                if (ingreso > mayorIngreso) mayorIngreso = ingreso;
                                if (ingreso < menorIngreso) menorIngreso = ingreso;
                            }
                        } else {
                            contadorDevoluciones++;
                        }

                        int absCode = code;
                        if (absCode < 0) absCode = -absCode;
                        int tempCode = absCode;
                        int sumaDigitos = 0;
                        int conteoDigitos = 0;

                        if (tempCode == 0) {
                            conteoDigitos = 1;
                        } else {
                            while (tempCode > 0) {
                                sumaDigitos += tempCode % 10;
                                tempCode /= 10;
                                conteoDigitos++;
                            }
                        }

                        int tipo = 4; 

                        if (sumaDigitos % 2 == 0 && sumaDigitos % 4 == 0) {
                            tipo = 1;
                        }
                        else if (sumaDigitos % 2 != 0 && sumaDigitos % 6 == 0) {
                            tipo = 2; 
                        }
                        
                        else if (conteoDigitos == 3) {
                            tipo = 3; 
                        }
                        
                        historialCodigos = (historialCodigos * 10) + tipo;
                        cantidadTransacciones++;
                    }
                }
                break;
            }

            case 2: { 
                cout << "INGRESO_NETO=" << ingresoNeto << endl;
                cout << "VENTAS_VALIDAS=" << contadorVentasValidas << endl;
                cout << "DEVOLUCIONES=" << contadorDevoluciones << endl;
                cout << "TRANS_INVALIDAS=" << contadorInvalidas << endl;
                
                
                if (primerIngresoValido) { 
                     cout << "MAYOR_ING=0" << endl;
                     cout << "MENOR_ING=0" << endl;
                } else {
                     cout << "MAYOR_ING=" << mayorIngreso << endl;
                     cout << "MENOR_ING=" << menorIngreso << endl;
                }

                
                if (contadorVentasValidas > 0) {
                    
                    long long promedio = sumaIngresoVentasValidas / contadorVentasValidas;
                    cout << "PROM_VENTA=" << promedio << endl;
                } else {
                    cout << "PROM_VENTA=NO EXISTE" << endl;
                }
                break;
            }

            case 3: { 
                long long tempHist = historialCodigos;
                long long histInvertido = 0;
                int count = cantidadTransacciones;
                
                
                long long aux = tempHist;
                
                
                for(int i = 0; i < count; i++) {
                    int digito = aux % 10;
                    histInvertido = (histInvertido * 10) + digito;
                    aux /= 10;
                }

                // Paso 2: Imprimir descifrando los números
                for(int i = 0; i < count; i++) {
                    int tipo = histInvertido % 10;
                    
                    if (tipo == 1) cout << "A";
                    else if (tipo == 2) cout << "B";
                    else if (tipo == 3) cout << "C";
                    else cout << "D";
                    
                    histInvertido /= 10;
                }
                cout << endl; // Salto de línea al final
                break;
            }

            case 4: { // Cerrar día
                diaCerrado = true;
                cout << "Dia cerrado." << endl;
                break;
            }

            case 5: {
                cout << "Saliendo..." << endl;
                break;
            }

            default: {
                cout << "Opcion no valida." << endl;
            }
        }

    } while (opcion != 5);

    return 0;
}