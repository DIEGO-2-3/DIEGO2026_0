 #include <iostream>   // Permite la entrada y salida de datos (cin y cout).
#include <string>     // Permite el uso de cadenas de texto (nombres de productos).
 #include <vector>     // Permite crear una lista dinámica para guardar muchos productos.
  
using namespace std;  // Evita escribir "std::" antes de cada comando.
 
struct Producto {     // Crea un nuevo tipo de dato que agrupa información relacionada.
     string nombre;    // Guarda el nombre del insumo.
     float stockFinal; // Guarda el resultado del cálculo matemático.
     bool alerta;      // Guarda un valor de "verdadero" o "falso" para la alerta.
 };

float validarEntrada(string mensaje) { 
    float valor;
         cout << mensaje; // Muestra el texto pidiendo el dato (ej. "Stock inicial").
         cin >> valor;    // Lee el número ingresado.
     
     // Bucle WHILE: Se repite MIENTRAS el valor sea menor a 0.
     while (valor < 0) { 
         cout << "Error: No se permiten valores negativos. Reintente: ";
         cin >> valor; // Pide el dato nuevamente hasta que sea correcto.
    }
     return valor; // Devuelve el número válido al programa principal.
 }
 int main() { // Punto de inicio obligatorio de cualquier programa en C++.
     int n;
         cout << "SISTEMA DE INVENTARIO MULTIPRODUCTO" << endl;
         cout << "Cuantos productos desea registrar? ";
         cin >> n; // Guarda la cantidad de veces que se repetirá el registro.
 
     // Crea una lista (vector) llamada 'lista' con capacidad para 'n' productos.
     vector<Producto> lista(n); 
 
     // Bucle FOR: Se repite desde el producto 0 hasta el producto n-1.
     for(int i = 0; i < n; i++) { 
         cout << "\n--- Registro de Producto " << i+1 << " ---" << endl;
         cout << "Nombre: ";
         cin.ignore(); // Limpia la memoria del teclado para que getline funcione bien.
         getline(cin, lista[i].nombre); // Lee el nombre completo (incluso con espacios).
 
         // Llama a la función de validación para asegurar datos positivos.
         float inicial = validarEntrada("Stock inicial (kg): ");
         float consumo = validarEntrada("Consumo de hoy (kg): ");
 
         // Cálculo: Se resta lo consumido al stock inicial.
         lista[i].stockFinal = inicial - consumo;
         
         // Lógica de Alerta: Si el stock es menor a 5, la alerta es verdadera (true).
         lista[i].alerta = (lista[i].stockFinal < 5);
     }
     cout << "\n======= REPORTE FINAL DE INVENTARIO =======" << endl;
     // Otro bucle FOR para mostrar los resultados guardados en la lista.
     for(int i = 0; i < n; i++) {
         cout << "- " << lista[i].nombre << ": " << lista[i].stockFinal << " kg ";
         
         // Estructura IF: Decide qué mensaje mostrar según el valor de 'alerta'.
         if(lista[i].alerta) {
             cout << "[ALERTA: REPOSICION NECESARIA]";
         } else {
             cout << "[ESTADO: OK]";
         }
         cout << endl; // Salto de línea para el siguiente producto.
     }
 
     return 0; // Indica que el programa terminó correctamente.
 }