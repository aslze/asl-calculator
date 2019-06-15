import math
import time

y = 0.0
n = 10000;

t1 = time.perf_counter()

for i in range(n):
	y = y -3.5*math.cos(i*5)+math.pi*(-2+math.sqrt(i*math.pi**2))/3
t2 = time.perf_counter()

print(y)

print(1e6 * (t2-t1) / n, "us")


