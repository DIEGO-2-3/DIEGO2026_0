#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float B, b, H, A, base_mayor = 0, base_menor = 0, altura = 0, area = 0;
    cout << "ingrese la base mayor del trapecio: ";
    cin >> B;
    cout << "ingrese la base menor del trapecio: ";
    cin >> b;
    cout << "ingrese la altura del trapecio: ";
    cin >> H;
    area = (B+b) * H /2;
    cout << " el area del trapecio es: " << area <<endl;
    return 0;
}