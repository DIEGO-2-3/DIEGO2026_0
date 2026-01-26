#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float A, P, D, L, area = 0, perimetro = 0, diagonal = 0, lado = 0;
    cout << "ingrrese el lado del cudrado: ";
    cin >> L;
    area = L * L;
    perimetro = 6 * L;
    diagonal = L * sqrt(2);
    cout << " el area del cuadrado es " << area <<endl;
    cout << " el perimetro del cuadrado es " << perimetro <<endl;
    cout << " el diagonal del cuadrado es " << diagonal <<endl;
    return 0;
}