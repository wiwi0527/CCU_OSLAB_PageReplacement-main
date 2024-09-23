import pickle
import matplotlib.pyplot as plt

# Load the deviations and reuse distance data
with open("perf.data_PmuTime_PhyAddrs.txt_devs.bin", 'rb') as devs_file:
    deviations = pickle.load(devs_file)

"""
with open("reuse_distance", 'rb') as reuse_distance_file:
    reuse_distances = pickle.load(reuse_distance_file)
"""

# Create a hist plot
plt.figure(figsize=(100, 50),dpi= 100)
#plt.hist( deviations, color='blue', label='Deviations', bins= 250 )

plt.scatter(range(len(deviations)), deviations, color='red', label='Reuse Distance', s=0.1)

#plt.scatter(range(len(reuse_distances)), reuse_distances, color='red', label='Reuse Distance', s=10)
plt.title('Deviations and Reuse Distance Scatter Plot')
plt.xlabel('Index')
plt.ylabel('Value')
plt.legend()
plt.grid(True)
plt.show()
