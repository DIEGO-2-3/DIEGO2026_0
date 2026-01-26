#include <iostream>
#include <cmath>

using namespace std;

int main() {
    float a, b, c, p, area;

    cout << "Ingrese lado a: "; cin >> a;
    cout << "Ingrese lado b: "; cin >> b;
    cout << "Ingrese lado c: "; cin >> c;
    p = (a + b + c) / 2;
    area = sqrt(p * (p - a) * (p - b) * (p - c));
    cout << "El semiperimetro es: " << p << endl;
    cout << "El area (Heron) es: " << area << endl;

    return 0;
}