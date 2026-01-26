#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float D, d, A, L, P, diagonal_mayor = 0, diagonal_menor = 0, area = 0, lado = 0, perimetro = 0;
    cout << "ingrese el diagonal mayor: ";
    cin >> D;
    cout << "ingrese el diagonal menor: ";
    cin >> d;
    area = D * d / 2;
    lado = sqrt((pow(D/2,2)) + (pow(d/2,2)));
    perimetro = 4 * lado;
    cout << " el area del rombo es " << area <<endl;
    cout << " el lado del rombo es " << lado <<endl;
    cout << " el perimetro del rombo es " << perimetro <<endl;
    return 0;
}