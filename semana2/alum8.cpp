#include <iostream>
using namespace std;

int main()
{
     float lado, perimetro;

     cout << "Ingresa la longitud del lado del cuadrado: ";
     cin >> lado;

     perimetro = 4 * lado;

     cout << "El perimetro del cuadrado es: " << perimetro << endl;

     return 0;
}