n = float(input("Ingresa el primer numero: "))
m = float(input("Ingresa el segundo numero: "))

if n > m:
    print(n, "es mayor que", m)
elif m > n:
    print(m, "es mayor que", n)
else:
    print("Ambos numeros son iguales.")