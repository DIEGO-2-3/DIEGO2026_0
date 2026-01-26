#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float L1, L2, B, H, A, P, lado_1 = 0, lado_2 = 0, base = 0, altura = 0, area = 0, perimetro = 0;
    cout << "ingrese el lado 1 del triangulo: ";
    cin >> L1;
    cout << "ingrese el lado 2 del triangulo: ";
    cin >> L2;
    cout << "ingrese la base del triangulo: ";
    cin >> B;
    cout << "ingrese la altura del triangulo: ";
    cin >> H;
    area = B * H / 2;
    perimetro = L1 + B + L2;
    cout << " el area del triangulo es: " << area <<endl;
    cout << " el perimetro del triangulo es: " << perimetro <<endl;
    return 0;
}