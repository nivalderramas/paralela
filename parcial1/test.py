import subprocess
import matplotlib.pyplot as plt
import time

# Compile the C program if not already compiled
# You can compile it manually with: gcc -o pi_calculation pi_calculation.c -fopenmp

# Define the number of threads to test
num_threads_list = [1, 2, 4, 8, 16]

# Lists to store response times and speedup
response_times = []
speedup = []

# Path to the compiled C binary
binary_path = "./pi_calculation_no_fs"

# Execute the binary with different numbers of threads
for num_threads in num_threads_list:
    start_time = time.time()
    command = [binary_path, str(num_threads)]
    subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    end_time = time.time()
    execution_time = end_time - start_time
    response_times.append(execution_time)

# Calculate speedup relative to the single-threaded execution
speedup.append(1.0)  # Speedup for 1 thread is 1 (baseline)
for i in range(1, len(num_threads_list)):
    speedup_value = response_times[0] / response_times[i]
    speedup.append(speedup_value)

# Plot the response time and speedup
plt.figure(figsize=(10, 5))

# Response time plot
plt.subplot(1, 2, 1)
plt.plot(num_threads_list, response_times, marker='o')
plt.xlabel('Number of Threads')
plt.ylabel('Response Time (s)')
plt.title('Response Time vs. Number of Threads')

# Speedup plot
plt.subplot(1, 2, 2)
plt.plot(num_threads_list, speedup, marker='o')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('Speedup vs. Number of Threads')

plt.tight_layout()
plt.savefig('response_time_and_speedup_no_fs.png')
plt.show()

