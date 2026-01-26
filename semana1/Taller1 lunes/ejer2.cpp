#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float B, H, A, P, D, base = 0, altura = 0, area = 0, perimetro = 0, diagonal = 0;
    cout << "ingrese la base del rectangulo: ";
    cin >> B;
    cout << "ingrese la altura del rectangulo: ";
    cin >> H;
    area = B * H;
    perimetro = 2 * (B+H);
    diagonal = sqrt(pow(B,2) + (pow(H, 2)));
    cout << " el area del rectangulo es " << area <<endl;
    cout << " el perimetro del rectangulo es " << perimetro <<endl;
    cout << " el diagonal del rectangulo es " << diagonal <<endl;
    return 0;
}