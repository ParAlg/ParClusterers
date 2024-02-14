import numpy as np

# modularity
print("Modularity")
sequence = [0.02 * (1.2 ** i) for i in range(1, 40, 4)]
print('; '.join(map(str, sequence)))  
sequence = [0.02 * (1.2 ** i) for i in range(40, 100, 10)]
formatted_values = ['{:.2f}'.format(val) for val in sequence]
print('; '.join(formatted_values))
sequence = [0.02 * (1.2 ** i) for i in range(100, 200, 20)]
formatted_values = ['{:.2f}'.format(val) for val in sequence]
print('; '.join(formatted_values))
sequence = [0.0002, 0.002]
formatted_values = ['{:.5f}'.format(val) for val in sequence]
print('; '.join(formatted_values))

# Par CC
print("ParCC")
sequence = [0.0001 * i for i in np.arange(1, 1000, 1000/5)]
print('; '.join(map(str, sequence)))
sequence = [0.1 * i for i in np.arange(1, 10, 10/5)]
print('; '.join(map(str, sequence)))

# ParHAC resolution parameters
print("Parhac")
a = 0.5
end = 15 # adjust this as needed
sequence = [a**i for i in np.arange(0, end, end/10)]
print('; '.join(map(str, sequence)))

print("Parhac large")
a = 0.5
sequence = [a**i for i in np.arange(end, end+15, 1.5)]
print('; '.join(map(str, sequence)))


# WCC
print("WCC")
values = []
for i in np.arange(50, 99, 1):
  values.append(0.01* i)
formatted_values = ['{:.2f}'.format(val) for val in values]
print('; '.join(formatted_values))


#tectonic
print("Tectonic")
values = []
for i in np.arange(1, 40, 40/40):
  values.append(0.01* i)
formatted_values = ['{:.2f}'.format(val) for val in values]
print('; '.join(formatted_values))

# parallel affinity
# start = np.log10(0.9)
# stop = np.log10(1e-5)

# exp_values = np.logspace(start, stop, num=10)
# formatted_values = ['{{weight_decay_function: EXPONENTIAL_DECAY, upper_bound: 1, lower_bound: {:.5f}}}'.format(val) for val in exp_values]
# print('; '.join(formatted_values))