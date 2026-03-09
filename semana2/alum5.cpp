#include <iostream>
using namespace std;
int main()
{
     int n, m, z, promedio = 0;
     cout << "ingrese el primer numero: ";
     cin >> n;
     cout << "ingrese el segundo numero: ";
     cin >> m;
     cout << "ingrese el tercer numero: ";
     cin >> z;
     promedio = (n + m + z) / 3;
     cout << "el promedio es : " << promedio << endl;
     return 0;
}