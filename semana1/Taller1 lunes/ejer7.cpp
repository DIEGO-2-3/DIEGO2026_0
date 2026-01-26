#include <iostream>

using namespace std;

int main() {
    float a, b, c, r, p, area;
    cout << "Ingrese lado (a): ";
    cin >> a;
    cout << "Ingrese lado (b): ";
    cin >> b;
    cout << "Ingrese lado (c): ";
    cin >> c;
    cout << "Ingrese el radio (r): ";
    cin >> r;
    p = (a + b + c) / 2;
    area = r * p;
    cout << "El semiperimetro (p) es: " << p << endl;
    cout << "El area (S) del triangulo es: " << area << endl;

    return 0;
}