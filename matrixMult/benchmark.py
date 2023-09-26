import subprocess
import numpy as np
import time
import matplotlib.pyplot as plt

# Define the range of thread counts and matrix sizes to test
thread_counts = [1, 2, 4, 8, 16]
matrix_sizes = [128, 256, 512, 1024]

response_times = {}
speedup_data = {}

for n in matrix_sizes:
    response_times[n] = []
    speedup_data[n] = []

for n in matrix_sizes:
    # Create random matrices A and B
    min_value = 30.0  # Minimum value
    max_value = 100.0  # Maximum value
    A = np.random.uniform(min_value, max_value, size=(n, n))
    B = np.random.uniform(min_value, max_value, size=(n, n))
    for num_threads in thread_counts:

        program_name = "./bin"

        # Construct the command to run the C program with arguments
        command = [program_name, str(num_threads), str(n)]

        start_time = time.time()
        process = subprocess.Popen(command, stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True)

        # Construct the input data as you were doing
        input_data = "\n".join(" ".join(str(val) for val in row) for row in A) + \
            "\n" + "\n".join(" ".join(str(val) for val in row) for row in B)

        stdout, stderr = process.communicate(input=input_data)

        end_time = time.time()
        response_time = end_time - start_time

        # Verificar que no haya habido errores en la ejecución
        if process.returncode != 0:
            print(f"Error al ejecutar el programa en C (threads={num_threads}, n={n}):")
            print(stderr)
        else:
            # Obtener la matriz resultante del programa en C
            C = np.array([[float(val) for val in row.split()]
                          for row in stdout.strip().split('\n')])

            # Calcular la multiplicación de matrices en Python
            expected_result = np.dot(A, B)

            # Verificar si las matrices son iguales dentro de una pequeña tolerancia
            tolerance = 1e-6
            if np.allclose(C, expected_result, rtol=tolerance, atol=tolerance):
                print(f"La multiplicación de matrices es correcta (threads={num_threads}, n={n}). time={response_time}")
            else:
                print(f"La multiplicación de matrices es incorrecta (threads={num_threads}, n={n}).")

            response_times[n].append(response_time)

# Calculate speedup
baseline_time = response_times[matrix_sizes[0]][-1]  # Response time with 1 thread for the smallest matrix size
for n in matrix_sizes:
    speedup_data[n] = [response_times[n][0] / rt for rt in response_times[n]]

# Create two graphs
plt.figure(figsize=(10, 5))

# Response Time vs. Number of Threads
plt.subplot(1, 2, 1)
for n in matrix_sizes:
    plt.plot(thread_counts, response_times[n], label=f'n={n}')
plt.xlabel("Number of Threads")
plt.ylabel("Response Time (s)")
plt.title("Response Time vs. Number of Threads")
plt.legend()

# Speedup vs. Number of Threads
plt.subplot(1, 2, 2)
for n in matrix_sizes:
    plt.plot(thread_counts, speedup_data[n], label=f'n={n}')
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs. Number of Threads")
plt.legend()

plt.tight_layout()
plt.savefig('result.png')
plt.show()
