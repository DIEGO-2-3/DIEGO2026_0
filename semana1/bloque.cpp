#include <stdio.h>
#include <math.h>

int main() {
    int val_a, val_b, val_c;
    float s_p, area_heron;
    printf("Introduzca medida de lado A: ");
    scanf("%d", &val_a);
    printf("Introduzca medida de lado B: ");
    scanf("%d", &val_b);
    printf("Introduzca medida de lado C: ");
    scanf("%d", &val_c);
    s_p = (float)(val_a + val_b + val_c) / 2;
    area_heron = sqrt(s_p * (s_p - val_a) * (s_p - val_b) * (s_p - val_c));
    printf("\n--- RESULTADOS DEL TEOREMA ---\n");
    printf("Semiperimetro: %.2f\n", s_p);
    printf("Area Calculada: %.4f unidades\n", area_heron);

    return 0;
}