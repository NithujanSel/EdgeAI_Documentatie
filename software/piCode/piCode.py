from mpi4py import MPI
import random

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    print("Calculating pi with {} devices".format(size))

# Number of random points to use in the calculation
num_points = 10000000

# Number of points inside the unit circle
inside_circle = 0

# Generate the random points and check if they are inside the unit circle
for i in range(rank, num_points, size):
    x = random.uniform(-1, 1)
    y = random.uniform(-1, 1)
    if x*x + y*y <= 1:
        inside_circle += 1

# Use MPI to collect the results from all devices
inside_circle = comm.reduce(inside_circle, op=MPI.SUM, root=0)

if rank == 0:
    # Calculate pi
    pi = 4 * inside_circle / num_points
    print("Pi: ", pi)
    print("MPI test successful.")