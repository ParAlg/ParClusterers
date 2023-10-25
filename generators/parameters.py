# modularity
for i in range(1, 40, 4):
  print(0.02 * (1.2 ** i))

# ParHAC resolution parameters
a = 0.5
num_terms = 10  # adjust this as needed
sequence = [a**i for i in range(num_terms)]
print('; '.join(map(str, sequence)))


# WCC
import numpy as np
start = np.log10(0.99)
stop = np.log10(0.7)

exp_values = np.logspace(start, stop, num=10)
print('; '.join(map(str, exp_values )))


#tectonic
values = []
for i in range(1, 300, 10):
  values.append(0.01* i)
print('; '.join(map(str, values )))
