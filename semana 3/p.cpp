#include <iostream>
using namespace std;

void invertirArreglo(int arr[], int tam) {
    for(int i = 0; i < tam / 2; i++) {
        int aux = arr[i];
        arr[i] = arr[tam - 1 - i];
        arr[tam - 1 - i] = aux;
    }
}

void mostrar(int arr[], int tam) {
    for(int i = 0; i < tam; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int numeros[] = {1, 2, 3, 4, 5, 6};
    cout << "Original: "; mostrar(numeros, 6);
    
    invertirArreglo(numeros, 6);
    
    cout << "Invertido: "; mostrar(numeros, 6);
    return 0;
}