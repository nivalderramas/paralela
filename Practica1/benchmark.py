import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np

# Parámetros
video_input = "video.mp4"
num_threads_list = [1, 2, 4, 8, 16]
response_times = []
speedup = []

with open("results.txt", "w") as results_file:
    for num_threads in num_threads_list:
        start_time = time.time()
        cmd = ["./bin", video_input, "video2.mp4", str(num_threads)]
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, close_fds=True)

        end_time = time.time()

        # Calcular el tiempo de respuesta
        execution_time = end_time - start_time
        response_times.append(execution_time)

        # Escribir el tiempo en el archivo de resultados
        results_file.write(f"Threads: {num_threads}, Time: {execution_time:.4f} seconds\n")

# Calcular el speedup relativo al caso de un solo hilo
baseline_response_time = response_times[0]
speedup = [baseline_response_time / execution_time for execution_time in response_times]

# Generar gráficas
plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.plot(num_threads_list, response_times, marker='o')
plt.xlabel("Número de Hilos")
plt.ylabel("Tiempo de Respuesta (segundos)")
plt.title("Response Time vs. Número de Hilos")

plt.subplot(1, 2, 2)
plt.plot(num_threads_list, speedup, marker='o')
plt.xlabel("Número de Hilos")
plt.ylabel("Speedup (vs. 1 hilo)")
plt.title("Speedup vs. Número de Hilos")

plt.tight_layout()
plt.show()
