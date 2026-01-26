#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float L1, L2, L3, R, A, lado_1 = 0, lado_2 = 0, lado_3 = 0, radio =0, area = 0;
    cout << "ingrese el lado 1 del triangulo inscrito: ";
    cin >> L1;
    cout << "ingrese el lado 2 del triangulo inscrito: ";
    cin >> L2;
    cout << "ingrese la base del triangulo inscrito: ";
    cin >> L3;
    cout << "ingrese la radio del triangulo inscrito: ";
    cin >> R;
    area = L1 * L2 * L3 / 4 *R;
    cout << " el area del triangulo inscrito es: " << area <<endl;
    return 0;
}