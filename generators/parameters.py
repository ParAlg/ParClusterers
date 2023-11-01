import numpy as np

# modularity
for i in range(1, 40, 4):
  print(0.02 * (1.2 ** i))

# ParHAC resolution parameters
print("Parhac")
a = 0.5
end = 15 # adjust this as needed
sequence = [a**i for i in np.arange(0, end, end/10)]
print('; '.join(map(str, sequence)))


# WCC
start = np.log10(0.99)
stop = np.log10(0.7)

exp_values = np.logspace(start, stop, num=10)
print('; '.join(map(str, exp_values )))


#tectonic
values = []
for i in range(1, 300, 10):
  values.append(0.01* i)
print('; '.join(map(str, values )))

# parallel affinity
# start = np.log10(0.9)
# stop = np.log10(1e-5)

# exp_values = np.logspace(start, stop, num=10)
# formatted_values = ['{{weight_decay_function: EXPONENTIAL_DECAY, upper_bound: 1, lower_bound: {:.5f}}}'.format(val) for val in exp_values]
# print('; '.join(formatted_values))