#include <iostream>
using namespace std;

int main()
{
     float n, m;

     cout << "Ingresa el primer numero: ";
     cin >> n;

     cout << "Ingresa el segundo numero: ";
     cin >> m;

     if (n > m)
     {
          cout << n << " es mayor que " << m << endl;
     }
     else if (m > n) 
     {
          cout << m << " es mayor que " << n << endl;
     }
     else
     {
          cout << "Ambos numeros son iguales." << endl;
     }

     return 0;
}