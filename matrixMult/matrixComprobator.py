import subprocess
import numpy as np

# Tamaño de las matrices
n = 8

# Crear matrices aleatorias en Python entre 1 y 5 (con decimales)
A = np.random.randint(1, 6, size=(n, n))
B = np.random.randint(1, 6, size=(n, n))

# girar la matriz 90 grados a la derecha

# Ejecutar el programa en C
# Asegúrate de que este sea el nombre correcto del ejecutable
program_name = "./bin"
num_threads = 8

# Construct the command to run the C program with arguments
command = [program_name, str(num_threads), str(n)]

process = subprocess.Popen(command, stdin=subprocess.PIPE,
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE, text=True)

# Construct the input data as you were doing
input_data = "\n".join(" ".join(str(val) for val in row) for row in A) + \
    "\n" + "\n".join(" ".join(str(val) for val in row) for row in B)

stdout, stderr = process.communicate(input=input_data)

# Verificar que no haya habido errores en la ejecución
if process.returncode != 0:
    print("Error al ejecutar el programa en C:")
    print(stderr)
else:
    # Obtener la matriz resultante del programa en C
    C = np.array([[float(val) for val in row.split()]
                 for row in stdout.strip().split('\n')])

    # Calcular la multiplicación de matrices en Python
    expected_result = np.dot(A, B)
    print("Matriz A:")
    print(A)
    print("Matriz B:")
    print(B)
    print("Matriz C:")
    print(C)
    print("Matriz esperada:")
    print(expected_result)

    # Verificar si las matrices son iguales dentro de una pequeña tolerancia
    tolerance = 1e-6
    if np.allclose(C, expected_result, rtol=tolerance, atol=tolerance):
        print("La multiplicación de matrices es correcta.")
    else:
        print("La multiplicación de matrices es incorrecta.")
